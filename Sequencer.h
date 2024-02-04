#include "MIDI.h"

#define STEPS 16
#define CHANNELS 3

using namespace MIDI;

class Sequencer {
	private:
		byte notes[CHANNELS][STEPS];
		byte current_step = 0;
	public:
		Sequencer() {
			for (byte channel = 0; channel < CHANNELS; channel++) {
				for (byte step = 0; step < STEPS; step++) {
					notes[channel][step] = 60;
				}
			}
		}
		void play() {
			for (byte channel = 0; channel < CHANNELS; channel++) {
				// note value of 0 means hold the previous note
				if (notes[channel][current_step] == 0) continue;
				write_note_off(channel, {notes[channel][current_step],0});
			}
			current_step++;
			if (current_step >= STEPS - 1) {
				current_step = 0;
			}
			for (byte channel = 0; channel < CHANNELS; channel++) {
				if (notes[channel][current_step] == 0) continue;
				write_note_on(channel, {notes[channel][current_step],100});
			}
		}
		void set_note(byte channel, byte note) {
			notes[channel][current_step] = note;
		}
};

