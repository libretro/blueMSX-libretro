/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32DirectShow.cpp,v $
**
** $Revision: 1.5 $
**
** $Date: 2006/07/04 07:49:05 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vikl, Tomas Karlsson
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
******************************************************************************
*/
#include "Win32DirectShow.h"
#include <strsafe.h>
#include <comutil.h>
#pragma comment(lib, "comsupp.lib")

typedef struct _callbackinfo {
    long lBufferSize;
    BYTE *pBuffer;
    BITMAPINFOHEADER bih;
    DWORD biSize;

} CALLBACKINFO;

CALLBACKINFO cbInfo = {0};

class CSampleGrabberCB : public ISampleGrabberCB 
{

public:

    long Width;
    long Height;

    STDMETHODIMP_(ULONG) AddRef()
    {
        return 2;
    }

    STDMETHODIMP_(ULONG) Release()
    {
        return 1;
    }

    STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        if ((ppv) == NULL) {
            return (E_POINTER);
        }

        if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) {
            *ppv = (void *) static_cast <ISampleGrabberCB*> (this);
            return NOERROR;
        }    

        return E_NOINTERFACE;
    }

    STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample)
    {
        return 0;
    }

    STDMETHODIMP BufferCB(double dblSampleTime, BYTE *pBuffer, long lBufferSize)
    {
        if (!pBuffer) {
            return E_POINTER;
        }

        if (cbInfo.lBufferSize < lBufferSize) {
            delete [] cbInfo.pBuffer;
            cbInfo.pBuffer = NULL;
            cbInfo.lBufferSize = 0;
        }

        if (!cbInfo.pBuffer) {
            cbInfo.pBuffer = new BYTE[lBufferSize];
            cbInfo.lBufferSize = lBufferSize;
        }

        if (!cbInfo.pBuffer) {
            cbInfo.lBufferSize = 0;
            return E_OUTOFMEMORY;
        }

        memcpy(cbInfo.pBuffer, pBuffer, lBufferSize);

        return 0;
    }
};

CSampleGrabberCB CB;

CVideoGrabber::CVideoGrabber()
{
}

CVideoGrabber::~CVideoGrabber()
{
}

void CVideoGrabber::ShutdownGrabber()
{
    HRESULT hr;
    CComQIPtr <IMediaControl, &IID_IMediaControl> pControl = m_pGraph;

    hr = pControl->Stop();

#ifdef _DEBUG
    if (m_dwGraphRegister) {
        RemoveGraphFromRot(m_dwGraphRegister);
    }
#endif
}

int CVideoGrabber::SetupGrabber()
{
    CComPtr <ISampleGrabber> pGrabber;
    CComPtr <IBaseFilter>    pSource;
    CComPtr <IVideoWindow>   pVideoWindow;
    CComPtr <ICaptureGraphBuilder2> pBuilder;
    IAMStreamConfig *pConfig;
    HRESULT hr;

    hr = pBuilder.CoCreateInstance(CLSID_CaptureGraphBuilder2);
    if (FAILED(hr)) {
        return 0;
    }

    hr = pGrabber.CoCreateInstance(CLSID_SampleGrabber);
    if (FAILED(hr)) {
        return 0;
    }

    GetDefaultCapDevice(&pSource);
    if (!pSource) {
        return 0;
    }

    hr = m_pGraph.CoCreateInstance(CLSID_FilterGraph);
    if (FAILED(hr)) {
        return 0;
    }

    hr = m_pGraph->AddFilter(pSource, L"Source");
    if (FAILED(hr)) {
        return 0;
    }

    CComQIPtr <IBaseFilter, &IID_IBaseFilter> pGrabberBase(pGrabber);
    hr = m_pGraph->AddFilter(pGrabberBase, L"Grabber");
    if (FAILED(hr)) {
        return 0;
    }

    CComPtr <IBaseFilter> pRenderer;
    hr = pRenderer.CoCreateInstance(CLSID_NullRenderer);
    if (FAILED(hr)) {
        return 0;
    }
    hr = m_pGraph->AddFilter(pRenderer, L"Null Renderer");
    if (FAILED(hr)) {
        return 0;
    }

    hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSource, IID_IAMStreamConfig, (void **)&pConfig);
    if (FAILED(hr)) {
        return 0;
    }
    hr = SetupVideoStreamConfig(pConfig);
    if (FAILED(hr)) {
        return 0;
    }

    CMediaType GrabType;
    GrabType.SetType(&MEDIATYPE_Video);
    GrabType.SetSubtype(&MEDIASUBTYPE_RGB555);
    hr = pGrabber->SetMediaType(&GrabType);
    if (FAILED(hr)) {
        return 0;
    }

    CComPtr <IPin> pSourcePin;
    CComPtr <IPin> pGrabPin;
    pSourcePin = GetOutPin(pSource, 0);
    pGrabPin = GetInPin(pGrabberBase, 0);
    hr = m_pGraph->Connect(pSourcePin, pGrabPin);
    if (FAILED(hr)) {
        return 0;
    }

    AM_MEDIA_TYPE mt;
    hr = pGrabber->GetConnectedMediaType(&mt);
    if (FAILED(hr)) {
        return 0;
    }
    VIDEOINFOHEADER *vih = (VIDEOINFOHEADER*) mt.pbFormat;
    CB.Width = vih->bmiHeader.biWidth;
    CB.Height = vih->bmiHeader.biHeight;
    FreeMediaType(mt);

    memset(&(cbInfo.bih), 0, sizeof(cbInfo.bih));
    cbInfo.bih.biSize = sizeof(cbInfo.bih);
    cbInfo.bih.biWidth = CB.Width;
    cbInfo.bih.biHeight = CB.Height;
    cbInfo.bih.biPlanes = 1;
    cbInfo.bih.biBitCount = 16;

    CComPtr <IPin> pGrabOutPin = GetOutPin(pGrabberBase, 0);
    hr = m_pGraph->Render(pGrabOutPin);
    if (FAILED(hr)) {
        return 0;
    }

    hr = pGrabber->SetBufferSamples(FALSE);
    if (FAILED(hr)) {
        return 0;
    }
    hr = pGrabber->SetOneShot(TRUE);
    if (FAILED(hr)) {
        return 0;
    }
    hr = pGrabber->SetCallback(&CB, 1);
    if (FAILED(hr)) {
        return 0;
    }

    CComQIPtr <IVideoWindow, &IID_IVideoWindow> pWindow = m_pGraph;
    if (pWindow) {
        hr = pWindow->put_AutoShow(OAFALSE);
    }

