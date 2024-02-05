#include <iostream>
#include "lib/MockArduino.h"

#include "../MIDI.h"
bool ok(bool test_passes, std::string msg)
{
    if (test_passes)
    {
        // those random characters make it green.
        std::cout << "\033[1;32m" << msg << "\033[0m\n";
    }
    else
    {
        // those random characters make it red.
        std::cout << "\033[1;31m" << msg << "\033[0m\n";
    }
    return test_passes;
}

template <typename T>
bool is(T got, T want, std::string msg)
{
    if (ok(got == want, msg))
    {
        return true;
    }
    std::cout << "want " << want << "; got " << got << "\n";
    return false;
}

void test_parse_status()
{
    Serial.push(std::vector<int>{0x80, 0x93, 0x55});
    MIDI::MidiStatus status = MIDI::parse_status_byte();
    is((int)status.status, NOTE_OFF, "Correct MIDI command (Note Off)");
    is((int)status.channel, 0, "Correct MIDI channel");

    status = MIDI::parse_status_byte();
    is((int)status.status, NOTE_ON, "Correct MIDI command (Note On)");
    is((int)status.channel, 3, "Correct MIDI channel");

    status = MIDI::parse_status_byte();
    is((int)status.status, 0, "Empty MIDI status");
}

void test_build_status()
{
    int status = MIDI::build_status_byte(0x80, 3);
    is(status, 0x83, "Correct status byte (Note Off on Channel 3)");
}

void test_mock_serial()
{
    Serial.push(std::vector<int>{1, 2});
    int x = Serial.read();
    is(x, 1, "First item");
    x = Serial.read();
    is(x, 2, "Last item");
    x = Serial.read();
    is(x, -1, "No data");
}

void test_write_note_on()
{
    MIDI::MidiNote note;
    MIDI::write_note_on(1, {.note = 100, .velocity = 100});
    is(Serial.tx_queue[0], NOTE_ON + 1, "status byte");
    is(Serial.tx_queue[1], 0x64, "note byte");
    is(Serial.tx_queue[2], 0x64, "velocity byte");
}

int main()
{
    test_parse_status();
    test_build_status();
    test_mock_serial();
    test_write_note_on();
    return 0;
}
