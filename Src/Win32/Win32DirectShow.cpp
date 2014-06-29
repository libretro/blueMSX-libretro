/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32DirectShow.cpp,v $
**
** $Revision: 1.11 $
**
** $Date: 2008-03-30 18:38:48 $
**
** More info: http://www.bluemsx.com
**
** Copyright (C) 2003-2006 Daniel Vik, Tomas Karlsson
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
:
    m_initialized(false)
{
}

CVideoGrabber::~CVideoGrabber()
{
}

void CVideoGrabber::ShutdownGrabber()
{
    
    if (m_initialized) {
        HRESULT hr;
        IMediaControlPtr pControl = m_pGraph;
        hr = pControl->Stop();

#ifdef _DEBUG
        if (m_dwGraphRegister) {
            RemoveGraphFromRot(m_dwGraphRegister);
        }
#endif
        m_pGraph = 0;
        m_initialized = false;
    }
}

HRESULT CVideoGrabber::ShowFilterPropertyPage(HWND hwndParent, IBaseFilter *pFilter)
{
    HRESULT hr;

    if(!pFilter) {
        return E_POINTER;
    }

    ISpecifyPropertyPagesPtr pPropertyPage;
    hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **) &pPropertyPage);
    if(SUCCEEDED(hr)) {
        FILTER_INFO FilterInfo;
        hr = pFilter->QueryFilterInfo(&FilterInfo);
        if(FAILED(hr)) {
            return hr;
        }

        CAUUID caGUID;
        hr = pPropertyPage->GetPages(&caGUID);
        if (FAILED(hr)) {
            return hr;
        }
    
        OleCreatePropertyFrame(hwndParent, 0, 0, FilterInfo.achName, 1, (IUnknown **)&pFilter, caGUID.cElems, caGUID.pElems, 0, 0, NULL);

        CoTaskMemFree(caGUID.pElems);
    }
        
    return hr;
}

void CVideoGrabber::ShowProperties(HWND hwndParent, const std::string& devName)
{
    IBaseFilterPtr pSource;

    pSource = GetCapDevice(devName);
    if (pSource) {
        ShowFilterPropertyPage(hwndParent, pSource);
    }
}

bool CVideoGrabber::SetupGrabber(const std::string& devName)
{
    ISampleGrabberPtr pGrabber;
    IBaseFilterPtr    pSource;
    IVideoWindowPtr   pVideoWindow;
    ICaptureGraphBuilder2Ptr pBuilder;
    IAMStreamConfig *pConfig;
    HRESULT hr;

    if (m_initialized) {
        ShutdownGrabber();
    }

    hr = pBuilder.CreateInstance(CLSID_CaptureGraphBuilder2);
    if (FAILED(hr)) {
        return false;
    }

    hr = pGrabber.CreateInstance(CLSID_SampleGrabber);
    if (FAILED(hr)) {
        return false;
    }

    pSource = GetCapDevice(devName);
    if (pSource == NULL) {
        return false;
    }

    hr = m_pGraph.CreateInstance(CLSID_FilterGraph);
    if (FAILED(hr)) {
        return false;
    }

    hr = m_pGraph->AddFilter(pSource, L"Source");
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    IBaseFilterPtr pGrabberBase(pGrabber);
    hr = m_pGraph->AddFilter(pGrabberBase, L"Grabber");
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    IBaseFilterPtr pRenderer;
    hr = pRenderer.CreateInstance(CLSID_NullRenderer);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }
    hr = m_pGraph->AddFilter(pRenderer, L"Null Renderer");
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pSource, IID_IAMStreamConfig, (void **)&pConfig);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }
    hr = SetupVideoStreamConfig(pConfig);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    CMediaType GrabType;
    GrabType.SetType(&MEDIATYPE_Video);
    GrabType.SetSubtype(&MEDIASUBTYPE_RGB555);
    hr = pGrabber->SetMediaType(&GrabType);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    IPinPtr pSourcePin;
    IPinPtr pGrabPin;
    pSourcePin = GetOutPin(pSource, 0);
    pGrabPin = GetInPin(pGrabberBase, 0);
    hr = m_pGraph->Connect(pSourcePin, pGrabPin);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    AM_MEDIA_TYPE mt;
    hr = pGrabber->GetConnectedMediaType(&mt);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
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

    IPinPtr pGrabOutPin = GetOutPin(pGrabberBase, 0);
    hr = m_pGraph->Render(pGrabOutPin);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    hr = pGrabber->SetBufferSamples(FALSE);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }
    hr = pGrabber->SetOneShot(TRUE);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }
    hr = pGrabber->SetCallback(&CB, 1);
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    IVideoWindowPtr pWindow = m_pGraph;
    if (pWindow) {
        hr = pWindow->put_AutoShow(OAFALSE);
    }

