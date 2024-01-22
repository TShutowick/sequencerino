#define NOTE_ON 90
#define NOTE_OFF 80

#define MAX_NOTES 256
#define MAX_BEATS 16

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

  // the last time 
  int last_step_time = 0;
  // length in ms of one beat
  int beat_length = 500;
  
  Sequencer(int max_beats=16) {
    num_beats = max_beats;
    clear();
  }

  void clear() {
    num_notes = 0;
    cur_beat = 0;
  }

  int step() {
    last_step_time = millis();
    ++cur_beat;
    cur_beat %= num_beats;
    return ++cur_beat;
  }

  bool maybe_step() {
    int now = millis();
    if (now - last_step_time > beat_length) {
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
        if (notes[i].velocity > 0) {
          Serial.print("PLAYING ");
    byte noteOnStatus = 0x90 | (notes[i].channel & 0x0f);
Serial.print(noteOnStatus, HEX);
    //Transmit a Note-On message
    Serial1.write(noteOnStatus);
    Serial1.write(notes[i].note);
    Serial1.write(notes[i].velocity);
        } else {
          // TODO: send note off
          
        }
      }
    }
  }
};
Sequencer seq;




void setup()
{
  Serial.begin(9600);
  Serial1.begin(31250,SERIAL_8N1);
}

MidiStatus readStatus() {
      int incomingByte = Serial1.read();
      if (incomingByte >= 0x80 && incomingByte <= 0x9F) {
              Serial.println(incomingByte,HEX);

          byte status = (incomingByte >> 4) * 10;
          byte channel = (incomingByte & 0x0f);
          return {status,channel};
      }
      return {0,0};
}

int readNote() {
    int incomingByte = Serial1.read();
    while (incomingByte < 0) incomingByte = Serial1.read();
    return incomingByte;
}


bool handleInput() {
    MidiStatus statusByte = readStatus();
    if (!statusByte.status) return false;

    int noteByte = readNote();
    int velByte = readNote();
    if (statusByte.status == NOTE_ON) {
      Serial.print("NOTE ON ");
      seq.add_note_on(noteByte, velByte);
    }
    if (statusByte.status == NOTE_OFF) {
      Serial.print("NOTE OFF ");
      seq.add_note_off(noteByte, velByte);
    }
    Serial.print(noteByte);
    Serial.print(" CHANNEL ");
    Serial.print(statusByte.channel);
    Serial.print(" VELOCITY ");
    Serial.println(velByte);
    return true;
}

void loop()
{
  handleInput();
  if (seq.maybe_step()) {
    seq.play();  
  }
}

