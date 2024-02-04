#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#include "Globals.h"

typedef unsigned char byte;
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

byte build_status_byte(byte cmd, byte channel);

void write_note_on(byte channel,MidiNote note);
void write_note_off(byte channel,MidiNote note);

} // namespace MIDI
