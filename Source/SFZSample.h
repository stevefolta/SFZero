#ifndef SFZSample_h
#define SFZSample_h

#include "../JuceLibraryCode/JuceHeader.h"


namespace SFZero {

class SFZSample {
	public:
		SFZSample(const File& fileIn)
			: loopStart(0), loopEnd(0), file(fileIn), buffer(NULL) {}
		SFZSample(double sampleRateIn)
			: sampleLength(0), loopStart(0), loopEnd(0),
			  buffer(NULL), sampleRate(sampleRateIn) {}
		~SFZSample();

		bool	load(AudioFormatManager* formatManager);
		File	getFile() { return file; }
		AudioSampleBuffer*	getBuffer() { return buffer; }
		double	getSampleRate() { return sampleRate; }
		String	getShortName();
		void	setBuffer(AudioSampleBuffer* newBuffer);
		AudioSampleBuffer*	detachBuffer();
		void	dump();

		unsigned long	sampleLength, loopStart, loopEnd;

#ifdef JUCE_DEBUG
		void	checkIfZeroed(const char* where);
#endif

	protected:
		File	file;
		AudioSampleBuffer*	buffer;
		double	sampleRate;
	};

}

#endif 	// !SFZSample_h

