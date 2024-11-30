# Universal Makefile for compiling .cpp files
# Compiler
CXX = g++

# Directories
SRC_DIR = src
BUILD_DIR = build

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -O2 -I$(SRC_DIR)

# Find all .cpp files in the source directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding .exe executables in build directory
EXECUTABLES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.exe,$(SOURCES))

# Default target: create build directory and compile executables
all: $(BUILD_DIR) $(EXECUTABLES)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build each executable
$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.cpp $(SRC_DIR)/rf24_config.h
	$(CXX) $(CXXFLAGS) $< -o $@ -lrf24

# Clean up compiled files
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
