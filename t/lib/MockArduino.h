#include <vector>
#include <chrono>
#define ARDUINO 1
typedef unsigned char byte;

inline bool pins[8] = {0,0,0,0,0,0,0,0};

inline bool digitalRead(unsigned int pin) {
	if (pin > 8 || pin == 0) return 0;
	return pins[pin-1];
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

};

inline auto start = std::chrono::high_resolution_clock::now();

inline unsigned long millis() {
	 return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::high_resolution_clock::now() - start
     ).count();
}

inline MockSerial Serial1;
