// $Id: OpenMsxY8950Adpcm.h,v 1.2 2006-06-17 21:42:32 vincent_van_dam Exp $

#ifndef __Y8950ADPCM_HH__
#define __Y8950ADPCM_HH__

#include "MsxTypes.h"
#include <string>

using namespace std;


typedef unsigned long  UINT32;
typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned __int64 UINT64;
class Y8950;

class Y8950Adpcm
{
public:
	Y8950Adpcm(Y8950& y8950, const string& name, int sampleRam);
	virtual ~Y8950Adpcm();
	
	void reset(const UINT32& time);
	void setSampleRate(int sr);
	bool muted();
	void writeReg(UINT8 rg, UINT8 data, const UINT32& time);
	UINT8 readReg(UINT8 rg);
	int calcSample();
    
    void pushTime(const UINT32 &time);

    void loadState();
    void saveState();

private:
	// Debuggable
	virtual unsigned getSize() const;
	virtual const string& getDescription() const;
	virtual UINT8 read(unsigned address);
	virtual void write(unsigned address, UINT8 value);
	
	// Schedulable
	virtual void executeUntil(const UINT32& time, int userData);
	virtual const string& schedName() const;

	void schedule(const UINT32& time);
    void unschedule(const UINT32 &time);
	int CLAP(int min, int x, int max);
	void restart();

	Y8950& y8950;
	const string name;

	int sampleRate;
	
	int ramSize;
	int startAddr;
	int stopAddr;
	int playAddr;
	int addrMask;
	int memPntr;
	bool romBank;
	UINT8* ramBank;
	
	bool playing;
	int volume;
	UINT16 delta;
	unsigned int nowStep, step;
	int out, output;
	int diff;
	int nextLeveling;
	int sampleStep;
	int volumeWStep;
        
    UINT32 oldTime;
    UINT64  sysTime;
    UINT64  syncTime;
	
	UINT8 reg7;
	UINT8 reg15;
};


#endif 
