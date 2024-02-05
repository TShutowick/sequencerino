#include "MockArduino.h"

bool pins[8] = {0, 0, 0, 0, 0, 0, 0, 0};

bool digitalRead(unsigned int pin)
{
    if (pin > 8 || pin == 0)
        return 0;
    return pins[pin - 1];
}

void pinMode(int, int) {}

void digitalWrite(unsigned int pin, bool value)
{
    if (pin > 8 || pin == 0)
        return;
    pins[pin - 1] = value;
}

auto start = std::chrono::high_resolution_clock::now();

unsigned long millis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::high_resolution_clock::now() - start)
        .count();
}
