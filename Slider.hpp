#ifndef SLIDER_HPP_INCLUDED
#define SLIDER_HPP_INCLUDED

#include "Component.hpp"
#include <string>
#include "RangedValue.hpp"

class Slider : public Component
{
public:
    Slider (const std::string &nameInit);
    ~Slider();

    void setRange (double bottomValue, double topValue, double newSkewFactor = 1.0);

    void setValue (double newValue);
    void setProportionOfLength (double newProportionOfLength);
    void incrementValue();
    void decrementValue();
    double getValue() const;

    double valueToProportionOfLength (double valueToConvert);
    double proportionOfLengthToValue (double valueToConvert);

    void keyPressed (int key) override;

private:
    std::string name;

    RangedValue <double> value;
    double skewFactor;
    double proportionOfLength;
    double increment;

    int sliderHeight;

    void draw (Window &win) override;
    void resized() override;
};

#endif // SLIDER_HPP_INCLUDED
