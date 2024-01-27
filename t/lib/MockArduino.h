#include <vector>
#define ARDUINO 1
typedef unsigned char byte;


class MockSerial {
	private:
		int i = 0;
		std::vector<int> queue;
	public: 
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
};

inline MockSerial Serial1;
