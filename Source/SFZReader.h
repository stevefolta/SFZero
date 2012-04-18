#ifndef SFZReader_h
#define SFZReader_h

#include "../JuceLibraryCode/JuceHeader.h"

class SFZRegion;
class SFZSound;


class SFZReader {
	public:
		SFZReader();
		~SFZReader();

		SFZSound*	read(const char* text, unsigned int length);

	protected:
		SFZSound*	sound;
		StringArray*	errors;
		int	line;

		const char*	handleLineEnd(char lineEndChar, const char* p);
		int 	keyValue(const String& str);
		int 	triggerValue(const String& str);
		int 	loopModeValue(const String& str);
		void	finishRegion(SFZRegion* region);
		void	error(const String& message);
	};


#endif 	// !SFZReader_h

