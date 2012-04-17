#include "SFZSound.h"
#include "SFZRegion.h"


SFZSound::SFZSound()
	: errors(NULL)
{
}


SFZSound::~SFZSound()
{
	int numRegions = regions.size();
	for (int i = 0; i < numRegions; ++i) {
		delete regions[i];
		regions.set(i, NULL);
		}
	delete errors;
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


void SFZSound::setErrors(StringArray* newErrors)
{
	delete errors;
	errors = newErrors;
}


