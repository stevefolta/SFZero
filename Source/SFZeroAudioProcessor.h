#ifndef __PLUGINPROCESSOR_H_7DD34D53__
#define __PLUGINPROCESSOR_H_7DD34D53__

#include <JuceHeader.h>

namespace SFZero {
	class SFZSound;
	}


class SFZeroAudioProcessor  : public juce::AudioProcessor {
	public:
		SFZeroAudioProcessor();
		~SFZeroAudioProcessor();

		void prepareToPlay(double sampleRate, int samplesPerBlock) override;
		void releaseResources() override;
		void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;

		juce::AudioProcessorEditor* createEditor() override;
		bool hasEditor() const override;

		const juce::String getName() const override;

		int getNumParameters() override;

		float getParameter(int index) override;
		void setParameter(int index, float newValue) override;

		const juce::String getParameterName(int index) override;
		const juce::String getParameterText(int index) override;

		void		setSfzFile(const juce::File& newSfzFile);
		void		setSfzFileThreaded(const juce::File& newSfzFile);
		juce::File	getSfzFile() { return sfzFile; }

		const juce::String getInputChannelName(int channelIndex) const override;
		const juce::String getOutputChannelName(int channelIndex) const override;
		bool isInputChannelStereoPair(int index) const override;
		bool isOutputChannelStereoPair(int index) const override;

		bool acceptsMidi() const override;
		bool producesMidi() const override;
		bool silenceInProducesSilenceOut() const override;
		double getTailLengthSeconds() const override;


		int getNumPrograms() override;
		int getCurrentProgram() override;
		void setCurrentProgram(int index) override;
		const juce::String getProgramName(int index) override;
		void changeProgramName(int index, const juce::String& newName) override;

		void getStateInformation(juce::MemoryBlock& destData) override;
		void setStateInformation(const void* data, int sizeInBytes) override;

		juce::MidiKeyboardState	keyboardState;
		double loadProgress;

		SFZero::SFZSound*	getSound();
		int      			numVoicesUsed();
		juce::String   		voiceInfoString();

#if JUCE_DEBUG
		void	relayLogMessages();
#endif

	protected:
		class LoadThread : public juce::Thread {
			public:
				LoadThread(SFZeroAudioProcessor* processor);
				void	run();

			protected:
				SFZeroAudioProcessor* processor;
			};
		friend class LoadThread;

		juce::File sfzFile;
		SFZero::SFZSynth synth;
		juce::AudioFormatManager formatManager;
		LoadThread	loadThread;

		void	loadSound(juce::Thread* thread = NULL);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SFZeroAudioProcessor);
	};


#endif  // __PLUGINPROCESSOR_H_7DD34D53__

