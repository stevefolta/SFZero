#include "SFZeroEditor.h"
#include "SFZeroAudioProcessor.h"
#include "SFZSound.h"
#include "SFZDebug.h"
#include <time.h>

using namespace SFZero;


enum {
	hMargin = 12,
	vMargin = 12,
	labelHeight = 25,
	progressBarHeight = 30,
	keyboardHeight = 54,
	};


SFZeroEditor::SFZeroEditor(SFZeroAudioProcessor* ownerFilter)
	: AudioProcessorEditor(ownerFilter),
		fileLabel(String::empty, "File... (click here to choose)"),
		pathLabel(String::empty),
		showingInfo(showingSoundInfo),
		midiKeyboard(ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard),
		progressBar(NULL)
{
	setSize(500, 300);

#ifdef JUCE_MAC
	Font fileFont("Helvetica", 22.0, Font::bold);
	Font labelFont("Helvetica", 15.0, Font::plain);
#else
	Font fileFont("Ariel", 22.0, Font::bold);
	Font labelFont("Ariel", 15.0, Font::plain);
#endif

	addAndMakeVisible(&fileLabel);
	fileLabel.setFont(fileFont);
	fileLabel.setColour(Label::textColourId, Colours::grey);
	fileLabel.addClickListener(this);

	addAndMakeVisible(&pathLabel);
	pathLabel.setFont(labelFont);
	pathLabel.setColour(Label::textColourId, Colours::grey);
	pathLabel.addClickListener(this);

	addAndMakeVisible(&infoLabel);
	infoLabel.setFont(labelFont);
	infoLabel.setJustificationType(Justification::topLeft);
	infoLabel.addClickListener(this);

	addAndMakeVisible(&midiKeyboard);
	midiKeyboard.setOctaveForMiddleC(4);

	startTimer(200);

	File sfzFile = ownerFilter->getSfzFile();
	if (sfzFile != File::nonexistent) {
		updateFile(&sfzFile);
		showSoundInfo();
		SFZSound* sound = ownerFilter->getSound();
		if (sound && sound->numSubsounds() > 1)
			showSubsound();
		}
	else
		showVersion();
}


SFZeroEditor::~SFZeroEditor()
{
	delete progressBar;
}


void SFZeroEditor::paint(Graphics& g)
{
	g.fillAll(Colours::white);
}


void SFZeroEditor::resized()
{
	int marginedWidth = getWidth() - 2 * hMargin;

	fileLabel.setBounds(
		hMargin, vMargin, marginedWidth, labelHeight);
	pathLabel.setBounds(
		hMargin, vMargin + labelHeight, marginedWidth, labelHeight);
	int infoTop = vMargin + 2 * labelHeight;
	int keyboardTop = getHeight() - keyboardHeight - vMargin;
	int infoLabelHeight = keyboardTop - infoTop - 4;
	infoLabel.setBounds(hMargin, infoTop, marginedWidth, infoLabelHeight);
	midiKeyboard.setBounds(hMargin, keyboardTop, marginedWidth, keyboardHeight);
}


void SFZeroEditor::labelClicked(Label* clickedLabel)
{
	if (clickedLabel == &fileLabel)
		chooseFile();

	else if (clickedLabel == &pathLabel) {
		if (showing == showingSubsound) {
			SFZeroAudioProcessor* processor = getProcessor();
			SFZSound* sound = processor->getSound();
			if (sound) {
				PopupMenu menu;
				int selectedSubsound = sound->selectedSubsound();
				int numSubsounds = sound->numSubsounds();
				for (int i = 0; i < numSubsounds; ++i) {
					menu.addItem(
						i + 1, sound->subsoundName(i), true, (i == selectedSubsound));
					}
				int result = menu.show();
				if (result != 0) {
					sound->useSubsound(result - 1);
					showSubsound();
					}
				}
			}
		else if (showing == showingVersion)
			showPath();
		else
			showVersion();
		}

	else if (clickedLabel == &infoLabel) {
		if (showingInfo == showingSoundInfo)
			showVoiceInfo();
		else
			showSoundInfo();
		}
}


