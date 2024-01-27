#include "MIDI.h"

#define MAX_NOTES 256
#define MAX_BEATS 16
#define PULSES_PER_BEAT 24

struct Note {
  byte channel;
  byte beat;
  byte note;
  short velocity; // negative is off
};

struct MidiStatus {
  byte status;
  byte channel;
};

class Sequencer {
public:
  byte num_beats;
  // Recorded data
  Note notes[MAX_NOTES];
  short num_notes;
  // Intermediate data needed for recording
  byte cur_beat;
  byte cur_pulse;

  int last_pulse_time = 0;
  // length in ms of one pulse, of which there are 24 per beat
  // this is necessary in order for midi clock signals to work
  int pulse_length = 20;

  Sequencer(int max_beats=16) {
    num_beats = max_beats;
    clear();
  }

  void clear() {
    num_notes = 0;
    cur_beat = 0;
    cur_pulse = 0;
  }

  // Increment the pulse until it hits 24, then increment the beat.
  int step() {
    last_pulse_time = millis();
    ++cur_pulse;
    if (cur_pulse == PULSES_PER_BEAT) {
        cur_pulse = 0;
        ++cur_beat;
        if (cur_beat == num_beats) {
          cur_beat = 0;
        }
    }
    return cur_pulse;
  }

  bool maybe_step() {
    int now = millis();
    if (now - last_pulse_time > pulse_length) {
      step();
      return true;
    }
    return false;
  }

  bool add_note_on(byte channel, byte note, short velocity=127) {
    if (num_notes >= MAX_NOTES) {
      return false;
    }
    notes[num_notes] = {channel,cur_beat, note, velocity};
    ++num_notes;
    return true;
  }

  bool add_note_off(byte channel, byte note, short velocity=127) {
    if(!add_note_on(channel, note, -velocity)) {
      return false;
    }
    // If there is a "note-on" event on the same beat, then move this note off
    // to the next beat.
    for (short i = 0; i < num_notes - 1; ++i) {
      if (notes[i].note == note && notes[i].velocity > 0) {
        // Move the note-off we just added to the next beat
        ++notes[num_notes - 1].beat;
        break;
      }
    }
    return true;
  }
  void play() {
    for (short i = 0; i < num_notes; ++i) {
      if (notes[i].beat == cur_beat) {
        byte status = 0;
        if (notes[i].velocity > 0) {
          status = MIDI::build_status_byte(NOTE_ON, notes[i].channel);
        } else {
          status = MIDI::build_status_byte(NOTE_OFF, notes[i].channel);
        }
        Serial1.write(status);
        Serial1.write(notes[i].note);
        Serial1.write(abs(notes[i].velocity));
      }
    }
  }
};
Sequencer seq;




void setup()
{
    Serial.begin(9600);
    Serial1.begin(31250,SERIAL_8N1);
    pinMode(LED_BUILTIN, OUTPUT);
}

int read_note() {
    int incoming_byte = Serial1.read();
    while (incoming_byte < 0) incoming_byte = Serial1.read();
    return incoming_byte;
}


bool handle_input() {
    MIDI::MidiStatus status_byte = MIDI::parse_status_byte();
    if (!status_byte.status) return false;

    int note_byte = read_note();
    int vel_byte = read_note();
    if (status_byte.status == NOTE_ON) {
      seq.add_note_on(status_byte.channel, note_byte, vel_byte);
    }
    if (status_byte.status == NOTE_OFF) {
      seq.add_note_off(status_byte.channel, note_byte, vel_byte);
    }
    Serial1.write( MIDI::build_status_byte(status_byte.status,status_byte.channel));
    Serial1.write(note_byte);
    Serial1.write(vel_byte);
    return true;
}

void loop()
{
    handle_input();
    if (seq.maybe_step()) {
      seq.play();
    }
    if (seq.cur_pulse == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else if (seq.cur_pulse == PULSES_PER_BEAT / 2) {
        digitalWrite(LED_BUILTIN, LOW);
    }
}
