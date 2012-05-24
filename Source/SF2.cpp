#include "SF2.h"


#define readAbyte(name, file) 	\
	name = (byte) file->readByte();
#define readAchar(name, file) 	\
	name = file->readByte();
#define readAdword(name, file) 	\
	name = (dword) file->readInt();
#define readAword(name, file) 	\
	name = (word) file->readShort();
#define readAshort(name, file) 	\
	name = file->readShort();
#define readAchar20(name, file) 	\
	file->read(name, 20);
#define readAgenAmountType(name, file) 	\
	name.shortAmount = file->readShort();

#define SF2Field(type, name) 	\
	readA##type(name, file)


void SF2::iver::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/iver.h"
}


void SF2::phdr::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/phdr.h"
}


void SF2::pbag::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/pbag.h"
}


void SF2::pmod::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/pmod.h"
}


void SF2::pgen::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/pgen.h"
}


void SF2::inst::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/inst.h"
}


void SF2::ibag::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/ibag.h"
}


void SF2::imod::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/imod.h"
}


void SF2::igen::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/igen.h"
}


void SF2::shdr::ReadFrom(InputStream* file)
{
	#include "sf2-chunks/shdr.h"
}



