#include "../cheri_compressed_cap.h"
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "test_util.h"

#include "inputs.cpp"


static_assert(CC128_FIELD_OTYPE_START == 27, "");
static_assert(CC128_FIELD_OTYPE_SIZE == 18, "");
static_assert(CC128_FIELD_OTYPE_LAST == 44, "");

template <class T, std::size_t N>
constexpr inline size_t array_lengthof(T (&)[N]) {
  return N;
}

static bool check_fields_match(const cap_register_t& result, const test_input& ti, const std::string& prefix) {
        uint64_t top64 = (uint64_t)(result.cr_base + result._cr_length);
        unsigned __int128 top_full = result.cr_base + result._cr_length;
        bool top_bit65 = (top_full >> 64) & 1;

        bool success = check(ti.address, result.cr_base + result.cr_offset, prefix + "Wrong base");
        success = success && check(ti.bot, result.cr_base, prefix + "Wrong base");
        success = success && check(ti.otype, result.cr_otype, prefix + "Wrong otype");
        success = success && check(ti.permissions, result.cr_perms + (result.cr_uperms << 15), prefix + "Wrong perms");
        success = success && check(ti.top.u64, top64, prefix + "Wrong top");
        success = success && check(ti.top.bit65, top_bit65, prefix + "Wrong top bit 65");
        return success;
}

static bool test_one_entry(const test_input& ti) {
    // decompress_representable(ti.input.pesbt, ti.input.cursor);
    cap_register_t result;
    memset(&result, 0, sizeof(result));
    // The inputs already have the NULL xor mask removed -> decompress_128cap_already_xored()
    decompress_128cap_already_xored(ti.input.pesbt, ti.input.cursor, &result);

    bool success = check_fields_match(result, ti, "");
    if (!success) {
        fprintf(stderr, "Decompressed raw pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", ti.input.pesbt, ti.input.cursor);
        dump_cap_fields(result);
    }
    // If it was a valid input ensure that recompressing results in the same pesbt field:
    uint64_t reserved_bits = CC128_EXTRACT_FIELD(ti.input.pesbt, RESERVED);
    // success = success && check(UINT64_C(0), reserved_bits, "Reserved capability bits were non-zero");
    if (reserved_bits != 0) {
        // Set the PESBT field in the capreg so that the reserved bits can be added back to the capability
        // This is an invalid capability
    }


    // Now try recompressing and compare pesbt:
    {
        uint64_t recompressed_pesbt = compress_128cap_without_xor(&result);
        if (ti.input.pesbt != recompressed_pesbt) {
            fprintf(stderr, "Note: Recompressing resulted in different pesbt = 0x%016" PRIx64
                            ", original = 0x%016" PRIx64 ", xor = 0x%016" PRIx64 "\n",
                            recompressed_pesbt, ti.input.pesbt, recompressed_pesbt ^ ti.input.pesbt);
            // This is not an error since there are multiple ways of encoding the same bot/top values with different exponents
        }
        // But even if it didn't compress the same at least all fields decompressed from the new pesbt must be the same:
        cap_register_t result_recompressed;
        memset(&result_recompressed, 0, sizeof(result_recompressed));
        // The inputs already have the NULL xor mask removed -> decompress_128cap_already_xored()
        decompress_128cap_already_xored(ti.input.pesbt, ti.input.cursor, &result_recompressed);
        success = success && check_fields_match(result_recompressed, ti, "Recompressed pesbt: ");

        // It should now be normalized -> recompressing again should result in the same pesbt:
        uint64_t recompressed_pesbt_after_normalize = compress_128cap_without_xor(&result_recompressed);
        success = success && check(recompressed_pesbt, recompressed_pesbt_after_normalize, "Compressing normalized cap should result in same pesbt");
    }
    return success;
}

int main() {
    int failure_count = 0;
    for (size_t i = 0; i < array_lengthof(inputs); i++) {
        if (!test_one_entry(inputs[i])) {
            fprintf(stderr, "Failed at index %zd\n", i);
            failure_count++;
            continue;
        }
    }
    if (!failed)
        printf("\nAll tests passed!\n");
    else
        printf("\nFailure count %d!\n", failure_count);
    return failed ? 1 : 0;
}
