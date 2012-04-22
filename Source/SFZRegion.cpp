#include "SFZRegion.h"
#include "SFZSample.h"
#include <string.h>
#include <stdio.h>


void SFZEGParameters::clear()
{
	delay = 0.0;
	start = 0.0;
	attack = 0.0;
	hold = 0.0;
	decay = 0.0;
	sustain = 100.0;
	release = 0.0;
}


void SFZEGParameters::clearMod()
{
	// Clear for velocity or other modification.
	delay = start = attack = hold = decay = sustain = release = 0.0;
}



SFZRegion::SFZRegion()
{
	clear();
}


void SFZRegion::clear()
{
	memset(this, 0, sizeof(*this));
	hikey = 127;
	hivel = 127;
	pitch_keycenter = 60; 	// C4
	volume = pan = 0.0;
	amp_veltrack = 100.0;
	ampeg.clear();
	ampeg_veltrack.clear();
}


void SFZRegion::dump()
{
	printf("%d - %d, vel %d - %d", lokey, hikey, lovel, hivel);
	if (sample) {
		char name[64];
		sample->getShortName().copyToUTF8(name, 64);
		printf(": %s", name);
		}
	printf("\n");
}



