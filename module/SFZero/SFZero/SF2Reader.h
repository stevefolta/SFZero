#ifndef SF2Reader_h
#define SF2Reader_h

#include "SF2.h"

namespace SFZero {

class SF2Sound;
class SFZRegion;
class SFZSample;


class SF2Reader
{
public:
	SF2Reader (SF2Sound* sound, const juce::File& file);
	~SF2Reader ();

	void	read();
	juce::AudioSampleBuffer* readSamples (double* progressVar = NULL, juce::Thread* thread = NULL);

protected:
	SF2Sound*	sound;
	std::unique_ptr<juce::FileInputStream>	file;

	void	addGeneratorToRegion (word genOper, SF2::genAmountType* amount, SFZRegion* region);
};

}

#endif 	// !SF2Reader_h

