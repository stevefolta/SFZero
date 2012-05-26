#include "SF2Sound.h"
#include "SF2Reader.h"


SF2Sound::SF2Sound(const File& file)
	: SFZSound(file)
{
}


SF2Sound::~SF2Sound()
{
	// "presets" owns the regions, so clear them out of "regions" so ~SFZSound()
	// doesn't try to delete them.
	regions.clear();
}


void SF2Sound::loadRegions()
{
	SF2Reader reader(this, file);
	reader.read();
	useSubsound(0);
}


void SF2Sound::loadSamples(
	AudioFormatManager* formatManager,
	double* progressVar, Thread* thread)
{
	SF2Reader reader(this, file);
	SFZSample* sample = reader.readSamples(progressVar, thread);
	if (sample)
		samples.set(String::empty, sample);
}


void SF2Sound::addPreset(SF2Sound::Preset* preset)
{
	presets.add(preset);
}


int SF2Sound::numSubsounds()
{
	return presets.size();
}


String SF2Sound::subsoundName(int whichSubsound)
{
	return presets[whichSubsound]->name;
}


void SF2Sound::useSubsound(int whichSubsound)
{
	selectedPreset = whichSubsound;
}


int SF2Sound::selectedSubsound()
{
	return selectedPreset;
}



