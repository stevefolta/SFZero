#include "SFZSample.h"


void SFZSample::load()
{
	/***/
}


String SFZSample::getShortName()
{
	return file.getFileName();
}


void SFZSample::dump()
{
	char path[256];
	file.getFullPathName().copyToUTF8(path, 256);
	printf("%s\n", path);
}



