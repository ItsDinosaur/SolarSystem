CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS =

LDLIBS = -lGL -lGLU -lglut -lm

SRCS = main.cpp body.cpp camera.cpp physics.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = solar_system

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp body.h camera.h physics.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
