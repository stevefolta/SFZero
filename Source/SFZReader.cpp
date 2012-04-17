#include "SFZReader.h"
#include "SFZRegion.h"
#include "SFZSound.h"


SFZReader::SFZReader(const char* text, unsigned int length)
	: p(text), end(text + length)
{
	sound = new SFZSound();
	errors = new StringArray();
}



