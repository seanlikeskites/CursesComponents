test: main.cpp Slider.cpp Component.cpp
	clang++ main.cpp Slider.cpp Component.cpp -o test -std=c++11 -lcurses
