#ifndef RANGED_VALUE_HPP_INCLUDED
#define RANGED_VALUE_HPP_INCLUDED

#include <algorithm>

/** A class which represents a value constricted to a certain range. */
template <typename T>
class RangedValue
{
public:
    /** Constructor
     *
     *  @param valueInit the initial value
     *  @param bottomValueInit one of the range boundaries
     *  @param topValueInit the other range boundary
     */
    RangedValue (T valueInit, T bottomValueInit, T topValueInit)
        : value (0),
          minValue (std::min (bottomValueInit, topValueInit)),
          maxValue (std::max (bottomValueInit, topValueInit)),
          bottomValue (bottomValueInit),
          topValue (topValueInit)
    {
        operator= (valueInit);
    }

    /** Copy Constructor */
    RangedValue (const RangedValue &other) = default;
    /** Copy Assignment Operator */
    RangedValue& operator= (const RangedValue &rhs) = default;
    /** Destructor */
    ~RangedValue() = default;

    /** Set the value. 
     *
     *  @param rhs the new value
     */
    RangedValue& operator= (T rhs)
    {
        if (rhs <= minValue)
        {
            value = minValue;
        }
        else if (rhs >= maxValue)
        {
            value = maxValue;
        }
        else
        {
            value = rhs;
        }

        return *this;
    }

    /** Implicit cast to the underlying type. */
    operator T() const
    {
        return value;
    }

    /** Increment the value. 
     *
     *  @param rhs the value to increment by
     */
    RangedValue& operator+= (const T &rhs)
    {
        operator= (value + rhs);
        return *this;
    }

    /** Decrement the value. 
     *
     *  @param rhs the value to decrement by
     */
    RangedValue& operator-= (const T &rhs)
    {
        operator= (value - rhs);
        return *this;
    }

    /** Compound multiplication assignment.
     *
     *  @param rhs the value to multiply by
     */
    RangedValue& operator*= (const T &rhs)
    {
        operator= (value * rhs);
        return *this;
    }

    /** Compound division assignment.
     *
     *  @param rhs the value to divide by
     */
    RangedValue& operator/= (const T &rhs)
    {
        operator= (value / rhs);
        return *this;
    }

    /** Set the range.
     *
     *  @param newBottomValue one of the new range boundaries
     *  @param newTopValue the other new range boundary
     */
    void setRange (T newBottomValue, T newTopValue)
    {
          minValue = std::min (newBottomValue, newTopValue);
          maxValue = std::max (newBottomValue, newTopValue);
          topValue = newTopValue;
          bottomValue = newBottomValue;
    }

    /** Returns the range. */
    T getRange() const
    {
        return topValue - bottomValue;
    }

    /** Returns the lowest end of the range. */
    T getMinValue() const
    {
        return minValue;
    }

    /** Returns the highest end of the range. */
    T getMaxValue() const
    {
        return maxValue;
    }

    /** Returns the bottom end of the range. */
    T getBottomValue() const
    {
        return bottomValue;
    }

    /** Returns the top end of the range. */
    T getTopValue() const
    {
        return topValue;
    }

private:
    T value;
    T minValue, maxValue;
    T bottomValue, topValue;
};

#endif // RANGED_VALUE_HPP_INCLUDED
