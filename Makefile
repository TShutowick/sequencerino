.PHONY: test
test:
	g++ t/tests.cpp -o test && ./test && rm test
