#ifndef SFZDebug_h
#define SFZDebug_h

#include "../JuceLibraryCode/JuceHeader.h"

// Juce's standard DBG is all wrong; it only writes to stdout.  So instead, use
// one that'll write to the real log.

#if JUCE_DEBUG
	#define SFZDBG(msg)	fifoLogMessage(msg)
	#define SHOW(item)	SFZDBG( #item " = " + String(item) )
#else
	#define	SFZDBG(msg)
	#define	SHOW(msg)
#endif


#ifdef JUCE_DEBUG

namespace SFZero {

class LogFifo {
	public:
		LogFifo();
		~LogFifo();

		void logMessage(const String& message);
		void	relayMessages();
		String	nextMessage();
		bool	hasMessage();

	protected:
		enum {
			capacity = 512 * 1024,
			};
		AbstractFifo	fifo;
		char	buffer[capacity];
	};

extern void setupLogging(Logger* logger);
extern void fifoLogMessage(const String& message);
extern void relayFifoLogMessages();

extern void dbgprintf(const char* msg, ...);

}

#endif 	// JUCE_DEBUG

#endif 	// !SFZDebug_h

