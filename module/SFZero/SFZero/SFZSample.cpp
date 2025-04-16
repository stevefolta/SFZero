#include "SFZSample.h"
#include "SFZDebug.h"

namespace SFZero {

bool SFZSample::load(juce::AudioFormatManager* formatManager)
{
	juce::AudioFormatReader* reader = formatManager->createReaderFor(file);
	if (reader == NULL)
		return false;
	sampleRate = reader->sampleRate;
	sampleLength = (unsigned long)reader->lengthInSamples;
	// Read some extra samples, which will be filled with zeros, so interpolation
	// can be done without having to check for the edge all the time.
	buffer = new juce::AudioSampleBuffer(reader->numChannels, int (sampleLength + 4));
	reader->read(buffer, 0, int (sampleLength + 4), 0, true, true);
	juce::StringPairArray* metadata = &reader->metadataValues;
	int numLoops = metadata->getValue("NumSampleLoops", "0").getIntValue();
	if (numLoops > 0) {
		loopStart = (unsigned long)metadata->getValue("Loop0Start", "0").getLargeIntValue();
		loopEnd = (unsigned long)metadata->getValue("Loop0End", "0").getLargeIntValue();
		}
	delete reader;
	return true;
}


SFZSample::~SFZSample()
{
	delete buffer;
}


juce::String SFZSample::getShortName()
{
	return file.getFileName();
}


void SFZSample::setBuffer(juce::AudioSampleBuffer* newBuffer)
{
	buffer = newBuffer;
	sampleLength = buffer->getNumSamples();
}


juce::AudioSampleBuffer* SFZSample::detachBuffer()
{
	juce::AudioSampleBuffer* result = buffer;
	buffer = NULL;
	return result;
}


void SFZSample::dump()
{
	char path[256];
	file.getFullPathName().copyToUTF8(path, 256);
	printf("%s\n", path);
}


#ifdef JUCE_DEBUG
void SFZSample::checkIfZeroed(const char* where)
{
	if (buffer == NULL) {
		dbgprintf("SFZSample::checkIfZeroed(%s): no buffer!", where);
		return;
		}

	int samplesLeft = buffer->getNumSamples();
	unsigned long nonzero = 0, zero = 0;
	float* p = buffer->getWritePointer(0);
	for (; samplesLeft > 0; --samplesLeft) {
		if (*p++ == 0.0)
			zero += 1;
		else
			nonzero += 1;
		}
	if (nonzero > 0)
		dbgprintf("Buffer not zeroed at %s (%lu vs. %lu).", where, nonzero, zero);
	else
		dbgprintf("Buffer zeroed at %s!  (%lu zeros)", where, zero);
}
#endif 	// JUCE_DEBUG

}
