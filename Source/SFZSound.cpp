#include "SFZSound.h"
#include "SFZRegion.h"
#include "SFZSample.h"
#include "SFZReader.h"


SFZSound::SFZSound(const File& fileIn)
	: file(fileIn)
{
	SFZReader reader(this);
	reader.read(file);
}


SFZSound::~SFZSound()
{
	int numRegions = regions.size();
	for (int i = 0; i < numRegions; ++i) {
		delete regions[i];
		regions.set(i, NULL);
		}

	for (HashMap<String,SFZSample*>::Iterator i(samples); i.next();)
		delete i.getValue();
}


bool SFZSound::appliesToNote(const int midiNoteNumber)
{
	// Just say yes; we can't truly know unless we're told the velocity as well.
	return true;
}


bool SFZSound::appliesToChannel(const int midiChannel)
{
	return true;
}


void SFZSound::addRegion(SFZRegion* region)
{
	regions.add(region);
}


SFZSample* SFZSound::addSample(String path)
{
	File sampleFile = file.getSiblingFile(path.replaceCharacter('\\', '/'));
	String samplePath = sampleFile.getFullPathName();
	SFZSample* sample = samples[samplePath];
	if (sample == NULL) {
		sample = new SFZSample(sampleFile);
		samples.set(samplePath, sample);
		}
	return sample;
}


void SFZSound::addError(const String& message)
{
	errors.add(message);
}


void SFZSound::addUnsupportedOpcode(const String& opcode)
{
	unusedOpcodes.set(opcode, opcode);
}


void SFZSound::dump()
{
	int i;

	int numErrors = errors.size();
	if (numErrors > 0) {
		printf("Errors:\n");
		for (i = 0; i < numErrors; ++i) {
			char message[256];
			errors[i].copyToUTF8(message, 256);
			printf("- %s\n", message);
			}
		printf("\n");
		}

	if (unusedOpcodes.size() > 0) {
		printf("Unused opcodes:\n");
		for (HashMap<String,String>::Iterator i(unusedOpcodes); i.next();) {
			char opcode[64];
			i.getKey().copyToUTF8(opcode, 64);
			printf("  %s\n", opcode);
			}
		printf("\n");
		}

	printf("Regions:\n");
	int numRegions = regions.size();
	for (i = 0; i < numRegions; ++i)
		regions[i]->dump();
	printf("\n");

	printf("Samples:\n");
	for (HashMap<String,SFZSample*>::Iterator i(samples); i.next();)
		i.getValue()->dump();
}



