# Variables
CXX = g++
CXXFLAGS = -I include -I libs -std=c++20 -Wall -Werror -Wextra -g
TEST_FILES = libs/catch_amalgamated.cpp tests/chunks/*.cpp tests/table/*.cpp tests/*.cpp
SRC_FILES = src/chunks/*.cpp src/table/*.cpp

main:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) main.cpp -o build/dev_main
	./build/dev_main

test:
	$(CXX) $(CXXFLAGS) $(TEST_FILES) $(SRC_FILES) -o build/dev_test
	./build/dev_test