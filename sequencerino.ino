#include "Globals.h"
#include "ActionButton.h"
#include "Sequencer.h"
#include "Repeater.h"

bool playing;
ActionButton start_stop(START_STOP_PIN,[]{
	playing = !playing;
});

Sequencer seq;

unsigned long now, tempo_press_delta, tempo_press_time = 0;

Repeater seq_runner([]{
	seq.play();
}, START_TEMPO, &now);

bool led_on;
Repeater tempo_led([]{
    led_on = !led_on;
    digitalWrite(TEMPO_LED_PIN, led_on); 
}, START_TEMPO, &now);

ActionButton tempo(TEMPO_PIN,[]{
    tempo_press_delta = now - tempo_press_time;
    tempo_led.interval = tempo_press_delta / 2;
    seq_runner.interval = tempo_press_delta / 2;
    tempo_led.last_invocation = now;
    tempo_press_time = now;
});

void setup() {
  pinMode(TEMPO_LED_PIN, OUTPUT);
  pinMode(TEMPO_PIN, INPUT);
  Serial1.begin(31250);
  now = millis();
}


void loop() {
  now = millis();
  start_stop.loop();
  tempo.loop();
  if (!playing) return;
  tempo_led.loop();
  seq_runner.loop();
}

