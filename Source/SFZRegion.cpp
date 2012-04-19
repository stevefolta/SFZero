#include "SFZRegion.h"
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
}


void SFZRegion::dump()
{
	printf("%d - %d, vel %d - %d\n", lokey, hikey, lovel, hivel);
}



