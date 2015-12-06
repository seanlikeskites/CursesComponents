#ifndef SLIDER_HPP_INCLUDED
#define SLIDER_HPP_INCLUDED

#include "Component.hpp"

class Slider : public Component
{
public:
    Slider (const std::string &nameInit);
    ~Slider();

    void setRange (double newBottomValue, double newTopValue, double newSkewFactor = 1.0);

    void setValue (double newValue);
    double getValue() const;

    double valueToProportionOfLength (double valueToConvert);
    double proportionOfLengthToValue (double valueToConvert);

private:
    std::string name;

    double bottomValue, topValue, range;
    double skewFactor;
    double value, proportionOfLength;
    double increment;

    int sliderHeight;

    void draw (Window &win) override;
    void resized() override;
};

#endif // SLIDER_HPP_INCLUDED
