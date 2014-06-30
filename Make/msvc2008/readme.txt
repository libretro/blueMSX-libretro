BlueMSX can be build with vs2008, but unfortunately, it is not as easy as with VS2003 since the 
ATL-libraries are nolonger shipped with vs2005 and up.

However, they are part of the platform SDK that can be installed seporately.
Windows Server Platform SDK 2003 R2 is the last version to ship with ATL, 
"Windows Server Platform SDK 2008" nolonger has them.

BlueMSX will also require the DirectX SDK, I recommend the August 2007 version, since the current version
(August 2008, at the time of this writing) is missing dxtrans.h which it required by directshow.


- install vs2008                (ie. Microsoft Visual C++ 2008 Express Edition ISO +400MB)
- install the platform SDK      (ie. Microsoft Platform SDK R2.iso 409MB)
- install the DirectX SDK       (ie. dxsdk_aug2007.exe 468MB)

- add to the "additional include directories":
  c:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Include\atl 
  

- add to the pre-compiler directives: (to get rid of the clutter in the build-output window)
  _CRT_SECURE_NO_WARNINGS
 
- add to Linker->General->Additional library Directories:
   "C:\Program Files\Microsoft DirectX SDK (August 2007)\Lib\x86"


if you're missing atlthunk.lib, find altbase.h in C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2\Include\atl 
and comment the line:
#pragma comment(lib, "atlthunk.lib")

alternatively, you could probably install the DDK
(http://www.microsoft.com/whdc/DevTools/ddk/default.mspx) that
contains atlthunk.lib but I understand it is only needed for 64-bit compatibility.

if you get linker errors about "ConvertBSTRToString"
- add to additional dependencies: comsuppw.lib to release builds, and comsuppwd.lib to debug build

if yet get a linker error like "Fatal error CVT1100: duplicate resource. type:MANIFEST, name:1, language:0×0409 CVTRES”)"
- comment out the manifest line in blueMSX.rc like:
	// 1                       24      MOVEABLE PURE   "bluemsx.manifest"

if you get a linker error "LINK : warning LNK4098: defaultlib 'LIBCMT' conflicts with use of other libs; use /NODEFAULTLIB:library"
- add to Linker->command line: /nodefaultlib:"libcmt.lib"

Comments by Jan Wilmans, Jan 23, 2009, check sourceforge for email address
