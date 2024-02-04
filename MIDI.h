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

MidiStatus parse_status_byte();

byte build_status_byte(byte command, byte channel);

} // namespace MIDI
