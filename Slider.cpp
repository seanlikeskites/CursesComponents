#include "Slider.hpp"

Slider::Slider (const std::string &nameInit)
    : name (nameInit),
      minValue (0.0), maxValue (1.0),
      position (0.0), value (0.0)
{
}

Slider::~Slider()
{
}

void Slider::draw (WINDOW *w)
{
    mvwaddch (w, 1, 1, 'A');
    wgetch (w);
}
