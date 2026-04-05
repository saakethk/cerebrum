# Variables
CXX = g++
CXXFLAGS = -I include -I libs -std=c++20 -Wall -Werror -Wextra -g
TEST_FILES = tests/chunks/*.cpp tests/*.cpp

# Default target: builds the main app
all:
	$(CXX) $(CXXFLAGS) src/*.cpp -o build/dev

test:
	$(CXX) $(CXXFLAGS) $(TEST_FILES) src/*.cpp -o build/dev
	./build/dev