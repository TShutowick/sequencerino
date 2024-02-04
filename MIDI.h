#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#ifndef ARDUINO
#include "t/lib/MockArduino.h"
#endif

namespace MIDI {

struct MidiStatus {
  byte status;
  byte channel;
};

struct MidiNote {
  byte note;
  byte velocity; // negative is off
};

MidiStatus parse_status_byte();

byte build_status_byte(byte, byte);

void write_note_on(byte,MidiNote);
void write_note_off(byte,MidiNote);

} // namespace MIDI
