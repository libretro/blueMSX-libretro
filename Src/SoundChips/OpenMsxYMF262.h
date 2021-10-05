// This file is taken from the openMSX project. 
// The file has been modified to be built in the blueMSX environment.

#ifndef __YMF262_HH__
#define __YMF262_HH__

#include <string>
extern "C" {
#include "Board.h"
}

using namespace std;


typedef unsigned long  UINT32;
typedef unsigned char  UINT8;

extern "C" {
#include "AudioMixer.h"
}


class TimerCallback
{
	public:
		virtual void callback(UINT8 value) = 0;
};

extern void moonsoundTimerSet(void* ref, int timer, int count);
extern void moonsoundTimerStart(void* ref, int timer, int start, UINT8 timerRef);

template<int freq, UINT8 flag>
class Timer
{
	public:
        Timer(TimerCallback *cb, void* reference) {
            ref = reference;
            id = 12500 / freq;
        }
		virtual ~Timer() {}
		void setValue(UINT8 value) {
            moonsoundTimerSet(ref, id, id * (256 - value));
        }
		void setStart(bool start, const UINT32 &time) {
            moonsoundTimerStart(ref, id, start, flag);
        }

	private:
        void* ref;
        int id;
};


class IRQHelper 
{
public:
    IRQHelper() {}
    static void set() {
        boardSetInt(0x8);
    }
    static void reset() {
        boardClearInt(0x8);
    }
};

#ifndef OPENMSX_SOUNDDEVICE
#define OPENMSX_SOUNDDEVICE

class SoundDevice
{
	public:
        SoundDevice() : internalMuted(true) {}
		void setVolume(short newVolume) {
	        setInternalVolume(newVolume);
        }

	protected:
		virtual void setInternalVolume(short newVolume) = 0;
        void setInternalMute(bool muted) { internalMuted = muted; }
        bool isInternalMuted() const { return internalMuted; }
	public:
		virtual void setSampleRate(int newSampleRate, int Oversampling) = 0;
		virtual int* updateBuffer(int length) = 0;

	private:
		bool internalMuted;
};

#endif

class YMF262Slot
{
	public:
		YMF262Slot();
		inline int volume_calc(UINT8 LFO_AM);
		inline void FM_KEYON(UINT8 key_set);
		inline void FM_KEYOFF(UINT8 key_clr);

		UINT8 ar;	// attack rate: AR<<2
		UINT8 dr;	// decay rate:  DR<<2
		UINT8 rr;	// release rate:RR<<2
		UINT8 KSR;	// key scale rate
		UINT8 ksl;	// keyscale level
		UINT8 ksr;	// key scale rate: kcode>>KSR
		UINT8 mul;	// multiple: mul_tab[ML]

		// Phase Generator 
		unsigned int Cnt;	// frequency counter
		unsigned int Incr;	// frequency counter step
		UINT8 FB;	// feedback shift value
		int op1_out[2];	// slot1 output for feedback
		UINT8 CON;	// connection (algorithm) type

		// Envelope Generator 
		UINT8 eg_type;	// percussive/non-percussive mode 
		UINT8 state;	// phase type
		unsigned int TL;	// total level: TL << 2
		int TLL;	// adjusted now TL
		int volume;	// envelope counter
		int sl;		// sustain level: sl_tab[SL]

		unsigned int eg_m_ar;// (attack state)
		UINT8 eg_sh_ar;	// (attack state)
		UINT8 eg_sel_ar;	// (attack state)
		unsigned int eg_m_dr;// (decay state)
		UINT8 eg_sh_dr;	// (decay state)
		UINT8 eg_sel_dr;	// (decay state)
		unsigned int eg_m_rr;// (release state)
		UINT8 eg_sh_rr;	// (release state)
		UINT8 eg_sel_rr;	// (release state)

		UINT8 key;	// 0 = KEY OFF, >0 = KEY ON

		// LFO 
		UINT8  AMmask;	// LFO Amplitude Modulation enable mask 
		UINT8 vib;	// LFO Phase Modulation enable flag (active high)

		// waveform select 
		UINT8 waveform_number;
		unsigned int wavetable;

		int connect;	// slot output pointer
};

class YMF262Channel
{
	public:
		YMF262Channel();
		void chan_calc(UINT8 LFO_AM);
		void chan_calc_ext(UINT8 LFO_AM);
		void CALC_FCSLOT(YMF262Slot &slot);

