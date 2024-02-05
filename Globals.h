#pragma once
#define TEMPO_LED_PIN 2
#define TEMPO_PIN 3
#define START_STOP_PIN 4
#define START_TEMPO 500

#ifndef ARDUINO
#include "t/lib/MockArduino.h"
#else
#include "Arduino.h"
#endif