#ifdef _DEBUG
    m_dwGraphRegister = 0;
    hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
#endif

    CComQIPtr <IMediaControl, &IID_IMediaControl> pControl(m_pGraph);
    hr = pControl->Run();
    if (FAILED(hr)) {
        return 0;
    }

	return 1;
}

int CVideoGrabber::GrabFrame(WORD* bitmap, LONG width, LONG height)
{
    HRESULT hr;
    long EvCode = 0;

    if (!m_pGraph) {
        return 0;    
    }

    CComQIPtr <IMediaEvent, &IID_IMediaEvent> pEvent(m_pGraph);

    hr = pEvent->WaitForCompletion(INFINITE, &EvCode);
    
    if ((cbInfo.bih.biBitCount + 7) / 8 != sizeof(WORD)) {
        return 0;
    }

    // Set up soruce and destination width
    LONG srcWidth   = cbInfo.bih.biWidth;
    LONG srcHeight  = cbInfo.bih.biHeight;
    LONG dstWidth   = width;
    LONG dstHeight  = height;

    // Currently support only 1x and 2x scaling
    LONG srcScaleX = srcWidth  >= 2 * dstWidth  ? 2 : 1;
    LONG srcScaleY = srcHeight >= 2 * dstHeight ? 2 : 1;
    srcWidth  /= srcScaleX;
    srcHeight /= srcScaleY;

    LONG cpyWidth   = srcWidth  < dstWidth  ? srcWidth  : dstWidth;
    LONG cpyHeight  = srcHeight < dstHeight ? srcHeight : dstHeight;
    LONG dstBorderX = dstWidth  > srcWidth  ? (dstWidth  - srcWidth)  / 2 : 0;
    LONG dstBorderY = dstHeight > srcHeight ? (dstHeight - srcHeight) / 2 : 0;
    LONG srcBorderX = srcWidth  > dstWidth  ? (srcWidth  - dstWidth)  / 2 : 0;
    LONG srcBorderY = srcHeight > dstHeight ? (srcHeight - dstHeight) / 2 : 0;

    srcWidth   *= srcScaleX;
    srcHeight  *= srcScaleY;
    srcBorderX *= srcScaleX;
    srcBorderY *= srcScaleY;

    // Set up pointers to source and destination
    WORD* dstBitmap = bitmap                + dstBorderX + dstWidth * dstBorderY;
    WORD* srcBitmap = (WORD*)cbInfo.pBuffer + srcBorderX + srcWidth * srcBorderY + srcWidth * cpyHeight * srcScaleY;

    if (srcScaleX == 1 && srcScaleY == 1) {
        // Fast 1-1 blitting
        for (LONG y = 0; y < cpyHeight; y++) {
            srcBitmap -= srcWidth;
            memcpy(dstBitmap, srcBitmap, sizeof(WORD) * cpyWidth);
            dstBitmap += dstWidth;
        }
    }
    else {
        for (LONG y = 0; y < cpyHeight; y++) {
            srcBitmap -= srcWidth * srcScaleY;
            for (LONG x = 0; x < cpyWidth; x++) {
                dstBitmap[x] = srcBitmap[x * srcScaleX];
            }
            dstBitmap += dstWidth;
        }
    }

    return 1;
}

