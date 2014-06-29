/*****************************************************************************
** $Source: /cygdrive/d/Private/_SVNROOT/bluemsx/blueMSX/Src/Win32/Win32DirectShow.h,v $
**
** $Revision: 1.8 $
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
#ifndef WIN32_DIRECTSHOW
#define WIN32_DIRECTSHOW

#include <windows.h>
#include <streams.h>
#include <stdio.h>
#include <comdef.h>
#include <qedit.h>
#include <vector>
#include <string>

class CVideoGrabber
{
public:
    typedef std::vector<std::string> DeviceNameList;

    CVideoGrabber();
    virtual ~CVideoGrabber();

    bool SetupGrabber(const std::string& devName = "");
    void ShutdownGrabber();

    DeviceNameList GetDeviceNames() const;
    const std::string& GetName() const;
    void ShowProperties(HWND hwndParent, const std::string& devName);

    int GrabFrame(WORD* bitmap, LONG width, LONG height);

private:
    bool m_initialized;
    std::string m_szDeviceName;
    _COM_SMARTPTR_TYPEDEF(IBaseFilter,           __uuidof(IBaseFilter));
    _COM_SMARTPTR_TYPEDEF(ICaptureGraphBuilder2, __uuidof(ICaptureGraphBuilder2));
    _COM_SMARTPTR_TYPEDEF(ICreateDevEnum,        __uuidof(ICreateDevEnum));
    _COM_SMARTPTR_TYPEDEF(IEnumMoniker,          __uuidof(IEnumMoniker));
    _COM_SMARTPTR_TYPEDEF(IEnumPins,             __uuidof(IEnumPins));
    _COM_SMARTPTR_TYPEDEF(IGraphBuilder,         __uuidof(IGraphBuilder));
    _COM_SMARTPTR_TYPEDEF(IMediaControl,         __uuidof(IMediaControl));
    _COM_SMARTPTR_TYPEDEF(IMediaEvent,           __uuidof(IMediaEvent));
    _COM_SMARTPTR_TYPEDEF(IMoniker,              __uuidof(IMoniker));
    _COM_SMARTPTR_TYPEDEF(IPin,                  __uuidof(IPin));
    _COM_SMARTPTR_TYPEDEF(IPropertyBag,          __uuidof(IPropertyBag));
    _COM_SMARTPTR_TYPEDEF(IRunningObjectTable,   __uuidof(IRunningObjectTable));
    _COM_SMARTPTR_TYPEDEF(ISampleGrabber,        __uuidof(ISampleGrabber));
    _COM_SMARTPTR_TYPEDEF(ISpecifyPropertyPages, __uuidof(ISpecifyPropertyPages));
    _COM_SMARTPTR_TYPEDEF(IVideoWindow,          __uuidof(IVideoWindow));
    IGraphBuilderPtr  m_pGraph;
    HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
    IPin *GetInPin (IBaseFilter *pFilter, int Num);
    IPin *GetOutPin(IBaseFilter *pFilter, int Num);

    IBaseFilter* GetCapDevice(const std::string& devName = "");

    HRESULT SetupVideoStreamConfig(IAMStreamConfig *pSC);
    HRESULT ShowFilterPropertyPage(HWND hwndParent, IBaseFilter *pFilter);

#ifdef _DEBUG
    DWORD m_dwGraphRegister;
    HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
    void RemoveGraphFromRot(DWORD pdwRegister);
#endif
};

#endif //WIN32_DIRECTSHOW
