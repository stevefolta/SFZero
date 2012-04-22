#include "SFZVoice.h"
#include "SFZSound.h"
#include "SFZRegion.h"
#include "SFZSample.h"
#include "SFZDebug.h"
#include <math.h>

static const float globalGain = -4.0;


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
	const float floatVelocity,
	SynthesiserSound* soundIn,
	const int currentPitchWheelPosition)
{
	SFZSound* sound = dynamic_cast<SFZSound*>(soundIn);
	if (sound == NULL) {
		killNote();
		return;
		}

	int velocity = (int) (floatVelocity * 127.0);
	region = sound->getRegionFor(midiNoteNumber, (int) (velocity * 127.0));
	if (region == NULL || region->sample == NULL || region->sample->getBuffer() == NULL) {
		killNote();
		return;
		}

	double targetFreq = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
	double naturalFreq = MidiMessage::getMidiNoteInHertz(region->pitch_keycenter);
	pitchRatio =
		(targetFreq * region->sample->getSampleRate()) /
		(naturalFreq * getSampleRate());

	double noteGainDB = globalGain + region->volume;
	double velocityGain = -20.0 * log10((127.0 * 127.0) / (velocity * velocity));
	velocityGain *= region->amp_veltrack * 100;
	noteGainDB += velocityGain;
	noteGainLeft = noteGainRight = Decibels::decibelsToGain(noteGainDB);
	sourceSamplePosition = 0.0;
	//***
}


void SFZVoice::stopNote(const bool allowTailOff)
{
	if (!allowTailOff) {
		killNote();
		return;
		}

	/***/
	killNote();
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
	if (region == NULL)
		return;

	AudioSampleBuffer* buffer = region->sample->getBuffer();
	const float* inL = buffer->getSampleData(0, 0);
	const float* inR =
		buffer->getNumChannels() > 1 ? buffer->getSampleData(1, 0) : NULL;
	float sourceLength = buffer->getNumSamples();

	float* outL = outputBuffer.getSampleData(0, startSample);
	float* outR =
		outputBuffer.getNumChannels() > 1 ?
		outputBuffer.getSampleData(1, startSample) : NULL;

	double sourceSamplePosition = this->sourceSamplePosition;

	while (--numSamples >= 0) {
		int pos = (int) sourceSamplePosition;
		float alpha = (float) (sourceSamplePosition - pos);
		float invAlpha = 1.0f - alpha;

		// Simple linear interpolation.
		float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
		float r = inR ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

		l *= noteGainLeft;
		r *= noteGainRight;

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


void SFZVoice::killNote()
{
	region = NULL;
	clearCurrentNote();
}



