#ifndef __PLUGINPROCESSOR_H_7DD34D53__
#define __PLUGINPROCESSOR_H_7DD34D53__

#include "../JuceLibraryCode/JuceHeader.h"
#include "SFZSynth.h"

namespace SFZero {
	class SFZSound;
	}


class SFZeroAudioProcessor  : public AudioProcessor {
	public:
		SFZeroAudioProcessor();
		~SFZeroAudioProcessor();

		void prepareToPlay(double sampleRate, int samplesPerBlock);
		void releaseResources();
		void processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

		AudioProcessorEditor* createEditor();
		bool hasEditor() const;

		const String getName() const;

		int getNumParameters();

		float getParameter(int index);
		void setParameter(int index, float newValue);

		const String getParameterName(int index);
		const String getParameterText(int index);

		void	setSfzFile(File* newSfzFile);
		void	setSfzFileThreaded(File* newSfzFile);
		File	getSfzFile() { return sfzFile; }

		const String getInputChannelName(int channelIndex) const;
		const String getOutputChannelName(int channelIndex) const;
		bool isInputChannelStereoPair(int index) const;
		bool isOutputChannelStereoPair(int index) const;

		bool acceptsMidi() const;
		bool producesMidi() const;
		bool silenceInProducesSilenceOut() const;
		double getTailLengthSeconds() const;


		int getNumPrograms();
		int getCurrentProgram();
		void setCurrentProgram(int index);
		const String getProgramName(int index);
		void changeProgramName(int index, const String& newName);

		void getStateInformation(MemoryBlock& destData);
		void setStateInformation(const void* data, int sizeInBytes);

		MidiKeyboardState	keyboardState;
		double loadProgress;

		SFZero::SFZSound*	getSound();
		int      	numVoicesUsed();
		String   	voiceInfoString();

#if JUCE_DEBUG
		void	relayLogMessages();
#endif

	protected:
		class LoadThread : public Thread {
			public:
				LoadThread(SFZeroAudioProcessor* processor);
				void	run();

			protected:
				SFZeroAudioProcessor* processor;
			};
		friend class LoadThread;

		File sfzFile;
		SFZero::SFZSynth synth;
		AudioFormatManager formatManager;
		LoadThread	loadThread;

		void	loadSound(Thread* thread = NULL);

	private:
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SFZeroAudioProcessor);
	};


#endif  // __PLUGINPROCESSOR_H_7DD34D53__

