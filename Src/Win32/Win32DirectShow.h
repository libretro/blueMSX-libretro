/*****************************************************************************
** $Source: /cvsroot/bluemsx/blueMSX/Src/Win32/Win32DirectShow.h,v $
**
** $Revision: 1.3 $
**
** $Date: 2006/07/03 19:25:45 $
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
#ifndef WIN32_DIRECTSHOW
#define WIN32_DIRECTSHOW

#include <windows.h>
#include <streams.h>
#include <stdio.h>
#include <atlbase.h>
#include <qedit.h>

class CVideoGrabber
{
public:
    CVideoGrabber();
    virtual ~CVideoGrabber();

    int SetupGrabber();
    void ShutdownGrabber();
    char *GetName();

    int GrabFrame(WORD* bitmap, LONG width, LONG height);

private:
    char m_szDeviceName[MAX_PATH];
    CComPtr <IGraphBuilder>  m_pGraph;
    HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
    IPin *GetInPin (IBaseFilter *pFilter, int Num);
    IPin *GetOutPin(IBaseFilter *pFilter, int Num);

    void GetDefaultCapDevice(IBaseFilter **ppCap);
    HRESULT SetupVideoStreamConfig(IAMStreamConfig *pSC);

#ifdef _DEBUG
    DWORD m_dwGraphRegister;
    HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
    void RemoveGraphFromRot(DWORD pdwRegister);
#endif
};

#endif //WIN32_DIRECTSHOW
