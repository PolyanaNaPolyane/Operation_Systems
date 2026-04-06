#pragma once

#include <cstdint>

#ifdef SAFEMATHLIB_EXPORTS
#define SAFEMATH_API extern "C" __declspec(dllexport)
#else
#define SAFEMATH_API extern "C" __declspec(dllimport)
#endif

enum class MathStatus : int
{
    Ok = 0,
    Overflow = 1,
    DivisionByZero = 2,
    NullPointer = 3
};

SAFEMATH_API MathStatus AddInt64(std::int64_t a, std::int64_t b, std::int64_t* result);
SAFEMATH_API MathStatus SubInt64(std::int64_t a, std::int64_t b, std::int64_t* result);
SAFEMATH_API MathStatus MulInt64(std::int64_t a, std::int64_t b, std::int64_t* result);
SAFEMATH_API MathStatus DivInt64(std::int64_t a, std::int64_t b, std::int64_t* result);
