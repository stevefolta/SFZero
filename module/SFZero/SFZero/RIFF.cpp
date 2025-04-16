#include "RIFF.h"

namespace SFZero {

void RIFFChunk::ReadFrom(juce::InputStream* file)
{
	file->read(&id, sizeof(fourcc));
	size = (dword) file->readInt();
	start = file->getPosition();

	if (FourCCEquals(id, "RIFF")) {
		type = RIFF;
		file->read(&id, sizeof(fourcc));
		start += sizeof(fourcc);
		size -= sizeof(fourcc);
		}
	else if (FourCCEquals(id, "LIST")) {
		type = LIST;
		file->read(&id, sizeof(fourcc));
		start += sizeof(fourcc);
		size -= sizeof(fourcc);
		}
	else
		type = Custom;
}


void RIFFChunk::Seek(juce::InputStream* file)
{
	file->setPosition(start);
}


void RIFFChunk::SeekAfter(juce::InputStream* file)
{
	int64_t next = start + size;
	if (next % 2 != 0)
		next += 1;
	file->setPosition(next);
}


juce::String RIFFChunk::ReadString(juce::InputStream* file)
{
	char* str = (char*)alloca (size);
	file->read(str, int (size));
	return juce::String(str);
}

}
