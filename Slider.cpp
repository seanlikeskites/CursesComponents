#include "Slider.hpp"

Slider::Slider()
{
}

Slider::~Slider()
{
}

void Slider::draw (Window &win)
{
    win.addCharacter ('A', 0, 0);
    win.setColours (Curses::Colour::red, Curses::Colour::blue);
    win.addCharacter ('B', 1, 1);
}
