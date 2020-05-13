#include "../cheri_compressed_cap.h"
#ifdef __cplusplus
extern "C" {
#endif
void sail_decode_128_mem(uint64_t pesbt, uint64_t cursor, bool tag, cap_register_t* cdp);
void sail_decode_128_raw(uint64_t pesbt, uint64_t cursor, bool tag, cap_register_t* cdp);
struct cc128_bounds_bits sail_extract_bounds_bits_128(uint64_t pesbt);
uint64_t sail_compress_128_raw(const cap_register_t* csp);
uint64_t sail_compress_128_mem(const cap_register_t* csp);

uint64_t sail_null_pesbt(void);
#ifdef __cplusplus
}
#endif
