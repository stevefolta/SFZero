#include "SFZSample.h"
#include "SFZDebug.h"

using namespace SFZero;


bool SFZSample::load(AudioFormatManager* formatManager)
{
	AudioFormatReader* reader = formatManager->createReaderFor(file);
	if (reader == NULL)
		return false;
	sampleRate = reader->sampleRate;
	sampleLength = reader->lengthInSamples;
	// Read some extra samples, which will be filled with zeros, so interpolation
	// can be done without having to check for the edge all the time.
	buffer = new AudioSampleBuffer(reader->numChannels, sampleLength + 4);
	reader->read(buffer, 0, sampleLength + 4, 0, true, true);
	StringPairArray* metadata = &reader->metadataValues;
	int numLoops = metadata->getValue("NumSampleLoops", "0").getIntValue();
	if (numLoops > 0) {
		loopStart = metadata->getValue("Loop0Start", "0").getLargeIntValue();
		loopEnd = metadata->getValue("Loop0End", "0").getLargeIntValue();
		}
	delete reader;
	return true;
}


SFZSample::~SFZSample()
{
	delete buffer;
}


String SFZSample::getShortName()
{
	return file.getFileName();
}


void SFZSample::setBuffer(AudioSampleBuffer* newBuffer)
{
	buffer = newBuffer;
	sampleLength = buffer->getNumSamples();
}


AudioSampleBuffer* SFZSample::detachBuffer()
{
	AudioSampleBuffer* result = buffer;
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



