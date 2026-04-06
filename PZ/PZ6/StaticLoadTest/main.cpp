#include <iostream>
#include <limits>

#include "../SafeMathLib/SafeMathLib.h"

const char* StatusToText(MathStatus status)
{
    switch (status)
    {
    case MathStatus::Ok:
        return "Ok";
    case MathStatus::Overflow:
        return "Overflow";
    case MathStatus::DivisionByZero:
        return "DivisionByZero";
    case MathStatus::NullPointer:
        return "NullPointer";
    default:
        return "Unknown";
    }
}

void PrintResult(const char* opName, MathStatus status, std::int64_t value)
{
    std::cout << opName << " -> " << StatusToText(status);
    if (status == MathStatus::Ok)
    {
        std::cout << ", result = " << value;
    }

    std::cout << '\n';
}

int main()
{
    std::int64_t out = 0;
    MathStatus status = MathStatus::Ok;

    status = AddInt64(40, 2, &out);
    PrintResult("Add 40 + 2", status, out);
    status = AddInt64(std::numeric_limits<std::int64_t>::max(), 1, &out);
    PrintResult("Add max + 1", status, out);

    status = SubInt64(40, 2, &out);
    PrintResult("Sub 40 - 2", status, out);
    status = SubInt64(std::numeric_limits<std::int64_t>::min(), 1, &out);
    PrintResult("Sub min - 1", status, out);

    status = MulInt64(6, 7, &out);
    PrintResult("Mul 6 * 7", status, out);
    status = MulInt64(std::numeric_limits<std::int64_t>::max(), 2, &out);
    PrintResult("Mul max * 2", status, out);

    status = DivInt64(84, 2, &out);
    PrintResult("Div 84 / 2", status, out);
    status = DivInt64(84, 0, &out);
    PrintResult("Div 84 / 0", status, out);

    return 0;
}
