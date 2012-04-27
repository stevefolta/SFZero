#ifndef SFZSample_h
#define SFZSample_h

#include "../JuceLibraryCode/JuceHeader.h"


class SFZSample {
	public:
		SFZSample(const File& fileIn)
			: loopStart(0), loopEnd(0), file(fileIn), buffer(NULL) {}
		~SFZSample();

		bool	load(AudioFormatManager* formatManager);
		File	getFile() { return file; }
		AudioSampleBuffer*	getBuffer() { return buffer; }
		double	getSampleRate() { return sampleRate; }
		String	getShortName();
		void	dump();

		unsigned long	sampleLength, loopStart, loopEnd;

	protected:
		File	file;
		AudioSampleBuffer*	buffer;
		double	sampleRate;
	};


#endif 	// !SFZSample_h

