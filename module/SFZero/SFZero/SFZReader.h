#ifndef SFZReader_h
#define SFZReader_h

namespace SFZero {

class SFZRegion;
class SFZSound;


class SFZReader {
	public:
		SFZReader(SFZSound* sound);
		~SFZReader();

		void	read (const juce::File& file);
		void	read (const char* text, unsigned int length);

	protected:
		SFZSound*	sound;
		int	line;

		const char*	handleLineEnd (const char* p);
		const char*	readPathInto (juce::String* pathOut, const char* p, const char* end);
		int 	keyValue (const juce::String& str);
		int 	triggerValue (const juce::String& str);
		int 	loopModeValue (const juce::String& str);
		void	finishRegion (SFZRegion* region);
		void	error (const juce::String& message);
	};

}

#endif 	// !SFZReader_h

