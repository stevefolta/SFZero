#ifndef SFZRegion_h
#define SFZRegion_h

class SFZSample;


// SFZRegion is designed to be able to be bitwise-copied.

struct SFZEGParameters {
	float	delay, start, attack, hold, decay, sustain, release;

	void	clear();
	void	clearMod();
	};

class SFZRegion {
	public:
		enum Trigger {
			attack, release, first, legato
			};
		enum LoopMode {
			sample_loop, no_loop, one_shot, loop_continuous, loop_sustain
			};


		SFZRegion();
		void	clear();
		void	dump();

		bool	matches(unsigned char note, unsigned char velocity, Trigger trigger) {
			return
				note >= lokey && note <= hikey &&
				velocity >= lovel && velocity <= hivel &&
				trigger == this->trigger;
			}

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
		float amp_veltrack;

		SFZEGParameters	ampeg, ampeg_veltrack;
	};


#endif 	// !SFZRegion_h

