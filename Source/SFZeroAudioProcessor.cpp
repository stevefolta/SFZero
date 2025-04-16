#include "SFZeroAudioProcessor.h"
#include "SFZeroEditor.h"

SFZeroAudioProcessor::SFZeroAudioProcessor()
	: loadProgress(0.0), loadThread(this)
{
#if JUCE_DEBUG
	SFZero::setupLogging(
		 juce::FileLogger::createDefaultAppLogger(
			"SFZero", "SFZero.log", "SFZero started"));
#endif

	formatManager.registerFormat(new juce::WavAudioFormat(), false);
	formatManager.registerFormat(new juce::OggVorbisAudioFormat(), false);

	for (int i = 0; i < 128; ++i)
		synth.addVoice(new SFZero::SFZVoice());
}

SFZeroAudioProcessor::~SFZeroAudioProcessor()
{
}

const juce::String SFZeroAudioProcessor::getName() const
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

const juce::String SFZeroAudioProcessor::getParameterName(int index)
{
    return {};
}

const juce::String SFZeroAudioProcessor::getParameterText(int index)
{
	return {};
}


void SFZeroAudioProcessor::setSfzFile(const juce::File& newSfzFile)
{
	sfzFile = newSfzFile;
	loadSound();
}


void SFZeroAudioProcessor::setSfzFileThreaded(const juce::File& newSfzFile)
{
	loadThread.stopThread(2000);
	sfzFile = newSfzFile;
	loadThread.startThread();
}


const juce::String SFZeroAudioProcessor::getInputChannelName(int channelIndex) const
{
    return juce::String(channelIndex + 1);
}

const juce::String SFZeroAudioProcessor::getOutputChannelName(int channelIndex) const
{
    return juce::String(channelIndex + 1);
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

bool SFZeroAudioProcessor::silenceInProducesSilenceOut() const
{
	return true;
}


double SFZeroAudioProcessor::getTailLengthSeconds() const
{
  //*** What is correct here?  Should we iterate through all the voices to get
  // the longest current release time?
  return 0;
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

const juce::String SFZeroAudioProcessor::getProgramName(int index)
{
    return {};
}

void SFZeroAudioProcessor::changeProgramName(int index, const juce::String& newName)
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


void SFZeroAudioProcessor::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
	int numSamples = buffer.getNumSamples();
	keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);
	synth.renderNextBlock(buffer, midiMessages, 0, numSamples);
}


bool SFZeroAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SFZeroAudioProcessor::createEditor()
{
	return new SFZeroEditor(this);
}


void SFZeroAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// There's something weird about JUCE's DynamicObjects that doesn't allow
	// them to be used as stack-allocated variables.
	juce::DynamicObject::Ptr state = new juce::DynamicObject();
	state->setProperty("sfzFilePath", sfzFile.getFullPathName());
	auto sound = getSound();
	if (sound) {
		int subsound = sound->selectedSubsound();
		if (subsound != 0)
			state->setProperty("subsound", subsound);
		}

	juce::MemoryOutputStream out(destData, false);
	juce::JSON::writeToStream(out, juce::var(state));
}

void SFZeroAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	juce::MemoryInputStream in(data, sizeInBytes, false);
	juce::var state = juce::JSON::parse(in);
	juce::var pathVar = state["sfzFilePath"];
	if (pathVar.isString()) {
		juce::String sfzFilePath = pathVar.toString();
		if (!sfzFilePath.isEmpty()) {
			juce::File file(sfzFilePath);
			setSfzFile(file);
			auto sound = getSound();
			if (sound) {
				juce::var subsoundVar = state["subsound"];
				if (subsoundVar.isInt())
					sound->useSubsound(int(subsoundVar));
				}
			}
		}
}


SFZero::SFZSound* SFZeroAudioProcessor::getSound()
{
	auto sound = synth.getSound(0).get();
	return dynamic_cast<SFZero::SFZSound*>(sound);
}


int SFZeroAudioProcessor::numVoicesUsed()
{
	return synth.numVoicesUsed();
}


juce::String SFZeroAudioProcessor::voiceInfoString()
{
	return synth.voiceInfoString();
}


#if JUCE_DEBUG
void SFZeroAudioProcessor::relayLogMessages()
{
	SFZero::relayFifoLogMessages();
}
#endif



void SFZeroAudioProcessor::loadSound(juce::Thread* thread)
{
	loadProgress = 0.0;
	synth.clearSounds();

	if (!sfzFile.existsAsFile()) {
		//***
		return;
		}

	SFZero::SFZSound* sound;
	if (sfzFile.hasFileExtension (".sf2"))
		sound = new SFZero::SF2Sound(sfzFile);
	else
		sound = new SFZero::SFZSound(sfzFile);
	sound->loadRegions();
	sound->loadSamples(&formatManager, &loadProgress, thread);
	if (thread && thread->threadShouldExit()) {
		delete sound;
		return;
		}

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




juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SFZeroAudioProcessor();
}

