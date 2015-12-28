SOURCES = main.cpp Curses.cpp Component.cpp Slider.cpp Timer.cpp
OBJECTS = $(subst .cpp,.o, $(SOURCES))
CXX = clang++
CXXFLAGS = -std=c++14 -Wall -g
LIBS = -lcurses -lpthread

all: test

%.o: %.cpp
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
	@echo \*\* Compiling $<
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
	$(CXX) $(CXXFLAGS) -c $<
	@echo 

test: $(OBJECTS)
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
	@echo \*\* Linking $@
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
	$(CXX) -o $@ $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS) test
