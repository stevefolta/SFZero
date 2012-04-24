#include "SFZDebug.h"


FifoLogger::FifoLogger(Logger* outputLoggerIn)
	: outputLogger(outputLoggerIn), fifo(capacity)
{
}


FifoLogger::~FifoLogger()
{
	delete outputLogger;
}


void FifoLogger::logMessage(const String& message)
{
	const char* p;

	// Stupid String class doesn't really let us get number of bytes.
	const char* bytes = message.getCharPointer();
	unsigned long msgSize = strlen(bytes);
	int totalSize = sizeof(unsigned long) + msgSize;
	int start1, size1, start2, size2;
	fifo.prepareToWrite(totalSize, start1, size1, start2, size2);
	int givenSize = size1 + size2;
	if (givenSize < totalSize)
		msgSize -= givenSize - totalSize;

	// Write the count.
	if (size1 >= sizeof(unsigned long)) {
		memcpy(&buffer[start1], &msgSize, sizeof(unsigned long));
		size1 -= sizeof(unsigned long);
		start1 += sizeof(unsigned long);
		}
	else {
		p = (const char*) &msgSize;
		memcpy(&buffer[start1], p, size1);
		p += size1;
		size1 = 0;
		int bytesLeft = sizeof(unsigned long) - size1;
		memcpy(&buffer[start2], p, bytesLeft);
		start2 += bytesLeft;
		size2 -= bytesLeft;
		}

	// Write the string.
	p = bytes;
	if (size1 > 0) {
		memcpy(&buffer[start1], p, size1);
		p += size1;
		}
	if (size2 > 0)
		memcpy(&buffer[start2], p, size2);

	fifo.finishedWrite(givenSize);
}


void FifoLogger::relayMessages()
{
	// Juce has stupidly protected Logger::logMessage(), so we have to do it this
	// way.
	setCurrentLogger(outputLogger, false);

	while (hasMessage()) {
		String message = nextMessage();
		writeToLog(message);
		}

	setCurrentLogger(this, false);
}


String FifoLogger::nextMessage()
{
	// Read the count.
	unsigned long msgSize = 0;
	int start1, size1, start2, size2;
	fifo.prepareToRead(sizeof(unsigned long), start1, size1, start2, size2);
	char* p = (char*) &msgSize;
	if (size1 > 0) {
		memcpy(p, &buffer[start1], size1);
		p += size1;
		}
	if (size2 > 0)
		memcpy(p, &buffer[start2], size2);
	fifo.finishedRead(size1 + size2);

	// Read the string.
	String result;
	fifo.prepareToRead(msgSize, start1, size1, start2, size2);
	if (start1 > 0) {
		p = &buffer[start1];
		result = String(CharPointer_UTF8(p), CharPointer_UTF8(p + size1));
		}
	if (start2 > 0) {
		p = &buffer[start2];
		result += String(CharPointer_UTF8(p), CharPointer_UTF8(p + size2));
		}
	fifo.finishedRead(size1 + size2);
}


bool FifoLogger::hasMessage()
{
	return fifo.getNumReady() > 0;
}



