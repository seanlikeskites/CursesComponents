#include "Slider.hpp"
#include <cmath>
#include "MathsTools.hpp"

Slider::Slider (const std::string &nameInit)
    : name (nameInit),
      bottomValue (0.0), topValue (1.0), range (1.0),
      skewFactor (1.0),
      value (0.0), proportionOfLength (0.0),
      increment (0.1),
      sliderHeight (0)
{
}

Slider::~Slider()
{
}

void Slider::setRange (double newBottomValue, double newTopValue, double newSkewFactor)
{
    bottomValue = newBottomValue;
    topValue = newTopValue;

    if (skewFactor > 0.0)
    {
        skewFactor = newSkewFactor;
    }

    range = topValue - bottomValue;
    proportionOfLength = valueToProportionOfLength (value);
}

void Slider::setValue (double newValue)
{
    value = MathsTools::constrictValueToRange (newValue, bottomValue, topValue);
    proportionOfLength = valueToProportionOfLength (value);
    redraw();
}

double Slider::getValue() const
{
    return value;
}

double Slider::valueToProportionOfLength (double valueToConvert)
{
    return pow (((valueToConvert - bottomValue) / range), skewFactor);
}

double Slider::proportionOfLengthToValue (double valueToConvert)
{
    return range * pow (valueToConvert, (1.0 / skewFactor)) + bottomValue;
}

void Slider::draw (Window &win)
{
    int width = getWidth();
    int height = getHeight();

    int nameStart = (width - 1 - name.size()) / 2;

    if (nameStart < 0)
    {
        nameStart = 0;
    }

    win.printString (name, nameStart, height - 1);
    win.printDouble (value, nameStart, height - 2);
}

void Slider::resized()
{
    int height = getHeight();   
    sliderHeight = height - 4;

    increment = range / sliderHeight;
}
