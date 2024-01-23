#define NOTE_ON 0x90
#define NOTE_OFF 0x80

namespace MIDI{

struct MidiStatus {
  byte status;
  byte channel;
};

MidiStatus parse_status_byte(byte b) {
    MidiStatus ret = {};
    if (b >= 0x80 && b <= 0x9F) {
        ret.status = (b >> 4) << 4;
        ret.channel = (b & 0x0f);
    }
    return ret;
}

byte build_status_byte(byte command, byte channel) {
    return command | (channel & 0x0f);
}

}
