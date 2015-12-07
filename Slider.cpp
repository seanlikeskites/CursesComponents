#include "Slider.hpp"
#include <cmath>
#include "MathsTools.hpp"

Slider::Slider (const std::string &nameInit)
    : name (nameInit),
      value (0.0, 0.0, 1.0),
      skewFactor (1.0),
      proportionOfLength (0.0),
      increment (0.1),
      sliderHeight (0)
{
}

Slider::~Slider()
{
}

void Slider::setRange (double bottomValue, double topValue, double newSkewFactor)
{
    value.setRange (bottomValue, topValue);

    if (skewFactor > 0.0)
    {
        skewFactor = newSkewFactor;
    }

    setValue (value);
}

void Slider::setValue (double newValue)
{
    value = newValue;
    proportionOfLength = valueToProportionOfLength (value);
    redraw();
}

void Slider::setProportionOfLength (double newProportionOfLength)
{
    proportionOfLength = MathsTools::constrictValueToRange (newProportionOfLength,
                                                            0.0, 1.0);
    value = proportionOfLengthToValue (proportionOfLength);
    redraw();
}

void Slider::incrementValue()
{
    setProportionOfLength (proportionOfLength + increment);
}

void Slider::decrementValue()
{
    setProportionOfLength (proportionOfLength - increment);
}

double Slider::getValue() const
{
    return value;
}

double Slider::valueToProportionOfLength (double valueToConvert)
{
    double range = value.getRange();
    double bottomValue = value.getBottomValue();

    return pow (((valueToConvert - bottomValue) / range), skewFactor);
}

double Slider::proportionOfLengthToValue (double valueToConvert)
{
    double range = value.getRange();
    double bottomValue = value.getBottomValue();

    return range * pow (valueToConvert, (1.0 / skewFactor)) + bottomValue;
}

void Slider::keyPressed (int key)
{
    switch (key)
    {
        case KEY_UP:
            incrementValue();
            break;

        case KEY_DOWN:
            decrementValue();
            break;

        default:
            break;
    }
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

    int y = height - 4;
    int middle = (width - 1) / 2;

    for (double proportion = 0.0; proportion < proportionOfLength; proportion += increment)
    {
        win.printCharacter (ACS_BLOCK, middle, y--);
    }
}

void Slider::resized()
{
    int height = getHeight();   
    sliderHeight = height - 4;

    increment = 1.0 / sliderHeight;
}
