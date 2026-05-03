#include <iostream>
#include <iomanip>
#include <cstdint>
#include <windows.h>

typedef int      (*PFN_RSA_GenerateKeys)(uint64_t, uint64_t, uint64_t*, uint64_t*, uint64_t*);
typedef uint64_t (*PFN_RSA_Encrypt)     (uint64_t, uint64_t, uint64_t);
typedef uint64_t (*PFN_RSA_Decrypt)     (uint64_t, uint64_t, uint64_t);

static void print_keys(const char* label,
                       uint64_t e, uint64_t d, uint64_t n)
{
    std::cout << label << ":\n"
              << "  n = " << n << "\n"
              << "  e = " << e << "  (public)\n"
              << "  d = " << d << "  (private)\n";
}

int main()
{
    HMODULE hLib = LoadLibraryW(L"RSADynamicLibrary.dll");
    if (!hLib)
    {
        std::cerr << "LoadLibrary failed, error: " << GetLastError() << "\n";
        return 1;
    }

    auto fnGenerateKeys = reinterpret_cast<PFN_RSA_GenerateKeys>(
        GetProcAddress(hLib, "RSA_GenerateKeys"));
    auto fnEncrypt = reinterpret_cast<PFN_RSA_Encrypt>(
        GetProcAddress(hLib, "RSA_Encrypt"));
    auto fnDecrypt = reinterpret_cast<PFN_RSA_Decrypt>(
        GetProcAddress(hLib, "RSA_Decrypt"));

    if (!fnGenerateKeys || !fnEncrypt || !fnDecrypt)
    {
        std::cerr << "GetProcAddress failed, error: " << GetLastError() << "\n";
        FreeLibrary(hLib);
        return 1;
    }

    const uint64_t p0 = 4294967291ULL;
    const uint64_t q0 = 4294967279ULL;

    const uint64_t p1 = 1000000007ULL;
    const uint64_t q1 =  998244353ULL;

    uint64_t e0 = 0, d0 = 0, n0 = 0;
    uint64_t e1 = 0, d1 = 0, n1 = 0;

    if (fnGenerateKeys(p0, q0, &e0, &d0, &n0) != 0)
    {
        std::cerr << "[ERROR] Key-pair 0 generation failed.\n";
        FreeLibrary(hLib);
        return 1;
    }
    if (fnGenerateKeys(p1, q1, &e1, &d1, &n1) != 0)
    {
        std::cerr << "[ERROR] Key-pair 1 generation failed.\n";
        FreeLibrary(hLib);
        return 1;
    }

    print_keys("Key pair 0  (p0, q0 near 2^32)", e0, d0, n0);
    std::cout << "\n";
    print_keys("Key pair 1  (p1 = 10^9+7, q1 = 998244353)", e1, d1, n1);
    std::cout << "\n";

    const uint64_t testData[] =
    {
        1ULL,
        42ULL,
        1000ULL,
        65536ULL,
        123456789ULL,
        987654321ULL,
        9999999999ULL,
        123456789012345ULL,
    };
    const int dataCount = static_cast<int>(sizeof(testData) / sizeof(testData[0]));

    const uint64_t nMin = (n0 < n1) ? n0 : n1;

    std::cout << std::string(70, '-') << "\n";
    std::cout << "Running encrypt/decrypt chain for " << dataCount << " test values...\n";
    std::cout << std::string(70, '-') << "\n";

    int errors = 0;

    for (int i = 0; i < dataCount; ++i)
    {
        const uint64_t t = testData[i];

        if (t == 0 || t >= nMin)
        {
            std::cout << "  [SKIP] t[" << i << "] = " << t
                      << " (out of valid range)\n";
            continue;
        }

        const uint64_t e1t = fnEncrypt(t, e1, n1);

        const uint64_t d1e1t = fnDecrypt(e1t, d1, n1);
        if (d1e1t != t)
        {
            std::cerr << "  [ERROR] t[" << i << "] = " << t
                      << ": D1(E1(t)) = " << d1e1t << " != t\n";
            ++errors;
            continue;
        }

        const uint64_t e0d1e1t = fnEncrypt(d1e1t, e0, n0);

        const uint64_t d0e0d1e1t = fnDecrypt(e0d1e1t, d0, n0);
        if (d0e0d1e1t != t)
        {
            std::cerr << "  [ERROR] t[" << i << "] = " << t
                      << ": D0(E0(D1(E1(t)))) = " << d0e0d1e1t << " != t\n";
            ++errors;
            continue;
        }

        std::cout << "  [OK] t[" << i << "] = " << std::setw(18) << t
                  << "  E1(t)=" << std::setw(20) << e1t
                  << "  E0(D1(E1(t)))=" << std::setw(20) << e0d1e1t << "\n";
    }

    std::cout << std::string(70, '-') << "\n";
    if (errors == 0)
        std::cout << "All tests PASSED.\n";
    else
        std::cerr << errors << " test(s) FAILED.\n";

    FreeLibrary(hLib);
    return (errors == 0) ? 0 : 1;
}

