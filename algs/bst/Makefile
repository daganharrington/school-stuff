CXXFLAGS = -g -Wall -std=c++0x
CXX = g++
DEPS = tree.h
SOURCES = tree.cpp
TARGET = tree
OBJS = tree.o

all: $(TARGET)

$(TARGET): $(SOURCES) $(DEPS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean: 
	rm -rf *.o