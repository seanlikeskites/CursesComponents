SOURCES = main.cpp Curses.cpp Component.cpp Slider.cpp
OBJECTS = $(subst .cpp,.o, $(SOURCES))
CXX = clang++
CXXFLAGS = -std=c++11 -Wall -g
LIBS = -lpanel -lcurses

all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

test: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) test
