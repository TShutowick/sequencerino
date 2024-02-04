.PHONY: test
test:
	g++ -o test t/tests.cpp MIDI.cpp -o test
	./test
	rm test
