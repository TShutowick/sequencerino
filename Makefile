.PHONY: test
test:
	g++ -std=c++17 -o test t/tests.cpp MIDI.cpp t/lib/MockArduino.cpp -o test
	./test
	rm test
