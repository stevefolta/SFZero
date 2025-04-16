#ifndef SFZSample_h
#define SFZSample_h

namespace SFZero {

class SFZSample
{
public:
	SFZSample(const juce::File& fileIn)
		: loopStart(0), loopEnd(0), file(fileIn), buffer(NULL) {}
	SFZSample(double sampleRateIn)
		: sampleLength(0), loopStart(0), loopEnd(0),
		  buffer(NULL), sampleRate(sampleRateIn) {}
	~SFZSample();

	bool	load(juce::AudioFormatManager* formatManager);
	juce::File	getFile() { return file; }
	juce::AudioSampleBuffer*	getBuffer() { return buffer; }
	double	getSampleRate() { return sampleRate; }
	juce::String	getShortName();
	void setBuffer(juce::AudioSampleBuffer* newBuffer);
	juce::AudioSampleBuffer* detachBuffer();
	void dump();

	unsigned long	sampleLength, loopStart, loopEnd;

#ifdef JUCE_DEBUG
		void	checkIfZeroed(const char* where);
#endif

protected:
	juce::File	file;
	juce::AudioSampleBuffer*	buffer;
	double	sampleRate;
};

}

#endif 	// !SFZSample_h