void SFZeroEditor::timerCallback()
{
#ifdef JUCE_DEBUG
	getProcessor()->relayLogMessages();
#endif

	if (showing == showingProgress) {
		SFZeroAudioProcessor* processor = getProcessor();
		if (processor->loadProgress >= 1.0) {
			SFZSound* sound = processor->getSound();
			if (sound && sound->numSubsounds() > 1)
				showSubsound();
			else
				showPath();
			showSoundInfo();
			}
		}

	if (showingInfo == showingVoiceInfo)
		showVoiceInfo();
}


void SFZeroEditor::chooseFile()
{
	FileChooser chooser(
		"Select an SFZ file...",
		File::nonexistent,
		"*.sfz;*.SFZ;*.sf2;*.SF2");
	if (chooser.browseForFileToOpen()) {
		File sfzFile(chooser.getResult());
		setFile(&sfzFile);
		}
}


void SFZeroEditor::setFile(File* newFile)
{
	SFZeroAudioProcessor* processor = getProcessor();
	processor->setSfzFileThreaded(newFile);

	updateFile(newFile);
	showProgress();
}


void SFZeroEditor::updateFile(File* file)
{
	fileLabel.setText(file->getFileName(), dontSendNotification);
	fileLabel.setColour(Label::textColourId, Colours::black);
	showPath();
}


void SFZeroEditor::showSoundInfo()
{
	SFZeroAudioProcessor* processor = getProcessor();
	SFZSound* sound = processor->getSound();
	if (sound)
		infoLabel.setText(sound->getErrorsString(), dontSendNotification);
	showingInfo = showingSoundInfo;
}


void SFZeroEditor::showVoiceInfo()
{
	SFZeroAudioProcessor* processor = getProcessor();
	infoLabel.setText(processor->voiceInfoString(), dontSendNotification);
	showingInfo = showingVoiceInfo;
}


void SFZeroEditor::showVersion()
{
	struct tm tm;
	strptime(__DATE__, "%b %d %Y", &tm);
	char str[64];
	sprintf(str, "SFZero beta %d.%d.%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	pathLabel.setText(str, dontSendNotification);
	pathLabel.setColour(Label::textColourId, Colours::grey);
	hideProgress();
	showing = showingVersion;
}


void SFZeroEditor::showPath()
{
	SFZeroAudioProcessor* processor = getProcessor();
	File file = processor->getSfzFile();
	pathLabel.setText(
		file.getParentDirectory().getFullPathName(), dontSendNotification);
	pathLabel.setColour(Label::textColourId, Colours::grey);
	hideProgress();
	showing = showingPath;
}


void SFZeroEditor::showSubsound()
{
	SFZeroAudioProcessor* processor = getProcessor();
	SFZSound* sound = processor->getSound();
	if (sound == NULL)
		return;

	pathLabel.setText(
		sound->subsoundName(sound->selectedSubsound()), dontSendNotification);
	pathLabel.setColour(Label::textColourId, Colours::black);
	hideProgress();
	showing = showingSubsound;
}


void SFZeroEditor::showProgress()
{
	SFZeroAudioProcessor* processor = getProcessor();
	pathLabel.setVisible(false);
	infoLabel.setVisible(false);
	progressBar = new ProgressBar(processor->loadProgress);
	addAndMakeVisible(progressBar);
	int marginedWidth = getWidth() - 2 * hMargin;
	progressBar->setBounds(
		hMargin, vMargin + labelHeight, marginedWidth, progressBarHeight);
	showing = showingProgress;
}


void SFZeroEditor::hideProgress()
{
	if (progressBar == NULL)
		return;

	removeChildComponent(progressBar);
	delete progressBar;
	progressBar = NULL;

	pathLabel.setVisible(true);
	infoLabel.setVisible(true);
}



