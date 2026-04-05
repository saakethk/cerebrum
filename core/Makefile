# Variables
CXX = g++
CXXFLAGS = -I include -I libs -std=c++20 -Wall -Werror -Wextra -g
TEST_FILES = tests/chunks/*.cpp tests/*.cpp

# Default target: builds the main app
main:
	$(CXX) $(CXXFLAGS) src/*.cpp -o build/dev_main
	./build/dev_main

test:
	$(CXX) $(CXXFLAGS) $(TEST_FILES) src/*.cpp -o build/dev_test
	./build/dev_test