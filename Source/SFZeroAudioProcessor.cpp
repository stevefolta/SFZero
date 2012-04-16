/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "SFZeroAudioProcessor.h"
#include "SFZeroEditor.h"


//==============================================================================
SFZeroAudioProcessor::SFZeroAudioProcessor()
{
}

SFZeroAudioProcessor::~SFZeroAudioProcessor()
{
}

//==============================================================================
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

//==============================================================================
void SFZeroAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SFZeroAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SFZeroAudioProcessor::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getSampleData(channel);

        // ..do something to the data...
    }

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SFZeroAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SFZeroAudioProcessor::createEditor()
{
    return new SFZeroEditor(this);
}

//==============================================================================
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

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SFZeroAudioProcessor();
}
