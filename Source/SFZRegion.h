#ifndef SFZRegion_h
#define SFZRegion_h

class SFZSample;


// SFZRegion is designed to be able to be bitwise-copied.

class SFZRegion {
	public:
		enum Trigger {
			attack, release, first, legato
			};
		enum LoopMode {
			no_loop, one_shot, loop_continuous, loop_sustain
			};


		SFZRegion();
		void	clear();

		SFZSample* sample;
		unsigned char lokey, hikey;
		unsigned char lovel, hivel;
		Trigger trigger;
		unsigned long group, off_by;

		unsigned long offset;
		unsigned long end;
		LoopMode loop_mode;
		unsigned long loop_start, loop_end;
		int transpose;
		int tune;
		int pitch_keycenter;

		float volume, pan;
	};


#endif 	// !SFZRegion_h

