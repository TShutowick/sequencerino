#include <vector>
#include <chrono>
#pragma once
#define OUTPUT 1
#define INPUT 1
typedef unsigned char byte;

bool digitalRead(unsigned int pin);

void pinMode(int, int);

void digitalWrite(unsigned int pin, bool value);

class MockSerial
{
private:
	int i;
	std::vector<int> queue;

public:
	MockSerial()
	{
		i = 0;
	}
	std::vector<int> tx_queue;
	int read()
	{
		if (i >= queue.size())
		{
			queue.clear();
			i = 0;
			return -1;
		}

		return queue[i++];
	}
	void push(std::vector<int> x)
	{
		queue.insert(queue.end(), x.begin(), x.end());
	}
	void write(int x)
	{
		tx_queue.push_back(x);
	}
	void begin(int) {}
};

unsigned long millis();

inline MockSerial Serial;
