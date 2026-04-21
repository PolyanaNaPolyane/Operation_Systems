#include "pch.h"
#include "rsa.h"
#include <intrin.h>
#include <cstdint>

static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t hi = 0;
    uint64_t lo = _umul128(a, b, &hi);
    if (hi == 0)
        return lo % m;
    uint64_t rem = 0;
    _udiv128(hi, lo, m, &rem);
    return rem;
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t m)
{
    if (m == 1) return 0;
    uint64_t result = 1;
    base %= m;
    while (exp > 0)
    {
        if (exp & 1)
            result = mulmod(result, base, m);
        base = mulmod(base, base, m);
        exp >>= 1;
    }
    return result;
}

static uint64_t gcd(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

static uint64_t mod_inverse(uint64_t a, uint64_t m)
{
    if (m <= 1 || a == 0) return 0;
    a %= m;
    if (a == 0) return 0;

    uint64_t r0 = m, r1 = a;
    uint64_t t0 = 0, t1 = 1;

    while (r1 != 0)
    {
        uint64_t q  = r0 / r1;
        uint64_t r2 = r0 - q * r1;

        uint64_t qt1 = mulmod(q, t1, m);
        uint64_t t2  = (t0 >= qt1) ? (t0 - qt1) : (m - qt1 + t0);

        r0 = r1; t0 = t1;
        r1 = r2; t1 = t2;
    }

    return (r0 == 1) ? t0 : 0;
}

RSA_API int RSA_GenerateKeys(uint64_t p, uint64_t q,
                             uint64_t* e, uint64_t* d, uint64_t* n)
{
    if (!e || !d || !n) return -1;
    if (p < 3 || q < 3 || p == q) return -1;

    *n = p * q;
    uint64_t phi = (p - 1) * (q - 1);

    *e = 65537ULL;
    if (*e >= phi || gcd(*e, phi) != 1)
    {
        *e = 3;
        while (*e < phi && gcd(*e, phi) != 1)
            *e += 2;
        if (*e >= phi) return -1;
    }

    *d = mod_inverse(*e, phi);
    if (*d == 0) return -1;

    return 0;
}

RSA_API uint64_t RSA_Encrypt(uint64_t data, uint64_t e, uint64_t n)
{
    return mod_pow(data, e, n);
}

RSA_API uint64_t RSA_Decrypt(uint64_t data, uint64_t d, uint64_t n)
{
    return mod_pow(data, d, n);
}
