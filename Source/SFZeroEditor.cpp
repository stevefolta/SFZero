#include "SFZeroEditor.h"
#include "SFZeroAudioProcessor.h"
#include "SFZSound.h"
#include "SFZDebug.h"
#include <time.h>

enum {
	hMargin = 12,
	vMargin = 12,
	buttonHeight = 25,
	labelHeight = 25,
	progressBarHeight = 40,
	keyboardHeight = 70,
	};


SFZeroEditor::SFZeroEditor(SFZeroAudioProcessor* ownerFilter)
	: AudioProcessorEditor(ownerFilter),
		fileLabel(String::empty, "File... (click here to choose)"),
		pathLabel(String::empty),
		midiKeyboard(ownerFilter->keyboardState, MidiKeyboardComponent::horizontalKeyboard)
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

	addAndMakeVisible(&midiKeyboard);
	midiKeyboard.setOctaveForMiddleC(4);

	startTimer(200);

	File sfzFile = ownerFilter->getSfzFile();
	if (sfzFile != File::nonexistent)
		updateFile(&sfzFile);
	else
		showVersion();
	SFZSound* sound = ownerFilter->getSound();
	if (sound)
		infoLabel.setText(sound->getErrorsString(), false);
}


SFZeroEditor::~SFZeroEditor()
{
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
		hMargin, vMargin + buttonHeight, marginedWidth, labelHeight);
	int infoTop = vMargin + buttonHeight + labelHeight;
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
		if (showing == showingVersion)
			showPath();
		else
			showVersion();
		}
}


void SFZeroEditor::timerCallback()
{
#ifdef JUCE_DEBUG
	getProcessor()->relayLogMessages();
#endif
}


void SFZeroEditor::chooseFile()
{
	FileChooser chooser(
		"Select an SFZ file...",
		File::nonexistent,
		"*.sfz;*.SFZ");
	if (chooser.browseForFileToOpen()) {
		File sfzFile(chooser.getResult());
		setFile(&sfzFile);
		}
}


void SFZeroEditor::setFile(File* newFile)
{
	double progress;
	pathLabel.setVisible(false);
	fileLabel.setVisible(false);
	infoLabel.setVisible(false);
	ProgressBar progressBar(progress);
	addAndMakeVisible(&progressBar);
	int marginedWidth = getWidth() - 2 * hMargin;
	progressBar.setBounds(
		hMargin, vMargin, marginedWidth, progressBarHeight);

	SFZeroAudioProcessor* processor = getProcessor();
	processor->setSfzFile(newFile, &progress);

	SFZSound* sound = processor->getSound();
	if (sound)
		infoLabel.setText(sound->getErrorsString(), false);

	removeChildComponent(&progressBar);
	pathLabel.setVisible(true);
	fileLabel.setVisible(true);
	infoLabel.setVisible(true);

	updateFile(newFile);
}


void SFZeroEditor::updateFile(File* file)
{
	fileLabel.setText(file->getFileName(), false);
	fileLabel.setColour(Label::textColourId, Colours::black);
	showPath();
}


void SFZeroEditor::showVersion()
{
	struct tm tm;
	strptime(__DATE__, "%b %d %Y", &tm);
	char str[64];
	sprintf(str, "SFZero beta %d.%d.%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	pathLabel.setText(str, false);
	showing = showingVersion;
}


void SFZeroEditor::showPath()
{
	SFZeroAudioProcessor* processor = getProcessor();
	File file = processor->getSfzFile();
	pathLabel.setText(file.getParentDirectory().getFullPathName(), false);
	showing = showingPath;
}



