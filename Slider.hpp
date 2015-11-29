#ifndef SLIDER_HPP_INCLUDED
#define SLIDER_HPP_INCLUDED

#include <string>

#include "Component.hpp"

class Slider : public Component
{
public:
    Slider (const std::string &nameInit);
    ~Slider();

private:
    std::string name;

    double minValue, maxValue;
    double position, value;

    int height;

    void draw (WINDOW *w) override;
};

#endif // SLIDER_HPP_INCLUDED
