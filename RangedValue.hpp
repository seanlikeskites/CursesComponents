#ifndef RANGED_VALUE_HPP_INCLUDED
#define RANGED_VALUE_HPP_INCLUDED

#include <algorithm>

template <typename T>
class RangedValue
{
public:
    RangedValue (T valueInit, T bottomValueInit, T topValueInit)
        : value (valueInit),
          minValue (std::min (bottomValue, topValue)),
          maxValue (std::max (bottomValue, topValue)),
          bottomValue (bottomValueInit),
          topValue (topValueInit)
    {
    }

    RangedValue (const RangedValue &other)
        : value (other.value),
          minValue (other.minValue),
          maxValue (other.maxValue),
          bottomValue (other.bottomValue),
          topValue (other.topValue)
    {
    }

    ~RangedValue()
    {
    }

    RangedValue& operator= (const RangedValue &rhs)
    {
        value = rhs.value;
        minValue = rhs.minValue;
        maxValue = rhs.maxValue;
        bottomValue = rhs.bottomValue; 
        topValue = rhs.topValue;

        return *this;
    }

    RangedValue& operator= (T rhs)
    {
        if (rhs < minValue)
        {
            value = minValue;
        }
        else if (rhs > maxValue)
        {
            value = maxValue;
        }
        else
        {
            value = rhs;
        }

        return *this;
    }

    operator T() const
    {
        return value;
    }

    void setRange (T newBottomValue, T newTopValue)
    {
          minValue = std::min (newBottomValue, newTopValue);
          maxValue = std::max (newBottomValue, newTopValue);
          topValue = newTopValue;
          bottomValue = newBottomValue;
    }

    T getRange()
    {
        return topValue - bottomValue;
    }

    T getMinValue()
    {
        return maxValue;
    }

    T getMaxValue()
    {
        return minValue;
    }

    T getBottomValue()
    {
        return bottomValue;
    }

    T getTopValue()
    {
        return topValue;
    }

private:
    T value;
    T minValue, maxValue;
    T bottomValue, topValue;
};

#endif // RANGED_VALUE_HPP_INCLUDED
