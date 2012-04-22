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
    void	pitchWheelMoved(const int newValue);
    void	controllerMoved(
			const int controllerNumber,
			const int newValue);
    void	renderNextBlock(
			AudioSampleBuffer& outputBuffer, int startSample, int numSamples);

		int	getGroup();
		int	getOffBy();

	protected:
		SFZRegion*	region;
		int       	curMidiNote, curPitchWheel;
		double    	pitchRatio;
		float     	noteGainLeft, noteGainRight;
		double    	sourceSamplePosition;
		SFZEG     	ampeg;

		void	calcPitchRatio();
		void	killNote();
		double	noteHz(double note, const double freqOfA = 440.0);
	};


#endif 	// SFZVoice_h

