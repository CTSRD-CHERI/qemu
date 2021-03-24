#include "../cheri_compressed_cap.h"
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "sail_wrapper.h"
#include "test_util.h"

#include "decode_inputs.cpp"

#ifndef CC_IS_MORELLO

static_assert(CC128_FIELD_OTYPE_START == 27, "");
static_assert(CC128_FIELD_OTYPE_SIZE == 18, "");
static_assert(CC128_FIELD_OTYPE_LAST == 44, "");

#endif

#define CHECK_AND_SAVE_SUCCESS(expr)                                                                                   \
    do {                                                                                                               \
        CHECK(expr);                                                                                                   \
        if (!(expr)) {                                                                                                 \
            success = false;                                                                                           \
        }                                                                                                              \
    } while (false)

template <class Handler, typename test_input>
static bool check_fields_match(const typename Handler::cap_t& result, const test_input& input,
                               const std::string& prefix) {
    typename Handler::cap_t sail_result;
    memset(&sail_result, 0, sizeof(sail_result));
    Handler::sail_decode_raw(input.pesbt, input.cursor, false, &sail_result);

    typename Handler::bounds_bits bounds_bits = Handler::extract_bounds_bits(input.pesbt);
    typename Handler::bounds_bits sail_bounds_bits = Handler::sail_extract_bounds_bits(input.pesbt);
    REQUIRE(sail_bounds_bits == bounds_bits);

    CAPTURE(bounds_bits);
    CAPTURE(sail_result);
    CAPTURE(result);
    // TODO: CHECK(sail_result._cr_top >= sail_result.cr_base);
    // TODO: CHECK(result._cr_top >= result.cr_base);

    bool success = true;
    CAPTURE(prefix);

    CHECK_AND_SAVE_SUCCESS(sail_result._cr_cursor == result._cr_cursor);
    CHECK_AND_SAVE_SUCCESS(sail_result._cr_top == result._cr_top);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_base == result.cr_base);
    CHECK_AND_SAVE_SUCCESS(sail_result.cached_pesbt == result.cached_pesbt);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_flags == result.cr_flags);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_otype == result.cr_otype);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_perms == result.cr_perms);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_reserved == result.cr_reserved);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_tag == result.cr_tag);
    CHECK_AND_SAVE_SUCCESS(sail_result.cr_uperms == result.cr_uperms);

    // Since we are parsing arbitrary bit patterns, the length can be negative.
    // For the CRRL/CRAM check we only look at the low 64 bits of length.
#ifndef CC_IS_MORELLO
    typename Handler::addr_t len_truncated = (typename Handler::addr_t)result.length();
    typename Handler::addr_t rep_len = Handler::representable_length(len_truncated);
    if (rep_len != 0) {
        REQUIRE(rep_len >= len_truncated); // representable length wraps to 0 for >= 2^64
    }
    // Check that CRRL is idempotent:
    CHECK_AND_SAVE_SUCCESS(rep_len == Handler::representable_length(rep_len));
    // And equal to sail:
    CHECK_AND_SAVE_SUCCESS(rep_len == Handler::sail_representable_length(len_truncated));
    CHECK_AND_SAVE_SUCCESS(rep_len == Handler::sail_representable_length(rep_len));
    // Finally check CRAM:
    typename Handler::addr_t rep_mask = Handler::representable_mask(len_truncated);
    CHECK_AND_SAVE_SUCCESS(rep_mask == Handler::sail_representable_mask(len_truncated));
#endif

    return success;
}

