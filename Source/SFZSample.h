#ifndef SFZSample_h
#define SFZSample_h

#include "../JuceLibraryCode/JuceHeader.h"


class SFZSample {
	public:
		SFZSample(const File& fileIn)
			: file(fileIn), buffer(NULL) {}
		~SFZSample();

		bool	load(AudioFormatManager* formatManager);
		File	getFile() { return file; }
		String	getShortName();
		void	dump();

	protected:
		File	file;
		AudioSampleBuffer*	buffer;
	};


#endif 	// !SFZSample_h

