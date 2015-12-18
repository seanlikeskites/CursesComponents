SOURCES = main.cpp Curses.cpp Component.cpp Slider.cpp Timer.cpp
OBJECTS = $(subst .cpp,.o, $(SOURCES))
CXX = clang++
CXXFLAGS = -std=c++14 -Wall -g
LIBS = -lpanel -lcurses -lpthread

all: test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

test: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) test
