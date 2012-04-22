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
	ampeg.setExponentialDecay(true);
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
	region = sound->getRegionFor(midiNoteNumber, velocity);
	if (region == NULL || region->sample == NULL || region->sample->getBuffer() == NULL) {
		killNote();
		return;
		}

	double note = midiNoteNumber;

	double sampleRate = getSampleRate();
	double targetFreq = noteHz(note);
	double naturalFreq = MidiMessage::getMidiNoteInHertz(region->pitch_keycenter);
	pitchRatio =
		(targetFreq * region->sample->getSampleRate()) /
		(naturalFreq * sampleRate);

	double noteGainDB = globalGain + region->volume;
	// Thanks to <http:://www.drealm.info/sfz/plj-sfz.xhtml> for explaining the
	// velocity curve in a way that I could understand, although they mean
	// "log10" when they say "log".
	double velocityGainDB = -20.0 * log10((127.0 * 127.0) / (velocity * velocity));
	velocityGainDB *= region->amp_veltrack / 100.0;
	noteGainDB += velocityGainDB;
	noteGainLeft = noteGainRight = Decibels::decibelsToGain(noteGainDB);
	sourceSamplePosition = 0.0;
	ampeg.startNote(&region->ampeg, floatVelocity, sampleRate, &region->ampeg_veltrack);
}


void SFZVoice::stopNote(const bool allowTailOff)
{
	if (!allowTailOff || region == NULL) {
		killNote();
		return;
		}

	if (region->loop_mode != SFZRegion::one_shot)
		ampeg.noteOff();
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
	float ampegGain = ampeg.level;
	float ampegSlope = ampeg.slope;
	long samplesUntilNextAmpSegment = ampeg.samplesUntilNextSegment;
	bool ampSegmentIsExponential = ampeg.segmentIsExponential;

	while (--numSamples >= 0) {
		int pos = (int) sourceSamplePosition;
		float alpha = (float) (sourceSamplePosition - pos);
		float invAlpha = 1.0f - alpha;

		// Simple linear interpolation.
		float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
		float r = inR ? (inR[pos] * invAlpha + inR[pos + 1] * alpha) : l;

		float gainLeft = noteGainLeft * ampegGain;
		float gainRight = noteGainRight * ampegGain;
		l *= gainLeft;
		r *= gainRight;
		// Shouldn't we dither here?

		if (outR) {
			*outL++ += l;
			*outR++ += r;
			}
		else
			*outL++ += (l + r) * 0.5f;

		sourceSamplePosition += pitchRatio;
		if (ampSegmentIsExponential)
			ampegGain *= ampegSlope;
		else
			ampegGain += ampegSlope;
		if (--samplesUntilNextAmpSegment < 0) {
			ampeg.level = ampegGain;
			ampeg.nextSegment();
			ampegGain = ampeg.level;
			ampegSlope = ampeg.slope;
			samplesUntilNextAmpSegment = ampeg.samplesUntilNextSegment;
			ampSegmentIsExponential = ampeg.segmentIsExponential;
			}

		if (sourceSamplePosition > sourceLength || ampeg.isDone()) {
			stopNote (false);
			break;
			}
		}

	this->sourceSamplePosition = sourceSamplePosition;
	ampeg.level = ampegGain;
	ampeg.samplesUntilNextSegment = samplesUntilNextAmpSegment;
}


void SFZVoice::killNote()
{
	region = NULL;
	clearCurrentNote();
}


double SFZVoice::noteHz(double note, const double freqOfA)
{
	// Like MidiMessage::getMidiNoteInHertz(), but with a float note.
	note -= 12 * 6 + 9;
	// Now 0 = A
	return freqOfA * pow(2.0, note / 12.0);
}



