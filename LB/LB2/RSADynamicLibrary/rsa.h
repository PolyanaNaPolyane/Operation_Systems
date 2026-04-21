#pragma once
#include <cstdint>

#ifdef RSADYNAMICLIBRARY_EXPORTS
#define RSA_API __declspec(dllexport)
#else
#define RSA_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    RSA_API int RSA_GenerateKeys(uint64_t p, uint64_t q,
                                 uint64_t* e, uint64_t* d, uint64_t* n);
    RSA_API uint64_t RSA_Encrypt(uint64_t data, uint64_t e, uint64_t n);
    RSA_API uint64_t RSA_Decrypt(uint64_t data, uint64_t d, uint64_t n);

#ifdef __cplusplus
}
#endif
