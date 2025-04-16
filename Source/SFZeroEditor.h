#ifndef __PLUGINEDITOR_H_A8E24640__
#define __PLUGINEDITOR_H_A8E24640__

#include <JuceHeader.h>
#include "SFZeroAudioProcessor.h"
#include "ClickableLabel.h"


class SFZeroEditor  :
	public juce::AudioProcessorEditor, public juce::Timer,
	public ClickableLabel::ClickListener,
	public juce::FileDragAndDropTarget
{
	public:
		SFZeroEditor(SFZeroAudioProcessor* ownerFilter);
		~SFZeroEditor();

		void	paint(juce::Graphics& g) override;
		void	resized() override;
		void	labelClicked(juce::Label* clickedLabel) override;
		void	timerCallback() override;

		bool	isInterestedInFileDrag (const juce::StringArray& files) override;
		void	filesDropped (const juce::StringArray& files, int x, int y) override;

	protected:
		// pathLabel options.
		enum {
			showingVersion,
			showingPath,
			showingProgress,
			showingSubsound,
			};

		// infoLabel options.
		enum {
			showingSoundInfo,
			showingVoiceInfo,
			};

		ClickableLabel	fileLabel;
		ClickableLabel	pathLabel;
		ClickableLabel 	infoLabel;
		int	showing, showingInfo;
		juce::MidiKeyboardComponent	midiKeyboard;
		juce::ProgressBar*	progressBar;

		SFZeroAudioProcessor* getProcessor() const {
			return static_cast<SFZeroAudioProcessor*> (getAudioProcessor());
			}

		void	chooseFile();
		void	setFile(const juce::File& newFile);
		void	updateFile(const juce::File& file);
		void	showSoundInfo();
		void	showVoiceInfo();
		void	showVersion();
		void	showPath();
		void	showProgress();
		void	hideProgress();
		void	showSubsound();
	};


#endif  // __PLUGINEDITOR_H_A8E24640__