template <class Handler, typename test_input> static bool test_one_entry(const test_input& ti) {
    // decompress_representable(ti.input.pesbt, ti.input.cursor);
    typename Handler::cap_t result;
    memset(&result, 0, sizeof(result));
    // The inputs already have the NULL xor mask removed -> Handler::decompress_raw()
    Handler::decompress_raw(ti.pesbt, ti.cursor, false, &result);
    CAPTURE(ti.pesbt, ti.cursor);

    bool success = check_fields_match<Handler>(result, ti, "");
    if (!success) {
        fprintf(stderr, "Decompressed raw pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", (uint64_t)ti.pesbt,
                (uint64_t)ti.cursor);
        dump_cap_fields(result);
    }
    // Now try recompressing and compare pesbt (for valid capabilities)
    typename Handler::length_t top_full = result.top();
    // Also don't attempt to recompress massively out-of-bounds caps since that might not work:
    if (top_full >= result.cr_base && top_full <= CC128_MAX_TOP && result.address() <= result.length()) {
        typename Handler::addr_t recompressed_pesbt = Handler::compress_raw(&result);
        typename Handler::addr_t sail_recompressed_pesbt = Handler::sail_compress_raw(&result);
        CHECK_AND_SAVE_SUCCESS(recompressed_pesbt == sail_recompressed_pesbt);
        CAPTURE(recompressed_pesbt);
        if (ti.pesbt != recompressed_pesbt) {
            fprintf(stderr,
                    "Note: Recompressing resulted in different pesbt = 0x%016" PRIx64 ", original = 0x%016" PRIx64
                    ", xor = 0x%016" PRIx64 "\n",
                    (uint64_t)recompressed_pesbt, (uint64_t)ti.pesbt, (uint64_t)(recompressed_pesbt ^ ti.pesbt));
            // This is not an error since there are multiple ways of encoding the same bot/top values with different
            // exponents
        }
        // But even if it didn't compress the same at least all fields decompressed from the new pesbt must be the same:
        typename Handler::cap_t result_recompressed;
        memset(&result_recompressed, 0, sizeof(result_recompressed));
        // The inputs already have the NULL xor mask removed -> Handler::decompress_raw()
        Handler::decompress_raw(recompressed_pesbt, ti.cursor, false, &result_recompressed);
        success = success && check_fields_match<Handler>(result_recompressed, ti, "Recompressed pesbt: ");
        // Sanity check: recompress with sail
        memset(&result_recompressed, 0, sizeof(result_recompressed));
        Handler::sail_decode_raw(recompressed_pesbt, ti.cursor, false, &result_recompressed);
        success = success && check_fields_match<Handler>(result_recompressed, ti, "Sail recompressed pesbt: ");
        if (!success) {
            fprintf(stderr, "\nOriginal decoded:\n");
            dump_cap_fields(result);
            fprintf(stderr, "\nRecompressed decoded:\n");
            dump_cap_fields(result_recompressed);
        }
        uint64_t recompressed_pesbt_after_normalize = Handler::compress_raw(&result_recompressed);
        CHECK_AND_SAVE_SUCCESS(recompressed_pesbt == recompressed_pesbt_after_normalize);
        uint64_t sail_recompressed_pesbt_after_normalize = Handler::sail_compress_raw(&result_recompressed);
        // Should match the sail values:
        CHECK_AND_SAVE_SUCCESS(recompressed_pesbt_after_normalize == sail_recompressed_pesbt_after_normalize);
    }
    return success;
}

TEST_CASE("Check sail-generated 128-bit inputs decode correctly", "[decode]") {
    int failure_count = 0;
    for (size_t i = 0; i < array_lengthof(inputs128); i++) {
        if (!test_one_entry<TestAPI128>(inputs128[i])) {
            fprintf(stderr, "Failed at index %zd\n", i);
            failure_count++;
            // break;
        }
    }
    REQUIRE(failure_count == 0);
}

TEST_CASE("Check sail-generated 64-bit inputs decode correctly", "[decode]") {
    int failure_count = 0;
    for (size_t i = 0; i < array_lengthof(inputs64); i++) {
        if (!test_one_entry<TestAPI64>(inputs64[i])) {
            fprintf(stderr, "Failed at index %zd\n", i);
            failure_count++;
            break;
        }
    }
    REQUIRE(failure_count == 0);
}
