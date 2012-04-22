#include "SFZSynth.h"
#include "SFZVoice.h"
#include "SFZSound.h"
#include "SFZDebug.h"


SFZSynth::SFZSynth()
	: Synthesiser()
{
}


void SFZSynth::noteOn(
	const int midiChannel, const int midiNoteNumber, const float velocity)
{
	int i;

	const ScopedLock locker(lock);

	// First, stop any currently-playing sounds in the group.
	int group = 0;
	SFZSound* sound = dynamic_cast<SFZSound*>(getSound(0));
	if (sound) {
		SFZRegion* region = sound->getRegionFor(midiNoteNumber, velocity);
		if (region)
			group = region->group;
		}
	if (group != 0) {
		for (i = voices.size(); --i >= 0;) {
			SFZVoice* voice = dynamic_cast<SFZVoice*>(voices.getUnchecked(i));
			if (voice == NULL)
				continue;
			if (voice->getOffBy() == group)
				voice->stopNoteForGroup();
			}
		}

	Synthesiser::noteOn(midiChannel, midiNoteNumber, velocity);
}



