#include <iostream>
#include "../Sequencer.h"
#include "../Repeater.h"

Sequencer seq;
unsigned long now = 0;
Repeater seq_runner([]{
	seq.play();
	for (int i = 0; i < 6; i++) {
		std::cout << Serial.tx_queue[i] << "\n";
	}
	Serial.tx_queue.clear();
}, 500, &now);

int main() {
	while(1) {
		now = millis();
		seq_runner.loop();
	}
	return 0;
}
