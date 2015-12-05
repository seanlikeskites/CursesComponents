#ifndef SLIDER_HPP_INCLUDED
#define SLIDER_HPP_INCLUDED

#include "Component.hpp"

class Slider : public Component
{
public:
    Slider();
    ~Slider();

private:
    void draw (Window &win);
};

#endif // SLIDER_HPP_INCLUDED
