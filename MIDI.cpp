#include "MIDI.h"

namespace MIDI {

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

//Code in here will only be compiled if an Arduino Mega is used.
auto MidiSerial = &Serial1;
#else
auto MidiSerial = &Serial;
#endif

void begin() {
  MidiSerial->begin(31250);
}

MidiStatus parse_status_byte() {
  MidiStatus ret = {};
  byte b = MidiSerial->read();
  if (b >= 0x80 && b <= 0x9F) {
    ret.status = (b >> 4) << 4;
    ret.channel = (b & 0x0f);
  }
  return ret;
}

void write_note_on(byte channel, MidiNote note) {
	byte status = build_status_byte(NOTE_ON, channel);
	MidiSerial->write(status);
	MidiSerial->write(note.note);
	MidiSerial->write(note.velocity);
}

void write_note_off(byte channel, MidiNote note) {
	byte status = build_status_byte(NOTE_OFF, channel);
	MidiSerial->write(status);
	MidiSerial->write(note.note);
	MidiSerial->write(note.velocity);
}

byte build_status_byte(byte command, byte channel) {
  return command | (channel & 0x0f);
}

}
