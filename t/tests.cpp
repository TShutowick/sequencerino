#include <iostream>

typedef unsigned char byte;

#include "../MIDI.h"
bool ok(bool test_passes, std::string msg) {
   if (test_passes) {
        // those random characters make it green.
        std::cout << "\033[1;32m" << msg  << "\033[0m\n";
   } else {
        // those random characters make it red.
        std::cout << "\033[1;31m" << msg  << "\033[0m\n";
   }
   return test_passes;
}

template <typename T>
bool is(T got, T want, std::string msg) {
    if (ok(got == want, msg)) {
        return true;
    }
    std::cout << "want " << want << "; got " << got << "\n";
    return false;
}

void test_parse_status() {
    MIDI::MidiStatus status = MIDI::parse_status_byte(0x80);
    is((int)status.status,  NOTE_OFF, "Correct MIDI command (Note Off)");
    is((int)status.channel, 0, "Correct MIDI channel");

    status = MIDI::parse_status_byte(0x93);
    is((int)status.status,  NOTE_ON, "Correct MIDI command (Note On)");
    is((int)status.channel, 3, "Correct MIDI channel");

    status = MIDI::parse_status_byte(0x55);
    is((int)status.status, 0, "Empty MIDI status");
}

void test_build_status() {
    int status = MIDI::build_status_byte(0x80, 3);
    is(status, 0x83, "Correct status byte (Note Off on Channel 3)");
}

int main() {
    test_parse_status();
    test_build_status();
    return 0;
}
