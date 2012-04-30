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

	int midiVelocity = (int) (velocity * 127);

	// First, stop any currently-playing sounds in the group.
	int group = 0;
	SFZSound* sound = dynamic_cast<SFZSound*>(getSound(0));
	if (sound) {
		SFZRegion* region = sound->getRegionFor(midiNoteNumber, midiVelocity);
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

	noteVelocities[midiNoteNumber] = midiVelocity;
}


void SFZSynth::noteOff(
	const int midiChannel, const int midiNoteNumber,
	const bool allowTailOff)
{
	const ScopedLock locker(lock);

	Synthesiser::noteOff(midiChannel, midiNoteNumber, allowTailOff);

	// Start release region.
	SFZSound* sound = dynamic_cast<SFZSound*>(getSound(0));
	if (sound) {
		SFZRegion* region =
			sound->getRegionFor(
				midiNoteNumber, noteVelocities[midiNoteNumber], SFZRegion::release);
		if (region) {
			SFZVoice* voice = dynamic_cast<SFZVoice*>(findFreeVoice(sound, false));
			if (voice) {
				// Synthesiser is too locked-down (ivars are private rt protected), so
				// we have to use a "setTrigger()" mechanism.
				voice->setTrigger(SFZRegion::release);
				startVoice(
					voice, sound,
					midiChannel, midiNoteNumber,
					noteVelocities[midiNoteNumber] / 127.0);
				}
			}
		}
}



