#ifndef MATHS_TOOLS_HPP_INCLUDED
#define MATHS_TOOLS_HPP_INCLUDED

struct MathsTools
{
    template <typename T>
    static int sign (T value)
    {
        return (T (0) < value) - (value < T (0));
    }

    template <typename T>
    static T constrictValueToRange (T value, T bottomValue, T topValue)
    {
        T maxValue = std::max (topValue, bottomValue);
        T minValue = std::min (topValue, bottomValue);

        if (value > maxValue)
        {
            return maxValue;
        }
        else if (value < minValue)
        {
            return minValue;
        }
        else 
        {
            return value;
        }
    }
};

#endif // MATHS_TOOLS_HPP_INCLUDED
