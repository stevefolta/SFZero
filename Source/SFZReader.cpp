#include "SFZReader.h"
#include "SFZRegion.h"
#include "SFZSound.h"
#include "StringSlice.h"


SFZReader::SFZReader()
	: line(1)
{
	sound = new SFZSound();
}


SFZReader::~SFZReader()
{
	delete sound;
}


SFZSound* SFZReader::read(const char* text, unsigned int length)
{
	const char* p = text;
	const char* end = text + length;
	char c;

	SFZRegion curGroup;
	SFZRegion curRegion;
	SFZRegion* buildingRegion = NULL;

	while (p < end) {
		// We're at the start of a line; skip any whitespace.
		while (p < end) {
			c = *p;
			if (c != ' ' && c != '\t')
				break;
			p += 1;
			}
		if (p >= end)
			break;

		// Check if it's a comment line.
		if (c == '/') {
			// Skip to end of line.
			while (p < end) {
				c = *++p;
				if (c == '\n' || c == '\r')
					break;
				}
			p = handleLineEnd(p);
			continue;
			}

		// Check if it's a blank line.
		if (c == '\r' || c == '\n') {
			p = handleLineEnd(p);
			continue;
			}

		// Handle elements on the line.
		while (p < end) {
			c = *p;

			// Tag.
			if (c == '<') {
				p += 1;
				const char* tagStart = p;
				while (p < end) {
					c = *p++;
					if (c == '\n' || c == '\r') {
						error("Unterminated tag");
						goto fatalError;
						}
					else if (c == '>')
						break;
					}
				if (p >= end) {
					error("Unterminated tag");
					goto fatalError;
					}
				StringSlice tag(tagStart, p - 1);
				if (tag == "region") {
					if (buildingRegion && buildingRegion == &curRegion)
						finishRegion(&curRegion);
					curRegion = curGroup;
					buildingRegion = &curRegion;
					}
				else if (tag == "group") {
					if (buildingRegion && buildingRegion == &curRegion)
						finishRegion(&curRegion);
					curGroup.clear();
					buildingRegion = &curGroup;
					}
				else
					error("Illegal tag.");
				}

			// Parameter.
			else {
				// Get the parameter name.
				const char* parameterStart = p;
				while (p < end) {
					c = *p++;
					if (c == '=' || c == ' ' || c == '\t' || c == '\r' || c == '\n')
						break;
					}
				if (p >= end || c != '=') {
					error("Malformed parameter");
					goto nextElement;
					}
				StringSlice opcode(parameterStart, p - 1);
				if (opcode == "sample") {
					// "sample" is the only opcode that takes a string, and strings
					// are kind of funny to parse because they can contain whitespace.
					//***
					}
				else {
					const char* valueStart = p;
					while (p < end) {
						c = *p;
						if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
							break;
						p++;
						}
					String value(valueStart, p - valueStart);
					if (buildingRegion == NULL)
						error("Setting a parameter outside a region or group");
					else if (opcode == "lokey")
						buildingRegion->lokey = keyValue(value);
					else if (opcode == "hikey")
						buildingRegion->hikey = keyValue(value);
					else if (opcode == "key")
						buildingRegion->hikey = buildingRegion->lokey = keyValue(value);
					else if (opcode == "lovel")
						buildingRegion->lovel = value.getIntValue();
					else if (opcode == "hivel")
						buildingRegion->hivel = value.getIntValue();
					else if (opcode == "trigger")
						buildingRegion->trigger = (SFZRegion::Trigger) triggerValue(value);
					else if (opcode == "group")
						buildingRegion->group = (unsigned long) value.getLargeIntValue();
					else if (opcode == "off_by")
						buildingRegion->off_by = (unsigned long) value.getLargeIntValue();
					else if (opcode == "offset")
						buildingRegion->offset = (unsigned long) value.getLargeIntValue();
					else if (opcode == "end")
						buildingRegion->end = (unsigned long) value.getLargeIntValue();
					else if (opcode == "loop_mode")
						buildingRegion->loop_mode = (SFZRegion::LoopMode) loopModeValue(value);
					else if (opcode == "loop_start")
						buildingRegion->loop_start = (unsigned long) value.getLargeIntValue();
					else if (opcode == "loop_end")
						buildingRegion->loop_end = (unsigned long) value.getLargeIntValue();
					else if (opcode == "transpose")
						buildingRegion->transpose = value.getIntValue();
					else if (opcode == "tune")
						buildingRegion->tune = value.getIntValue();
					else if (opcode == "pitch_keycenter")
						buildingRegion->pitch_keycenter = keyValue(value);
					else if (opcode == "volume")
						buildingRegion->volume = value.getFloatValue();
					else if (opcode == "pan")
						buildingRegion->pan = value.getFloatValue();
					else
						sound->addUnsupportedOpcode(String(opcode.start, opcode.length()));
					}
				}

			// Skip to next element.
nextElement:
			c = 0;
			while (p < end) {
				c = *p;
				if (c != ' ' && c != '\t')
					break;
				p += 1;
				}
			if (c == '\r' || c == '\n') {
				p = handleLineEnd(p);
				break;
				}
			}
		}

fatalError:
	if (buildingRegion && buildingRegion == &curRegion)
		finishRegion(buildingRegion);
	SFZSound* result = sound;
	sound = NULL;
	return result;
}


const char* SFZReader::handleLineEnd(const char* p)
{
	// Check for DOS-style line ending.
	char lineEndChar = *p++;
	if (lineEndChar == '\r' && *p == '\n')
		p += 1;
	line += 1;
	return p;
}


int SFZReader::keyValue(const String& str)
{
	/***/
	return str.getIntValue();
}


int SFZReader::triggerValue(const String& str)
{
	if (str == "release")
		return SFZRegion::release;
	else if (str == "first")
		return SFZRegion::first;
	else if (str == "legato")
		return SFZRegion::legato;
	return SFZRegion::attack;
}


int SFZReader::loopModeValue(const String& str)
{
	if (str == "no_loop")
		return SFZRegion::no_loop;
	else if (str == "one_shot")
		return SFZRegion::one_shot;
	else if (str == "loop_continuous")
		return SFZRegion::loop_continuous;
	else if (str == "loop_sustain")
		return SFZRegion::loop_sustain;
	return SFZRegion::sample_loop;
}


void SFZReader::finishRegion(SFZRegion* region)
{
	SFZRegion* newRegion = new SFZRegion();
	*newRegion = *region;
	sound->addRegion(newRegion);
}


void SFZReader::error(const String& message)
{
	String fullMessage = message;
	fullMessage += " (line " + String(line) + ").";
	sound->addError(fullMessage);
}



