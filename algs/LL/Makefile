CXXFLAGS = -g -Wall -std=c++0x
CXX = g++
DEPS = LL.h
SOURCES = LL.cpp
TARGET = LL
OBJS = LL.o

all: $(TARGET)

$(TARGET): $(SOURCES) $(DEPS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o