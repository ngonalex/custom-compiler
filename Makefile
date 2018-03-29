
CC = g++
CFLAGS = -Wall -std=c++11 -g

compile: src/rcc.cc 
	@$(CC) $(CFLAGS) -Iinclude/ -I/usr/local/include/ -o rcc src/rcc.cc

run_tests: tests/test.cc 
	@$(CC) $(CFLAGS) -o run_tests tests/test.cc -pthread -lgtest
	
test: run_tests
	@./run_tests

clean:
	@rm -rf run_tests rcc

all: compile test
