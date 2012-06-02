#ifndef SFZSynth_h
#define SFZSynth_h

#include "../JuceLibraryCode/JuceHeader.h"


class SFZSynth : public Synthesiser {
	public:
		SFZSynth();

		void	noteOn(
			const int midiChannel, const int midiNoteNumber, const float velocity);
		void	noteOff(
			const int midiChannel, const int midiNoteNumber,
			const bool allowTailOff);

		int	numVoicesUsed();

	protected:
		unsigned char	noteVelocities[128];
	};

#endif 	// !SFZSynth_h

