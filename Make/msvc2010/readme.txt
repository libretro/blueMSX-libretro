BlueMSX can be build with VS2010.

However, it also requires the qedit.h for the Sample Grabber, they are part of the Windows SDK
that can be installed separately.

BlueMSX will also require the DirectX SDK, I recommend the August 2007 version, since the current version
(June 2010, at the time of this writing) is missing dxtrans.h which it required by DirectShow.


- install VS2010                (ie. Microsoft Visual C++ 2010 Professional or Express)
- install the Windows SDK       (ie. Microsoft Windows Software Development Kit Update for Windows Vista)
- install the DirectX SDK       (ie. dxsdk_aug2007.exe)

- the "additional include directories" are already set in the project to:
  C:\Program Files\Microsoft SDKs\Windows\v6.1\Include
  C:\Program Files\Microsoft DirectX SDK (August 2007)\Include
  
- the "additional library directories" are already set in the project to:
   C:\Program Files\Microsoft SDKs\Windows\v6.1\Lib
   C:\Program Files\Microsoft DirectX SDK (August 2007)\Lib\x86

Comments by Jan Wilmans, Jan 23, 2009, check sourceforge for email address
