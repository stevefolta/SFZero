#include "SFZVoice.h"
#include "SFZSound.h"
#include "SFZRegion.h"
#include "SFZSample.h"


SFZVoice::SFZVoice()
	: region(NULL)
{
}


SFZVoice::~SFZVoice()
{
}


bool SFZVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<SFZSound*>(sound) != NULL;
}


void SFZVoice::startNote(
	const int midiNoteNumber,
	const float velocity,
	SynthesiserSound* soundIn,
	const int currentPitchWheelPosition)
{
	SFZSound* sound = dynamic_cast<SFZSound*>(soundIn);
	if (sound == NULL) {
		clearCurrentNote();
		return;
		}

	region = sound->getRegionFor(midiNoteNumber, (int) velocity * 127.0);
	if (region == NULL || region->sample == NULL || region->sample->getBuffer() == NULL) {
		clearCurrentNote();
		return;
		}

	double targetFreq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	double naturalFreq = MidiMessage::getMidiNoteInHertz(region->pitch_keycenter);
	pitchRatio = (targetFreq * region->sample->getSampleRate()) / (naturalFreq * getSampleRate());

	gainLeft = gainRight = region->volume;
	sourceSamplePosition = 0.0;
	//***
}


void SFZVoice::stopNote(const bool allowTailOff)
{
	if (!allowTailOff) {
		clearCurrentNote();
		return;
		}

	/***/
	clearCurrentNote();
}


void SFZVoice::pitchWheelMoved(const int newValue)
{
	/***/
}


void SFZVoice::controllerMoved(
	const int controllerNumber,
	const int newValue)
{
	/***/
}


void SFZVoice::renderNextBlock(
	AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
	AudioSampleBuffer* buffer = region->sample->getBuffer();
	const float* inL = buffer->getSampleData(0, 0);
	const float* inR =
		buffer->getNumChannels() > 1 ? buffer->getSampleData(1, 0) : NULL;
	float sourceLength = buffer->getNumSamples();

	float* outL = outputBuffer.getSampleData(0, startSample);
	float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getSampleData(1, startSample) : nullptr;

	float sourceSamplePosition = this->sourceSamplePosition;

	while (--numSamples > 0) {
		int pos = (int) sourceSamplePosition;
		float alpha = (float) (sourceSamplePosition - pos);
		float invAlpha = 1.0f - alpha;

		// Simple linear interpolation.
		float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
		float r = inR ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

		l *= gainLeft;
		r *= gainRight;

		if (outR) {
			*outL++ += l;
			*outR++ += r;
			}
		else
			*outL++ += (l + r) * 0.5f;

		sourceSamplePosition += pitchRatio;

		if (sourceSamplePosition > sourceLength) {
			stopNote (false);
			break;
			}
		}

	this->sourceSamplePosition = sourceSamplePosition;
}



