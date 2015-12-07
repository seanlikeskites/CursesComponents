#ifndef RANGED_VALUE_HPP_INCLUDED
#define RANGED_VALUE_HPP_INCLUDED

#include <algorithm>

template <typename T>
class RangedValue
{
public:
    RangedValue (T valueInit, T bottomValueInit, T topValueInit)
        : value (0),
          minValue (std::min (bottomValueInit, topValueInit)),
          maxValue (std::max (bottomValueInit, topValueInit)),
          bottomValue (bottomValueInit),
          topValue (topValueInit)
    {
        operator= (valueInit);
    }

    RangedValue (const RangedValue &other) = default;
    RangedValue& operator= (const RangedValue &rhs) = default;
    ~RangedValue() = default;

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
