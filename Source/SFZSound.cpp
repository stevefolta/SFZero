#include "SFZSound.h"
#include "SFZRegion.h"


SFZSound::SFZSound()
{
}


SFZSound::~SFZSound()
{
	int numRegions = regions.size();
	for (int i = 0; i < numRegions; ++i) {
		delete regions[i];
		regions.set(i, NULL);
		}
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

	printf("Errors:\n");
	int numErrors = errors.size();
	for (i = 0; i < numErrors; ++i) {
		char message[256];
		errors[i].copyToUTF8(message, 256);
		printf("- %s\n", message);
		}

	printf("Unused opcodes:\n");
	for (HashMap<String,String>::Iterator i(unusedOpcodes); i.next();) {
		char opcode[64];
		i.getKey().copyToUTF8(opcode, 64);
		printf("  %s\n");
		}

	printf("Regions:\n");
	int numRegions = regions.size();
	for (i = 0; i < numRegions; ++i)
		regions[i]->dump();
}