#ifdef _DEBUG
    m_dwGraphRegister = 0;
    hr = AddGraphToRot(m_pGraph, &m_dwGraphRegister);
#endif

    IMediaControlPtr pControl(m_pGraph);
    hr = pControl->Run();
    if (FAILED(hr)) {
        m_pGraph = 0;
        return false;
    }

    m_initialized = true;

    return true;
}

int CVideoGrabber::GrabFrame(WORD* bitmap, LONG width, LONG height)
{
    HRESULT hr;
    long EvCode = 0;

    if (!m_initialized) {
        return 0;    
    }

    IMediaEventPtr pEvent(m_pGraph);

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
    IEnumPinsPtr pEnum;
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
    IPinPtr pComPin=0;
    GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
    return pComPin;
}

IPin *CVideoGrabber::GetOutPin(IBaseFilter *pFilter, int nPin)
{
    IPinPtr pComPin=0;
    GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
    return pComPin;
}

CVideoGrabber::DeviceNameList CVideoGrabber::GetDeviceNames() const
{
    DeviceNameList nameList;
    HRESULT hr;

    ICreateDevEnumPtr pCreateDevEnum;
    hr = pCreateDevEnum.CreateInstance(CLSID_SystemDeviceEnum);
    if (FAILED(hr)) {
        return nameList;
    }

    if (pCreateDevEnum == NULL) {
        return nameList;
    }

    IEnumMonikerPtr pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (FAILED(hr)) {
        return nameList;
    }

    if (pEm == NULL) {
        return nameList;
    }

    hr = pEm->Reset();
    if (FAILED(hr)) {
        return nameList;
    }

    while (true) {
        ULONG ulFetched = 0;
        IMonikerPtr pM;
        hr = pEm->Next(1, &pM, &ulFetched);
        if (hr != S_OK) {
            break;
        }

        IPropertyBagPtr pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if (hr != S_OK) {
            continue;
        }

        _variant_t varName;
        varName.vt = VT_BSTR;
        hr = pBag->Read( L"FriendlyName", &varName, NULL);
        if (hr != S_OK) {
            continue;
        }
        _bstr_t bstrTemp = varName;
        LPCSTR  szTemp = (LPCSTR) bstrTemp;

        nameList.push_back(szTemp);
    }

    return nameList;
}

IBaseFilter* CVideoGrabber::GetCapDevice(const std::string& devName)
{
    HRESULT hr;

    m_szDeviceName.clear();

    ICreateDevEnumPtr pCreateDevEnum;
    hr = pCreateDevEnum.CreateInstance(CLSID_SystemDeviceEnum);
    if (FAILED(hr)) {
        return NULL;
    }

    if (pCreateDevEnum == NULL) {
        return NULL;
    }

    IEnumMonikerPtr pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (FAILED(hr)) {
        return NULL;
    }

    if (pEm == NULL) {
        return NULL;
    }

    hr = pEm->Reset();
    if (FAILED(hr)) {
        return NULL;
    }

    IBaseFilter* pCap = NULL;

    while (true) {
        ULONG ulFetched = 0;
        IMonikerPtr pM;
        hr = pEm->Next(1, &pM, &ulFetched);
        if (hr != S_OK) {
            break;
        }

        IPropertyBagPtr pBag;
        hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
        if (hr != S_OK) {
            continue;
        }

        _variant_t varName;
        varName.vt = VT_BSTR;
        hr = pBag->Read( L"FriendlyName", &varName, NULL);
        if (hr != S_OK) {
            continue;
        }
        _bstr_t bstrTemp = varName;
        LPCSTR  szTemp = (LPCSTR) bstrTemp;
        
        if (devName.length() > 0 || devName == szTemp) {
            m_szDeviceName = szTemp;

            hr = pM->BindToObject(0,0, IID_IBaseFilter, (void **)&pCap);
            if (pCap) {
                break;
            }
        }
    }
    return pCap;
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

    IRunningObjectTablePtr pROT;
    hr = GetRunningObjectTable(0, &pROT);
    if (FAILED(hr)) {
        return hr;
    }

    WCHAR wsz[128];
    hr = StringCchPrintfW(wsz, NUMELMS( wsz ), L"FilterGraph %08x pid %08x\0", (DWORD_PTR) pUnkGraph, GetCurrentProcessId());

    IMonikerPtr pMoniker;
    hr = CreateItemMoniker(L"!", wsz, &pMoniker);
    if (SUCCEEDED(hr)) {
        hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister);
    }

    return hr;
}

void CVideoGrabber::RemoveGraphFromRot(DWORD pdwRegister)
{
    HRESULT hr;
    IRunningObjectTablePtr pROT;

    hr = GetRunningObjectTable(0, &pROT);
    if (SUCCEEDED(hr)) {
        pROT->Revoke(pdwRegister);
    }
}
#endif

const std::string& CVideoGrabber::GetName() const
{  
    return m_szDeviceName;
}