		YMF262Slot slots[2];

		int block_fnum;	// block+fnum
		int fc;		// Freq. Increment base
		int ksl_base;	// KeyScaleLevel Base step
		UINT8 kcode;	// key code (for key scaling)

		// there are 12 2-operator channels which can be combined in pairs
		// to form six 4-operator channel, they are:
		//  0 and 3,
		//  1 and 4,
		//  2 and 5,
		//  9 and 12,
		//  10 and 13,
		//  11 and 14
		UINT8 extended;	// set to 1 if this channel forms up a 4op channel with another channel(only used by first of pair of channels, ie 0,1,2 and 9,10,11) 
};

// Bitmask for register 0x04 
static const int R04_ST1          = 0x01;	// Timer1 Start
static const int R04_ST2          = 0x02;	// Timer2 Start
static const int R04_MASK_T2      = 0x20;	// Mask Timer2 flag 
static const int R04_MASK_T1      = 0x40;	// Mask Timer1 flag 
static const int R04_IRQ_RESET    = 0x80;	// IRQ RESET 

// Bitmask for status register 
static const int STATUS_T2      = R04_MASK_T2;
static const int STATUS_T1      = R04_MASK_T1;

class YMF262 : public SoundDevice, public TimerCallback
{
	public:
		YMF262(short volume, const UINT32 &time, void* ref);
		virtual ~YMF262();
		
		virtual void reset(const UINT32 &time);
		void writeReg(int r, UINT8 v, const UINT32 &time);
		UINT8 peekReg(int reg);
		UINT8 readReg(int reg);
		UINT8 peekStatus();
		UINT8 readStatus();
		
		virtual void setInternalVolume(short volume);
		virtual void setSampleRate(int sampleRate, int Oversampling);
		virtual int* updateBuffer(int length);

		void callback(UINT8 flag);

        void loadState();
        void saveState();

	private:
		void writeRegForce(int r, UINT8 v, const UINT32 &time);
		void init_tables(void);
		void setStatus(UINT8 flag);
		void resetStatus(UINT8 flag);
		void changeStatusMask(UINT8 flag);
		void advance_lfo();
		void advance();
		void chan_calc_rhythm(bool noise);
		void set_mul(UINT8 sl, UINT8 v);
		void set_ksl_tl(UINT8 sl, UINT8 v);
		void set_ar_dr(UINT8 sl, UINT8 v);
		void set_sl_rr(UINT8 sl, UINT8 v);
		void update_channels(YMF262Channel &ch);
		void checkMute();
		bool checkMuteHelper();

        int buffer[AUDIO_MONO_BUFFER_SIZE];
		IRQHelper irq;
		Timer<12500, STATUS_T1> timer1;	//  80us
		Timer< 3125, STATUS_T2> timer2;	// 320us

        int oplOversampling;

        YMF262Channel channels[18];	// OPL3 chips have 18 channels

		UINT8 reg[512];

        unsigned int pan[18*4];		// channels output masks (0xffffffff = enable); 4 masks per one channel 

		unsigned int eg_cnt;		// global envelope generator counter
		unsigned int eg_timer;		// global envelope generator counter works at frequency = chipclock/288 (288=8*36) 
		unsigned int eg_timer_add;		// step of eg_timer

		unsigned int fn_tab[1024];		// fnumber->increment counter

		// LFO 
		UINT8 LFO_AM;
		UINT8 LFO_PM;
		
		UINT8 lfo_am_depth;
		UINT8 lfo_pm_depth_range;
		unsigned int lfo_am_cnt;
		unsigned int lfo_am_inc;
		unsigned int lfo_pm_cnt;
		unsigned int lfo_pm_inc;

		unsigned int noise_rng;		// 23 bit noise shift register
		unsigned int noise_p;		// current noise 'phase'
		unsigned int noise_f;		// current noise period

		bool OPL3_mode;			// OPL3 extension enable flag
		UINT8 rhythm;			// Rhythm mode
		UINT8 nts;			// NTS (note select)

		UINT8 status;			// status flag
		UINT8 status2;
		UINT8 statusMask;		// status mask

		int chanout[20];		// 18 channels + two phase modulation
		short maxVolume;
};

#endif

