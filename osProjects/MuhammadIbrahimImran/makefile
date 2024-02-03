# Makefile for memsim.cpp and VirtualMemorySimulator.cpp

# Compiler options
CXX = g++
CXXFLAGS = -std=c++11

# Source files
SRCS = memsim.cpp VirtualMemorySimulator.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = memsim

# Compile rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Object file rules
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET) *.o

# Default rule
all: clean $(TARGET)
