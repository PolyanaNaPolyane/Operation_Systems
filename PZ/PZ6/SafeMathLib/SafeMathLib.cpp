#include "SafeMathLib.h"

#include <limits>

namespace
{
    bool IsMulOverflow(std::int64_t a, std::int64_t b)
    {
        if (a == 0 || b == 0)
        {
            return false;
        }

        if (a == -1)
        {
            return b == std::numeric_limits<std::int64_t>::min();
        }

        if (b == -1)
        {
            return a == std::numeric_limits<std::int64_t>::min();
        }

        const auto maxValue = std::numeric_limits<std::int64_t>::max();
        const auto minValue = std::numeric_limits<std::int64_t>::min();

        if (a > 0)
        {
            if (b > 0)
            {
                return a > maxValue / b;
            }

            return b < minValue / a;
        }

        if (b > 0)
        {
            return a < minValue / b;
        }

        return a < maxValue / b;
    }
}

MathStatus AddInt64(std::int64_t a, std::int64_t b, std::int64_t* result)
{
    if (result == nullptr)
    {
        return MathStatus::NullPointer;
    }

    if ((b > 0 && a > std::numeric_limits<std::int64_t>::max() - b) ||
        (b < 0 && a < std::numeric_limits<std::int64_t>::min() - b))
    {
        return MathStatus::Overflow;
    }

    *result = a + b;
    return MathStatus::Ok;
}

MathStatus SubInt64(std::int64_t a, std::int64_t b, std::int64_t* result)
{
    if (result == nullptr)
    {
        return MathStatus::NullPointer;
    }

    if ((b < 0 && a > std::numeric_limits<std::int64_t>::max() + b) ||
        (b > 0 && a < std::numeric_limits<std::int64_t>::min() + b))
    {
        return MathStatus::Overflow;
    }

    *result = a - b;
    return MathStatus::Ok;
}

MathStatus MulInt64(std::int64_t a, std::int64_t b, std::int64_t* result)
{
    if (result == nullptr)
    {
        return MathStatus::NullPointer;
    }

    if (IsMulOverflow(a, b))
    {
        return MathStatus::Overflow;
    }

    *result = a * b;
    return MathStatus::Ok;
}

MathStatus DivInt64(std::int64_t a, std::int64_t b, std::int64_t* result)
{
    if (result == nullptr)
    {
        return MathStatus::NullPointer;
    }

    if (b == 0)
    {
        return MathStatus::DivisionByZero;
    }

    if (a == std::numeric_limits<std::int64_t>::min() && b == -1)
    {
        return MathStatus::Overflow;
    }

    *result = a / b;
    return MathStatus::Ok;
}
