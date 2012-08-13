#ifndef __PLUGINEDITOR_H_A8E24640__
#define __PLUGINEDITOR_H_A8E24640__

#include "../JuceLibraryCode/JuceHeader.h"
#include "SFZeroAudioProcessor.h"
#include "ClickableLabel.h"


class SFZeroEditor  :
	public AudioProcessorEditor, public Timer,
	public ClickableLabel::ClickListener
{
	public:
		SFZeroEditor(SFZeroAudioProcessor* ownerFilter);
		~SFZeroEditor();

		void	paint(Graphics& g);
		void	resized();
		void	labelClicked(Label* clickedLabel);
		void	timerCallback();

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
		MidiKeyboardComponent	midiKeyboard;
		ProgressBar*	progressBar;

		SFZeroAudioProcessor* getProcessor() const {
			return static_cast<SFZeroAudioProcessor*> (getAudioProcessor());
			}

		void	chooseFile();
		void	setFile(File* newFile);
		void	updateFile(File* file);
		void	showSoundInfo();
		void	showVoiceInfo();
		void	showVersion();
		void	showPath();
		void	showProgress();
		void	hideProgress();
		void	showSubsound();
	};


#endif  // __PLUGINEDITOR_H_A8E24640__
