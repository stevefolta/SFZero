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
}


void SF2Sound::loadSamples(
	AudioFormatManager* formatManager,
	double* progressVar, Thread* thread)
{
	/***/
}


void SF2Sound::addPreset(SF2Sound::Preset* preset)
{
	presets.add(preset);
}


