#include "SFZSample.h"


bool SFZSample::load(AudioFormatManager* formatManager)
{
	AudioFormatReader* reader = formatManager->createReaderFor(file);
	if (reader == NULL)
		return false;
	sampleRate = reader->sampleRate;
	buffer = new AudioSampleBuffer(reader->numChannels, reader->lengthInSamples);
	reader->read(buffer, 0, reader->lengthInSamples, 0, true, true);
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



