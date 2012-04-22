#include "SFZRegion.h"
#include "SFZSample.h"
#include <string.h>
#include <stdio.h>


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



