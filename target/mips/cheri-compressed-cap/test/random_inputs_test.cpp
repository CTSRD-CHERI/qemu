#include "../cheri_compressed_cap.h"
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

static bool failed = false;

#include "inputs.cpp"

template <typename T> static bool check(T expected, T actual, const char* msg) {
    if (expected == actual)
        return true;
    std::cerr << "ERROR: " << msg << ": 0x" << std::hex << actual << " != 0x" << expected << "\n";
    failed = true;
    return false;
}

#define __STRINGIFY(x) #x
#define _STRINGIFY(x) __STRINGIFY(x)
#define STRINGIFY(x) _STRINGIFY(x)

#define CHECK_FIELD(cap, field, expected) check((uint64_t)expected, (uint64_t)cap.cr_##field, "Line " STRINGIFY(__LINE__) ": " #field " is wrong")

static cap_register_t decompress_representable(uint64_t pesbt, uint64_t cursor) {
    cap_register_t result;
    fprintf(stderr, "Decompressing pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", pesbt, cursor);
    decompress_128cap(pesbt, cursor, &result);
    fprintf(stderr, "Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
    fprintf(stderr, "User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
    fprintf(stderr, "Base:        0x%" PRIx64 "\n", result.cr_base);
    fprintf(stderr, "Offset:      0x%" PRIx64 "\n", result.cr_offset);
    fprintf(stderr, "Length:      0x%" PRIx64 "\n", result.cr_length);
    fprintf(stderr, "Top:         0x%" PRIx64 "\n", result.cr_base + result.cr_length);
    fprintf(stderr, "Sealed:      %d\n", (int)cc128_is_cap_sealed(&result));
    fprintf(stderr, "OType:      0x%" PRIx32 "\n", result.cr_otype);
    fprintf(stderr, "\n");
    // Check that the result is the same again when compressed
    uint64_t new_pesbt = compress_128cap(&result);
    check(pesbt, new_pesbt, "recompressing resulted in different pesbt");
    check(cursor, result.cr_base + result.cr_offset, "recompressing resulted in different cursor");
    return result;
}

static_assert(CC128_FIELD_OTYPE_START == 27, "");
static_assert(CC128_FIELD_OTYPE_SIZE == 18, "");
static_assert(CC128_FIELD_OTYPE_LAST == 44, "");

template <class T, std::size_t N>
constexpr inline size_t array_lengthof(T (&)[N]) {
  return N;
}

bool test_one_entry(const test_input& ti) {
    // decompress_representable(ti.input.pesbt, ti.input.cursor);
    cap_register_t result;
    // The inputs already have the NULL xor mask removed -> decompress_128cap_already_xored()
    decompress_128cap_already_xored(ti.input.pesbt, ti.input.cursor, &result);
    uint64_t top64 = (uint64_t)(result.cr_base + result.cr_length);
    unsigned __int128 top_full = result.cr_base + result.cr_length;
    bool top_bit65 = (result.cr_base + result.cr_length) > UINT64_MAX;
    bool len_bit65 = result.cr_length > UINT64_MAX;

    bool success = check(ti.address, result.cr_base + result.cr_offset, "Wrong base");
    success = success && check(ti.bot, result.cr_base, "Wrong base");
    success = success && check(ti.otype, result.cr_otype, "Wrong otype");
    success = success && check(ti.permissions, result.cr_perms + (result.cr_uperms << 15), "Wrong perms");
    success = success && check(ti.top.u64, top64, "Wrong top");
    success = success && check(ti.top.bit65, top_bit65, "Wrong top bit 65");
    if (!success) {
        fprintf(stderr, "Decompressed raw pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", ti.input.pesbt, ti.input.cursor);
        fprintf(stderr, "Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
        fprintf(stderr, "User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
        fprintf(stderr, "Base:        0x%016" PRIx64 "\n", result.cr_base);
        fprintf(stderr, "Offset:      0x%016" PRIx64 "\n", result.cr_offset);
        fprintf(stderr, "Length:      0x%" PRIx64 "%016" PRIx64 "\n", (uint64_t)(result.cr_length >> 64), (uint64_t)result.cr_length);
        fprintf(stderr, "top:         0x%" PRIx64 "%016" PRIx64 "\n", (uint64_t)(top_full >> 64), (uint64_t)top_full);
        fprintf(stderr, "Sealed:      %d\n", (int)cc128_is_cap_sealed(&result));
        fprintf(stderr, "OType:      0x%" PRIx32 "\n", result.cr_otype);
        fprintf(stderr, "\n");
    }
    return success;
}

int main() {
    for (size_t i = 0; i < array_lengthof(inputs); i++) {
        if (!test_one_entry(inputs[i])) {
            fprintf(stderr, "Failed at index %zd\n", i);
            continue;
        }
    }
    return failed ? 1 : 0;
}
