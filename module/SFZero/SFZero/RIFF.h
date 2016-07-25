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
	int64  	start;

	void	ReadFrom(InputStream* file);
	void	Seek(InputStream* file);
	void	SeekAfter(InputStream* file);
	int64	End() { return start + size; }

	String	ReadString(InputStream* file);
	};


}

#endif 	// !RIFF_h

