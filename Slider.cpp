#include "Slider.hpp"

Slider::Slider()
{
}

Slider::~Slider()
{
}

void Slider::draw (Window &win)
{
    int width = getWidth();
    int height = getHeight();

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            win.addCharacter ('A', x, y);
        }
    }
}
