#ifndef SFZSynth_h
#define SFZSynth_h

namespace SFZero {

class SFZSynth : public juce::Synthesiser {
	public:
		SFZSynth();
		~SFZSynth() override = default;

		void	noteOn(int midiChannel, int midiNoteNumber, float velocity) override;
		void	noteOff(
			int midiChannel, int midiNoteNumber,
			float velocity, bool allowTailOff) override;

		int	numVoicesUsed();
		juce::String   	voiceInfoString();

	protected:
		unsigned char	noteVelocities[128];
	};

}

#endif 	// !SFZSynth_h

