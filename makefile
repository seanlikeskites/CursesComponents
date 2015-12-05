test: main.cpp Curses.cpp Component.cpp Slider.cpp
	clang++ main.cpp Curses.cpp Component.cpp Slider.cpp -o test -std=c++11 -lcurses -lpanel -Wall -g
