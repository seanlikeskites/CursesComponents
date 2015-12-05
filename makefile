test: main.cpp Curses.cpp
	clang++ main.cpp Curses.cpp -o test -std=c++11 -lcurses -Wall
