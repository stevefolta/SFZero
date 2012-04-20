#include "SFZeroAudioProcessor.h"
#include "SFZeroEditor.h"
#include "SFZSound.h"


SFZeroAudioProcessor::SFZeroAudioProcessor()
{
}

SFZeroAudioProcessor::~SFZeroAudioProcessor()
{
}

const String SFZeroAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SFZeroAudioProcessor::getNumParameters()
{
    return 0;
}

float SFZeroAudioProcessor::getParameter(int index)
{
    return 0.0f;
}

void SFZeroAudioProcessor::setParameter(int index, float newValue)
{
}

const String SFZeroAudioProcessor::getParameterName(int index)
{
    return String::empty;
}

const String SFZeroAudioProcessor::getParameterText(int index)
{
    return String::empty;
}


void SFZeroAudioProcessor::setSfzFile(File* newSfzFile, double* progressVar)
{
	sfzFile = *newSfzFile;
	loadSound(progressVar);
}


const String SFZeroAudioProcessor::getInputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

const String SFZeroAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return String(channelIndex + 1);
}

bool SFZeroAudioProcessor::isInputChannelStereoPair(int index) const
{
    return true;
}

bool SFZeroAudioProcessor::isOutputChannelStereoPair(int index) const
{
    return true;
}

bool SFZeroAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool SFZeroAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int SFZeroAudioProcessor::getNumPrograms()
{
    return 0;
}

int SFZeroAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SFZeroAudioProcessor::setCurrentProgram(int index)
{
}

const String SFZeroAudioProcessor::getProgramName(int index)
{
    return String::empty;
}

void SFZeroAudioProcessor::changeProgramName(int index, const String& newName)
{
}


void SFZeroAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
	keyboardState.reset();
}

void SFZeroAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
	keyboardState.reset();
}


void SFZeroAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	int numSamples = buffer.getNumSamples();
	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
	synth.renderNextBlock(buffer, midiMessages, 0, numSamples);
}


bool SFZeroAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SFZeroAudioProcessor::createEditor()
{
    return new SFZeroEditor(this);
}


void SFZeroAudioProcessor::getStateInformation(MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SFZeroAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


SFZSound* SFZeroAudioProcessor::getSound()
{
	SynthesiserSound* sound = synth.getSound(0);
	return dynamic_cast<SFZSound*>(sound);
}


void SFZeroAudioProcessor::loadSound(double* progressVar)
{
	synth.clearSounds();

	if (!sfzFile.existsAsFile()) {
		//***
		return;
		}

	SFZSound* sound = new SFZSound(sfzFile);
	sound->loadSamples(progressVar);

	synth.addSound(sound);
}




AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SFZeroAudioProcessor();
}

