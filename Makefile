# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra -O2

# Libraries
LIBS = -lcurl

# Files and directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Source files and object files
SRCS = main.cpp $(SRC_DIR)/container.cpp $(SRC_DIR)/network.cpp $(SRC_DIR)/event_handler.cpp
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = program

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
