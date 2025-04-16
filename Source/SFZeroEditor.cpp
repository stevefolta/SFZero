#include "SFZeroEditor.h"
#include "SFZeroAudioProcessor.h"
#include <time.h>


enum {
	hMargin = 12,
	vMargin = 12,
	labelHeight = 25,
	progressBarHeight = 30,
	keyboardHeight = 54,
	};


SFZeroEditor::SFZeroEditor(SFZeroAudioProcessor* ownerFilter)
	: AudioProcessorEditor(ownerFilter),
		fileLabel(juce::String(), "File... (click here to choose)"),
		pathLabel(juce::String()),
		showingInfo(showingSoundInfo),
		midiKeyboard(ownerFilter->keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
		progressBar(NULL)
{
	setSize(500, 300);

#ifdef JUCE_MAC
	juce::Font fileFont(juce::FontOptions("Helvetica", 22.0, juce::Font::bold));
	juce::Font labelFont(juce::FontOptions("Helvetica", 15.0, juce::Font::plain));
#else
	juce::Font fileFont(juce::FontOptions("Ariel", 22.0, juce::Font::bold));
	juce::Font labelFont(juce::FontOptions("Ariel", 15.0, juce::Font::plain));
#endif

	addAndMakeVisible(&fileLabel);
	fileLabel.setFont(fileFont);
	fileLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
	fileLabel.addClickListener(this);

	addAndMakeVisible(&pathLabel);
	pathLabel.setFont(labelFont);
	pathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
	pathLabel.addClickListener(this);

	addAndMakeVisible(&infoLabel);
	infoLabel.setFont(labelFont);
	infoLabel.setJustificationType(juce::Justification::topLeft);
	infoLabel.addClickListener(this);

	addAndMakeVisible(&midiKeyboard);
	midiKeyboard.setOctaveForMiddleC(4);

	startTimer(200);

	juce::File sfzFile = ownerFilter->getSfzFile();
	if (sfzFile != juce::File()) {
		updateFile(sfzFile);
		showSoundInfo();
		auto sound = ownerFilter->getSound();
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


void SFZeroEditor::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::white);
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


void SFZeroEditor::labelClicked(juce::Label* clickedLabel)
{
	if (clickedLabel == &fileLabel)
		chooseFile();

	else if (clickedLabel == &pathLabel) {
		if (showing == showingSubsound) {
			SFZeroAudioProcessor* processor = getProcessor();
			auto sound = processor->getSound();
			if (sound) {
				juce::PopupMenu menu;
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
			auto sound = processor->getSound();
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
	juce::FileChooser chooser(
		"Select an SFZ file...",
		juce::File(),
		"*.sfz;*.SFZ;*.sf2;*.SF2");
	if (chooser.browseForFileToOpen()) {
		juce::File sfzFile(chooser.getResult());
		setFile(sfzFile);
		}
}


void SFZeroEditor::setFile(const juce::File& newFile)
{
	SFZeroAudioProcessor* processor = getProcessor();
	processor->setSfzFileThreaded(newFile);

	updateFile(newFile);
	showProgress();
}


void SFZeroEditor::updateFile(const juce::File& file)
{
	fileLabel.setText(file.getFileName(), juce::dontSendNotification);
	fileLabel.setColour(juce::Label::textColourId, juce::Colours::black);
	showPath();
}


void SFZeroEditor::showSoundInfo()
{
	SFZeroAudioProcessor* processor = getProcessor();
	auto sound = processor->getSound();
	if (sound)
		infoLabel.setText(sound->getErrorsString(), juce::dontSendNotification);
	showingInfo = showingSoundInfo;
}


void SFZeroEditor::showVoiceInfo()
{
	SFZeroAudioProcessor* processor = getProcessor();
	infoLabel.setText(processor->voiceInfoString(), juce::dontSendNotification);
	showingInfo = showingVoiceInfo;
}


void SFZeroEditor::showVersion()
{
	struct tm tm;
	strptime(__DATE__, "%b %d %Y", &tm);
	char str[64];
	snprintf(str, 64, "SFZero beta %d.%d.%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	pathLabel.setText(str, juce::dontSendNotification);
	pathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
	hideProgress();
	showing = showingVersion;
}


void SFZeroEditor::showPath()
{
	SFZeroAudioProcessor* processor = getProcessor();
	juce::File file = processor->getSfzFile();
	pathLabel.setText(
		file.getParentDirectory().getFullPathName(), juce::dontSendNotification);
	pathLabel.setColour(juce::Label::textColourId, juce::Colours::grey);
	hideProgress();
	showing = showingPath;
}


void SFZeroEditor::showSubsound()
{
	SFZeroAudioProcessor* processor = getProcessor();
	auto sound = processor->getSound();
	if (sound == nullptr)
		return;

	pathLabel.setText(
		sound->subsoundName(sound->selectedSubsound()), juce::dontSendNotification);
	pathLabel.setColour(juce::Label::textColourId, juce::Colours::black);
	hideProgress();
	showing = showingSubsound;
}


void SFZeroEditor::showProgress()
{
	SFZeroAudioProcessor* processor = getProcessor();
	pathLabel.setVisible(false);
	infoLabel.setVisible(false);
	progressBar = new juce::ProgressBar(processor->loadProgress);
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

bool SFZeroEditor::isInterestedInFileDrag (const juce::StringArray& files)
{
	if (files.size() == 1)
	{
		auto f = juce::File(files[0]);
		if (f.hasFileExtension(".sfz") || f.hasFileExtension(".sf2"))
			return true;
	}
	return false;
}

void SFZeroEditor::filesDropped (const juce::StringArray& files, int, int)
{
	setFile(juce::File(files[0]));
}

