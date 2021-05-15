// This file is taken from the openMSX project. 
// The file has been modified to be built in the blueMSX environment.

#ifndef __YM2413_HH__
#define __YM2413_HH__

#include <string>

using namespace std;


typedef unsigned long  UINT32;
typedef unsigned char  UINT8;

extern "C" {
#include "AudioMixer.h"
}


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

#ifndef OPENMSX_YM2413BASE
#define OPENMSX_YM2413BASE

class OpenYM2413Base : public SoundDevice
{
public:
    OpenYM2413Base() {}
    virtual ~OpenYM2413Base() {}
		
	virtual void reset(const UINT32 &time) = 0;
	virtual void writeReg(UINT8 r, UINT8 v, const UINT32 &time) = 0;
    virtual UINT8 peekReg(UINT8 r) = 0;
	
	virtual int* updateBuffer(int length) = 0;
	virtual void setSampleRate(int sampleRate, int Oversampling) = 0;

    virtual void loadState() = 0;
    virtual void saveState() = 0;
};

#endif

class Slot
{
	public:
		Slot();

		inline int volume_calc(UINT8 LFO_AM);
		inline void KEY_ON (UINT8 key_set);
		inline void KEY_OFF(UINT8 key_clr);

		UINT8 ar;	// attack rate: AR<<2
		UINT8 dr;	// decay rate:  DR<<2
		UINT8 rr;	// release rate:RR<<2
		UINT8 KSR;	// key scale rate
		UINT8 ksl;	// keyscale level
		UINT8 ksr;	// key scale rate: kcode>>KSR
		UINT8 mul;	// multiple: mul_tab[ML]

		// Phase Generator
		int phase;	// frequency counter
		int freq;	// frequency counter step
		UINT8 fb_shift;	// feedback shift value
		int op1_out[2];	// slot1 output for feedback

		// Envelope Generator
		UINT8 eg_type;	// percussive/nonpercussive mode
		UINT8 state;	// phase type
		int TL;		// total level: TL << 2
		int TLL;	// adjusted now TL
		int volume;	// envelope counter
		int sl;		// sustain level: sl_tab[SL]

		UINT8 eg_sh_dp;	// (dump state)
		UINT8 eg_sel_dp;	// (dump state)
		UINT8 eg_sh_ar;	// (attack state)
		UINT8 eg_sel_ar;	// (attack state)
		UINT8 eg_sh_dr;	// (decay state)
		UINT8 eg_sel_dr;	// (decay state)
		UINT8 eg_sh_rr;	// (release state for non-perc.)
		UINT8 eg_sel_rr;	// (release state for non-perc.)
		UINT8 eg_sh_rs;	// (release state for perc.mode)
		UINT8 eg_sel_rs;	// (release state for perc.mode)

		UINT8 key;	// 0 = KEY OFF, >0 = KEY ON

		// LFO
		UINT8 AMmask;	// LFO Amplitude Modulation enable mask
		UINT8 vib;	// LFO Phase Modulation enable flag (active high)

		int wavetable;	// waveform select
};

class Channel
{
	public:
		Channel();
		inline int chan_calc(UINT8 LFO_AM);
		inline void CALC_FCSLOT(Slot *slot);

		Slot slots[2];
		// phase generator state
		int block_fnum;	// block+fnum
		int fc;		// Freq. freqement base
		int ksl_base;	// KeyScaleLevel Base step
		UINT8 kcode;	// key code (for key scaling)
		UINT8 sus;	// sus on/off (release speed in percussive mode)
};

class OpenYM2413 : public OpenYM2413Base
{
	public:
		OpenYM2413(const string &name, short volume, const UINT32 &time);
		virtual ~OpenYM2413();
		
		virtual void reset(const UINT32 &time);
		virtual void writeReg(UINT8 r, UINT8 v, const UINT32 &time);
        virtual UINT8 peekReg(UINT8 r) { return regs[r]; }
		
		virtual void setInternalVolume(short newVolume);
		virtual int* updateBuffer(int length);
		virtual void setSampleRate(int sampleRate, int Oversampling);

        virtual void loadState();
        virtual void saveState();

	private:
        int filter(int input);
		void checkMute();
		bool checkMuteHelper();
		
		void init_tables();
		
		inline void advance_lfo();
		inline void advance();
		inline int rhythm_calc(bool noise);

		inline void set_mul(UINT8 slot, UINT8 v);
		inline void set_ksl_tl(UINT8 chan, UINT8 v);
		inline void set_ksl_wave_fb(UINT8 chan, UINT8 v);
		inline void set_ar_dr(UINT8 slot, UINT8 v);
		inline void set_sl_rr(UINT8 slot, UINT8 v);
		void load_instrument(UINT8 chan, UINT8 slot, UINT8* inst);
		void update_instrument_zero(UINT8 r);
		void setRhythmMode(bool newMode);

        int buffer[AUDIO_MONO_BUFFER_SIZE];
        int oplOversampling;

        int in[5];

        UINT8 regs[0x40];

		Channel channels[9];	// OPLL chips have 9 channels
		UINT8 instvol_r[9];		// instrument/volume (or volume/volume in percussive mode)

        short maxVolume;

		unsigned int eg_cnt;		// global envelope generator counter
		unsigned int eg_timer;		// global envelope generator counter works at frequency = chipclock/72
		unsigned int eg_timer_add;	    	// step of eg_timer

		bool rhythm;			// Rhythm mode

		// LFO
		unsigned int lfo_am_cnt;
		unsigned int lfo_am_inc;
		unsigned int lfo_pm_cnt;
		unsigned int lfo_pm_inc;

		int noise_rng;		// 23 bit noise shift register
		int noise_p;		// current noise 'phase'
		int noise_f;		// current noise period

		// instrument settings
		//   0     - user instrument
		//   1-15  - fixed instruments
		//   16    - bass drum settings
		//   17-18 - other percussion instruments
		UINT8 inst_tab[19][8];

		int fn_tab[1024];		// fnumber->increment counter

		UINT8 LFO_AM;
		UINT8 LFO_PM;
};

#endif

