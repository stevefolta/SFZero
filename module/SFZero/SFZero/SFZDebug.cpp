#include "SFZDebug.h"
#include <stdarg.h>

namespace SFZero {

#ifdef JUCE_DEBUG

static LogFifo* fifo = nullptr;


LogFifo::LogFifo (juce::Logger* l)
	: fifo(capacity), logger (l)
{
}


LogFifo::~LogFifo()
{
	if (juce::Logger::getCurrentLogger() == logger.get())
		juce::Logger::setCurrentLogger(nullptr);
	SFZero::fifo = nullptr;
}


void LogFifo::logMessage(const juce::String& message)
{
	const char* p;

	// Stupid String class doesn't really let us get number of bytes.
	const char* bytes = message.getCharPointer();
	unsigned long msgSize = strlen(bytes);
	int totalSize = int (sizeof(unsigned long) + msgSize);
	int start1, size1, start2, size2;
	fifo.prepareToWrite(totalSize, start1, size1, start2, size2);
	int givenSize = size1 + size2;
	if (givenSize < totalSize)
		msgSize -= givenSize - totalSize;

	// Write the count.
	if (size1 >= int (sizeof(unsigned long))) {
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


void LogFifo::relayMessages()
{
	while (hasMessage())
	{
		juce::String message = nextMessage();
		juce::Logger::writeToLog(message);
	}
}


juce::String LogFifo::nextMessage()
{
	// Read the count.
	unsigned long msgSize = 0;
	int start1, size1, start2, size2;
	fifo.prepareToRead(sizeof(unsigned long), start1, size1, start2, size2);
	char* p = (char*) &msgSize;
	if (size1 > 0)
	{
		memcpy(p, &buffer[start1], size1);
		p += size1;
	}
	if (size2 > 0)
		memcpy(p, &buffer[start2], size2);
	fifo.finishedRead(size1 + size2);

	// Read the string.
	juce::String result;
	fifo.prepareToRead(int (msgSize), start1, size1, start2, size2);
	if (size1 > 0)
	{
		p = &buffer[start1];
		result = juce::String(juce::CharPointer_UTF8(p), juce::CharPointer_UTF8(p + size1));
	}
	if (size2 > 0)
	{
		p = &buffer[start2];
		result += juce::String(juce::CharPointer_UTF8(p), juce::CharPointer_UTF8(p + size2));
	}
	fifo.finishedRead(size1 + size2);

	return result;
}


bool LogFifo::hasMessage()
{
	return fifo.getNumReady() > 0;
}



void setupLogging (juce::Logger* logger)
{
	if (fifo == nullptr)
		fifo = new LogFifo(logger);
	juce::Logger::setCurrentLogger(logger);
}


void fifoLogMessage(const juce::String& message)
{
	if (fifo)
		fifo->logMessage(message);
}


void relayFifoLogMessages()
{
	if (fifo)
		fifo->relayMessages();
}


void dbgprintf(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	char output[256];
	vsnprintf(output, 256, msg, args);
	fifoLogMessage(output);

	va_end(args);
}


#endif 	// JUCE_DEBUG


}