HRESULT CVideoGrabber::GetPin(IBaseFilter *pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr <IEnumPins> pEnum;
    *ppPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if (FAILED(hr)) {
        return hr;
    }

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound)) {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;

        pPin->QueryDirection(&pindir);
        if (pindir == dirrequired) {
            if (iNum == 0) {
                *ppPin = pPin;
                hr = S_OK;
                break;
            }
            iNum--;
        } 

        pPin->Release();
    } 

    return hr;
}

IPin *CVideoGrabber::GetInPin(IBaseFilter *pFilter, int nPin)
{
    CComPtr <IPin> pComPin=0;
    GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
    return pComPin;
}

IPin *CVideoGrabber::GetOutPin(IBaseFilter *pFilter, int nPin)
{
    CComPtr <IPin> pComPin=0;
    GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
    return pComPin;
}

void CVideoGrabber::GetDefaultCapDevice(IBaseFilter **ppCap)
{
    HRESULT hr;

    ASSERT(ppCap);
    if (!ppCap) {
        return;
    }
    *ppCap = NULL;

    CComPtr <ICreateDevEnum> pCreateDevEnum;
    hr = pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
    if (FAILED(hr)) {
        return;
    }

//    ASSERT(pCreateDevEnum);
    if (!pCreateDevEnum) {
        return;
    }

    CComPtr <IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (FAILED(hr)) {
        return;
    }

//    ASSERT(pEm);
    if (!pEm) {
        return;
    }

    hr = pEm->Reset();
    if (FAILED(hr)) {
        return;
    }

    while (true) {
        ULONG ulFetched = 0;
        CComPtr <IMoniker> pM;
        hr = pEm->Next(1, &pM, &ulFetched);
        if (hr != S_OK) {
            break;
        }

        CComPtr <IPropertyBag> pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if (hr != S_OK) {
            continue;
        }

        CComVariant varName;
        varName.vt = VT_BSTR;
        hr = pBag->Read( L"FriendlyName", &varName, NULL);
        if (hr != S_OK) {
            continue;
        }
        _bstr_t bstrTemp = varName;
        LPCSTR  szTemp = (LPCSTR) bstrTemp;
        StringCchCopy(m_szDeviceName, strlen(szTemp) , szTemp);

        hr = pM->BindToObject(0,0, IID_IBaseFilter, (void **)ppCap);
        if (*ppCap) {
            break;
        }
    }
    return;
}

HRESULT CVideoGrabber::SetupVideoStreamConfig(IAMStreamConfig *pSC)
{
    HRESULT hr;
    VIDEO_STREAM_CONFIG_CAPS scc;
    AM_MEDIA_TYPE *pmt;
    HWND hWnd = NULL;
    int iCount;
    int iSize;

    hr = pSC->GetNumberOfCapabilities(&iCount, &iSize);
    if (sizeof(scc) != iSize) {
        return E_FAIL;
    }

    for (int i=0; i<iCount;i++) {
        hr = pSC->GetStreamCaps(i, &pmt, reinterpret_cast<BYTE*>(&scc));
        if (hr == S_OK) {
            VIDEOINFOHEADER *pVih = reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
            pVih->bmiHeader.biWidth = 640;
            pVih->bmiHeader.biHeight = 480;
            hr = pSC->SetFormat(pmt);
            if(hr != S_OK) {
                hr = E_FAIL;
                continue;
            }
            else {
                break;
            }
         }
    }

    if (hr == S_OK) {
        return S_OK;
    }
    return E_FAIL;
}

#ifdef _DEBUG
HRESULT CVideoGrabber::AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
    HRESULT hr;

    if (!pUnkGraph || !pdwRegister) { 
        return E_POINTER;
    }

    CComPtr <IRunningObjectTable> pROT;
    hr = GetRunningObjectTable(0, &pROT);
    if (FAILED(hr)) {
        return hr;
    }

    WCHAR wsz[128];
    hr = StringCchPrintfW(wsz, NUMELMS( wsz ), L"FilterGraph %08x pid %08x\0", (DWORD_PTR) pUnkGraph, GetCurrentProcessId());

    CComPtr <IMoniker> pMoniker;
    hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister);
    }

    return hr;
}

void CVideoGrabber::RemoveGraphFromRot(DWORD pdwRegister)
{
    HRESULT hr;
    CComPtr <IRunningObjectTable> pROT;

    hr = GetRunningObjectTable(0, &pROT);
    if (SUCCEEDED(hr)) {
        pROT->Revoke(pdwRegister);
    }
}
#endif

char* CVideoGrabber::GetName()
{  
    return m_szDeviceName;
}
