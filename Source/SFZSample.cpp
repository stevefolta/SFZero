#include "SFZSample.h"


bool SFZSample::load(AudioFormatManager* formatManager)
{
	AudioFormatReader* reader = formatManager->createReaderFor(file);
	if (reader == NULL)
		return false;
	sampleRate = reader->sampleRate;
	// Read some extra samples, which will be filled with zeros, so interpolation
	// can be done without having to check for the edge all the time.
	buffer = new AudioSampleBuffer(reader->numChannels, reader->lengthInSamples + 4);
	reader->read(buffer, 0, reader->lengthInSamples + 4, 0, true, true);
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


void SFZSample::dump()
{
	char path[256];
	file.getFullPathName().copyToUTF8(path, 256);
	printf("%s\n", path);
}



