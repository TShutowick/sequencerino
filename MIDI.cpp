#include "MIDI.h"

namespace MIDI {

MidiStatus parse_status_byte() {
  MidiStatus ret = {};
  byte b = Serial1.read();
  if (b >= 0x80 && b <= 0x9F) {
    ret.status = (b >> 4) << 4;
    ret.channel = (b & 0x0f);
  }
  return ret;
}

void write_note_on(byte channel, MidiNote note) {
	byte status = build_status_byte(NOTE_ON, channel);
	Serial1.write(status);
	Serial1.write(note.note);
	Serial1.write(note.velocity);
}
void write_note_off(byte channel, MidiNote note) {
	byte status = build_status_byte(NOTE_OFF, channel);
	Serial1.write(status);
	Serial1.write(note.note);
	Serial1.write(note.velocity);
}
byte build_status_byte(byte command, byte channel) {
  return command | (channel & 0x0f);
}

}
