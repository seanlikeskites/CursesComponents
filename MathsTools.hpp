#ifndef MATHS_TOOLS_HPP_INCLUDED
#define MATHS_TOOLS_HPP_INCLUDED

/** Some useful maths functions. */
struct MathsTools
{
    /** A generic implementation of the sign function.
     *
     *  @param value the input value
     *
     *  Returns -1 for a negative input, 0 for a 0 input and 1 for a positive input.
     */
    template <typename T>
    static int sign (T value)
    {
        return sign (value, std::is_signed <T>());
    }

    /** Constrict a value to a given range.
     *
     *  @param value the input value
     *  @param bottomValue one of the range boundaries
     *  @param topValue the other range boundary
     */
    template <typename T>
    static T constrictValueToRange (T value, T bottomValue, T topValue)
    {
        T maxValue = std::max (topValue, bottomValue);
        T minValue = std::min (topValue, bottomValue);

        if (value >= maxValue)
        {
            return maxValue;
        }
        else if (value <= minValue)
        {
            return minValue;
        }
        else 
        {
            return value;
        }
    }

private:
    template <typename T>
    static int sign (T value, std::true_type)
    {
        return (T (0) < value) - (value < T (0));
    }

    template <typename T>
    static int sign (T value, std::false_type)
    {
        return T (0) < value;
    }
};

#endif // MATHS_TOOLS_HPP_INCLUDED
