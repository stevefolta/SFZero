#ifndef SFZReader_h
#define SFZReader_h

#include "../JuceLibraryCode/JuceHeader.h"

class SFZRegion;
class SFZSound;


class SFZReader {
	public:
		SFZReader(const char* text, unsigned int length);

	protected:
		const char*	p;
		const char*	end;
		SFZSound*	sound;
		StringArray*	errors;
	};


#endif 	// !SFZReader_h

