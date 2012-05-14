#include "SFZeroAudioProcessor.h"
#include "SFZeroEditor.h"
#include "SFZSound.h"
#include "SFZVoice.h"
#include "SFZDebug.h"


SFZeroAudioProcessor::SFZeroAudioProcessor()
	: loadProgress(0.0), loadThread(this)
{
#if JUCE_DEBUG
	setupLogging(
		FileLogger::createDefaultAppLogger(
			"SFZero", "SFZero.log", "SFZero started"));
#endif

	formatManager.registerFormat(new WavAudioFormat(), false);
	formatManager.registerFormat(new OggVorbisAudioFormat(), false);

	for (int i = 0; i < 32; ++i)
		synth.addVoice(new SFZVoice());
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


void SFZeroAudioProcessor::setSfzFile(File* newSfzFile)
{
	sfzFile = *newSfzFile;
	loadSound();
}


void SFZeroAudioProcessor::setSfzFileThreaded(File* newSfzFile)
{
	loadThread.stopThread(2000);
	sfzFile = *newSfzFile;
	loadThread.startThread();
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
	DynamicObject state;
	state.setProperty("sfzFilePath", sfzFile.getFullPathName());
	MemoryOutputStream out(destData, false);
	JSON::writeToStream(out, &state);
}

void SFZeroAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	MemoryInputStream in(data, sizeInBytes, false);
	var state = JSON::parse(in);
	var pathVar = state["sfzFilePath"];
	if (pathVar.isString()) {
		String sfzFilePath = pathVar.toString();
		if (!sfzFilePath.isEmpty()) {
			File file(sfzFilePath);
			setSfzFile(&file);
			}
		}
}


SFZSound* SFZeroAudioProcessor::getSound()
{
	SynthesiserSound* sound = synth.getSound(0);
	return dynamic_cast<SFZSound*>(sound);
}


#if JUCE_DEBUG
void SFZeroAudioProcessor::relayLogMessages()
{
	relayFifoLogMessages();
}
#endif



void SFZeroAudioProcessor::loadSound(Thread* thread)
{
	loadProgress = 0.0;
	synth.clearSounds();

	if (!sfzFile.existsAsFile()) {
		//***
		return;
		}

	SFZSound* sound = new SFZSound(sfzFile);
	sound->loadSamples(&formatManager, &loadProgress, thread);

	synth.addSound(sound);
}


SFZeroAudioProcessor::LoadThread::LoadThread(SFZeroAudioProcessor* processorIn)
	: Thread("SFZLoad"), processor(processorIn)
{
}


void SFZeroAudioProcessor::LoadThread::run()
{
	processor->loadSound(this);
}




AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SFZeroAudioProcessor();
}

