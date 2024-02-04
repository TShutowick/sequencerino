#include <vector>
#include <chrono>
#pragma once
#define OUTPUT 1
#define INPUT 1
typedef unsigned char byte;

inline bool pins[8] = {0,0,0,0,0,0,0,0};

inline bool digitalRead(unsigned int pin) {
	if (pin > 8 || pin == 0) return 0;
	return pins[pin-1];
}

inline void pinMode(int,int){}

inline void digitalWrite(unsigned int pin, bool value) {
	if (pin > 8 || pin == 0) return;
	pins[pin-1] = value;
}


class MockSerial {
	private:
		int i = 0;
		std::vector<int> queue;
	public: 
		std::vector<int> tx_queue;
		int read() {
			if (i >= queue.size()) {
				queue.clear();
				i = 0;
				return -1;
			}

			return queue[i++];
		}
		void push (std::vector<int> x) {
			queue.insert(queue.end(), x.begin(), x.end());
		}
		void write (int x) {
			tx_queue.push_back(x);
		}
		void begin (int) {}

};

inline auto start = std::chrono::high_resolution_clock::now();

inline unsigned long millis() {
	 return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - start
     ).count();
}

inline MockSerial Serial;
