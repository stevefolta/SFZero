#ifndef SF2Reader_h
#define SF2Reader_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "SF2.h"

namespace SFZero {

class SF2Sound;
class SFZRegion;
class SFZSample;


class SF2Reader {
	public:
		SF2Reader(SF2Sound* sound, const File& file);
		~SF2Reader();

		void	read();
		AudioSampleBuffer*	readSamples(
			double* progressVar = NULL, Thread* thread = NULL);

	protected:
		SF2Sound*	sound;
		FileInputStream*	file;

		void	addGeneratorToRegion(
			word genOper, SF2::genAmountType* amount, SFZRegion* region);
	};

}

#endif 	// !SF2Reader_h

