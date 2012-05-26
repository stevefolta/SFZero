#ifndef SF2Reader_h
#define SF2Reader_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "SF2.h"

class SF2Sound;
class SFZRegion;
class SFZSample;


class SF2Reader {
	public:
		SF2Reader(SF2Sound* sound, const File& file);
		~SF2Reader();

		void	read();
		SFZSample*	readSamples(
			double sampleRate, double* progressVar = NULL, Thread* thread = NULL);

		double	getSampleRate() { return sampleRate; }

	protected:
		SF2Sound*	sound;
		FileInputStream*	file;
		double	sampleRate;

		void	addGeneratorToRegion(
			word genOper, SF2::genAmountType* amount, SFZRegion* region);
		float	timecents2Secs(short timecents);
	};

#endif 	// !SF2Reader_h

