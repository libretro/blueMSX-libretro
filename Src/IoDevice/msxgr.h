// Stub for eventual future support for the Game Reader

#ifndef _CMSXGR
#define _CMSXGR

class CMSXGr
{
	public:
		CMSXGr() {};
		~CMSXGr() {};

        static bool IsImplemented() { return false; }

        int  Init() { return 0; }

        bool IsSlotEnable(int) { return false; }
		bool IsCartridgeInserted(int) { return false; }

		int  ReadMemory(int,char*,int,int) { return 0; }
		int  WriteMemory(int,char*,int,int) { return 0; }
		int  WriteIO(int,char*,int,int) { return 0; }
		int  ReadIO(int,char*,int,int) { return 0; }
};

#endif
