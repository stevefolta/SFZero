#ifndef SFZSample_h
#define SFZSample_h

#include "../JuceLibraryCode/JuceHeader.h"


class SFZSample {
	public:
		SFZSample(const File& fileIn)
			: file(fileIn), buffer(NULL), loopStart(0), loopEnd(0) {}
		~SFZSample();

		bool	load(AudioFormatManager* formatManager);
		File	getFile() { return file; }
		AudioSampleBuffer*	getBuffer() { return buffer; }
		double	getSampleRate() { return sampleRate; }
		String	getShortName();
		void	dump();

		unsigned long	loopStart, loopEnd;

	protected:
		File	file;
		AudioSampleBuffer*	buffer;
		double	sampleRate;
	};


#endif 	// !SFZSample_h

