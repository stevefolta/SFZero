#ifndef SF2Sound_h
#define SF2Sound_h

#include "SFZSound.h"


namespace SFZero {

class SF2Sound : public SFZSound
{
public:
	SF2Sound(const juce::File& file);
	~SF2Sound() override;

	void	loadRegions() override;
	void	loadSamples(juce::AudioFormatManager* formatManager, double* progressVar = NULL, juce::Thread* thread = NULL) override;

	struct Preset
	{
		juce::String	name;
		int    	bank;
		int   	preset;
		juce::OwnedArray<SFZRegion>	regions;

		Preset(juce::String nameIn, int bankIn, int presetIn)
			: name(nameIn), bank(bankIn), preset(presetIn) {}
		~Preset() {}

		void addRegion(SFZRegion* region) { regions.add(region); }
		};
	void	addPreset(Preset* preset);

	int	numSubsounds() override;
	juce::String	subsoundName(int whichSubsound) override;
	void	useSubsound(int whichSubsound) override;
	int 	selectedSubsound() override;

	SFZSample* sampleFor (unsigned long sampleRate);
	void setSamplesBuffer (juce::AudioSampleBuffer* buffer);

protected:
	juce::OwnedArray<Preset>	presets;
	juce::HashMap<int64_t, SFZSample*>	samplesByRate;
	int selectedPreset;
};

}

#endif 	// !SF2Sound_h

