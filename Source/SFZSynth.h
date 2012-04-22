#ifndef SFZSynth_h
#define SFZSynth_h

#include "../JuceLibraryCode/JuceHeader.h"


class SFZSynth : public Synthesiser {
	public:
		SFZSynth();

		void	noteOn(
			const int midiChannel, const int midiNoteNumber, const float velocity);

	protected:
	};

#endif 	// !SFZSynth_h

