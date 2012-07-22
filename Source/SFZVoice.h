#ifndef SFZVoice_h
#define SFZVoice_h

#include "../JuceLibraryCode/JuceHeader.h"
#include "SFZEG.h"

class SFZRegion;


class JUCE_API SFZVoice : public SynthesiserVoice {
	public:
		SFZVoice();
		~SFZVoice();

    bool	canPlaySound(SynthesiserSound* sound);
    void	startNote(
			const int midiNoteNumber,
			const float velocity,
			SynthesiserSound* sound,
			const int currentPitchWheelPosition);
    void	stopNote(const bool allowTailOff);
		void	stopNoteForGroup();
		void	stopNoteQuick();
    void	pitchWheelMoved(const int newValue);
    void	controllerMoved(
			const int controllerNumber,
			const int newValue);
    void	renderNextBlock(
			AudioSampleBuffer& outputBuffer, int startSample, int numSamples);
		bool	isPlayingNoteDown();
		bool	isPlayingOneShot();

		int	getGroup();
		int	getOffBy();

		// Set the region to be used by the next startNote().
		void	setRegion(SFZRegion* nextRegion);

	protected:
		int       	trigger;
		SFZRegion*	region;
		int       	curMidiNote, curPitchWheel;
		double    	pitchRatio;
		float     	noteGainLeft, noteGainRight;
		double    	sourceSamplePosition;
		SFZEG     	ampeg;
		unsigned long	sampleEnd;
		unsigned long	loopStart, loopEnd;

		void	calcPitchRatio();
		void	killNote();
		double	noteHz(double note, const double freqOfA = 440.0);
	};


#endif 	// SFZVoice_h

