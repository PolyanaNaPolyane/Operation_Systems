#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <iostream>
#include <limits>

enum class MathStatus : int
{
    Ok = 0,
    Overflow = 1,
    DivisionByZero = 2,
    NullPointer = 3
};

using BinaryOp = MathStatus(*)(std::int64_t, std::int64_t, std::int64_t*);

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
    HMODULE module = LoadLibraryW(L"SafeMathLib.dll");
    if (module == nullptr)
    {
        std::cerr << "Cannot load SafeMathLib.dll\n";
        return 1;
    }

    const auto add = reinterpret_cast<BinaryOp>(GetProcAddress(module, "AddInt64"));
    const auto sub = reinterpret_cast<BinaryOp>(GetProcAddress(module, "SubInt64"));
    const auto mul = reinterpret_cast<BinaryOp>(GetProcAddress(module, "MulInt64"));
    const auto div = reinterpret_cast<BinaryOp>(GetProcAddress(module, "DivInt64"));

    if (add == nullptr || sub == nullptr || mul == nullptr || div == nullptr)
    {
        std::cerr << "Cannot resolve function pointers from SafeMathLib.dll\n";
        FreeLibrary(module);
        return 2;
    }

    std::int64_t out = 0;
    MathStatus status = MathStatus::Ok;

    status = add(100, 25, &out);
    PrintResult("Add 100 + 25", status, out);
    status = add(std::numeric_limits<std::int64_t>::max(), 1, &out);
    PrintResult("Add max + 1", status, out);

    status = sub(100, 25, &out);
    PrintResult("Sub 100 - 25", status, out);
    status = sub(std::numeric_limits<std::int64_t>::min(), 1, &out);
    PrintResult("Sub min - 1", status, out);

    status = mul(12, 12, &out);
    PrintResult("Mul 12 * 12", status, out);
    status = mul(std::numeric_limits<std::int64_t>::max(), 2, &out);
    PrintResult("Mul max * 2", status, out);

    status = div(81, 9, &out);
    PrintResult("Div 81 / 9", status, out);
    status = div(81, 0, &out);
    PrintResult("Div 81 / 0", status, out);

    FreeLibrary(module);
    return 0;
}
