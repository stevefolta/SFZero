#ifndef RIFF_h
#define RIFF_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "WinTypes.h"


namespace SFZero {

struct RIFFChunk {
	enum Type {
		RIFF,
		LIST,
		Custom
		};

	fourcc	id;
	dword 	size;
	Type  	type;
	int64_t	start;

	void	ReadFrom(juce::InputStream* file);
	void	Seek(juce::InputStream* file);
	void	SeekAfter(juce::InputStream* file);
	int64_t	End() { return start + size; }

	juce::String	ReadString(juce::InputStream* file);
	};


}

#endif 	// !RIFF_h

