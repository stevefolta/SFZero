#ifndef SFZSound_h
#define SFZSound_h

#include "../JuceLibraryCode/JuceHeader.h"

class SFZRegion;


class SFZSound : public SynthesiserSound {
	public:
		SFZSound();
		~SFZSound();

		bool	appliesToNote(const int midiNoteNumber);
		bool	appliesToChannel(const int midiChannel);

		void	addRegion(SFZRegion* region); 	// Takes ownership of the region.
		void	setErrors(StringArray* newErrors);

	protected:
		Array<SFZRegion*>	regions;
		StringArray*     	errors;
	};



#endif 	// SFZSound_h

