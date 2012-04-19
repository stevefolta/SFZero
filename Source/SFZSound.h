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
		void	addError(const String& message);
		void	addUnsupportedOpcode(const String& opcode);

		void	dump();

	protected:
		Array<SFZRegion*>	regions;
		StringArray      	errors;
		HashMap<String, String>	unusedOpcodes;
	};



#endif 	// SFZSound_h

