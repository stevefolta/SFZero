#ifndef SF2Sound_h
#define SF2Sound_h

#include "SFZSound.h"


class SF2Sound : public SFZSound {
	public:
		SF2Sound(const File& file);
		~SF2Sound();

		void	loadRegions();
		void	loadSamples(
			AudioFormatManager* formatManager,
			double* progressVar = NULL, Thread* thread = NULL);

		struct Preset {
			String	name;
			int   	preset;
			OwnedArray<SFZRegion>	regions;

			Preset(String nameIn, int presetIn)
				: name(nameIn), preset(presetIn) {}
			~Preset() {}

			void	addRegion(SFZRegion* region) {
				regions.add(region);
				}
			};
		void	addPreset(Preset* preset);

		int	numSubsounds();
		String	subsoundName(int whichSubsound);
		void	useSubsound(int whichSubsound);
		int 	selectedSubsound();

		SFZSample*	sampleFor(unsigned long sampleRate);
		void	setSamplesBuffer(AudioSampleBuffer* buffer);

	protected:
		OwnedArray<Preset>	presets;
		HashMap<unsigned long, SFZSample*>	samplesByRate;
		int               	selectedPreset;
	};


#endif 	// !SF2Sound_h

