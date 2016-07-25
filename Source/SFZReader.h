#ifndef SFZReader_h
#define SFZReader_h

#include "../JuceLibraryCode/JuceHeader.h"

namespace SFZero {

class SFZRegion;
class SFZSound;


class SFZReader {
	public:
		SFZReader(SFZSound* sound);
		~SFZReader();

		void	read(const File& file);
		void	read(const char* text, unsigned int length);

	protected:
		SFZSound*	sound;
		int	line;

		const char*	handleLineEnd(const char* p);
		const char*	readPathInto(String* pathOut, const char* p, const char* end);
		int 	keyValue(const String& str);
		int 	triggerValue(const String& str);
		int 	loopModeValue(const String& str);
		void	finishRegion(SFZRegion* region);
		void	error(const String& message);
	};

}

#endif 	// !SFZReader_h

