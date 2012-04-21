#ifndef SFZSound_h
#define SFZSound_h

#include "../JuceLibraryCode/JuceHeader.h"

class SFZRegion;
class SFZSample;


class SFZSound : public SynthesiserSound {
	public:
		SFZSound(const File& file);
		~SFZSound();

		bool	appliesToNote(const int midiNoteNumber);
		bool	appliesToChannel(const int midiChannel);

		void	addRegion(SFZRegion* region); 	// Takes ownership of the region.
		SFZSample*	addSample(String path);
		void	addError(const String& message);
		void	addUnsupportedOpcode(const String& opcode);

		void	loadSamples(AudioFormatManager* formatManager, double* progressVar = NULL);

		String	getErrorsString();

		void	dump();

	protected:
		File 	file;
		Array<SFZRegion*>	regions;
		HashMap<String, SFZSample*>	samples;
		StringArray      	errors;
		HashMap<String, String>	unusedOpcodes;
	};



#endif 	// SFZSound_h

