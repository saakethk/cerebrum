# Variables
CXX = g++
CXXFLAGS = -I include -I libs -std=c++20 -Wall -Werror -Wextra -g

# Default target: builds the main app
all:
	$(CXX) $(CXXFLAGS) src/*.cpp -o build/dev

test:
	$(CXX) $(CXXFLAGS) src/*.cpp tests/*.cpp -o build/dev
	./build/dev