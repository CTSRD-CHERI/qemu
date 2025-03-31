#define _CC_TEST_ENABLE_DEPRECATED
#include "test_common.cpp"
#include <catch2/matchers/catch_matchers_exception.hpp>

using Catch::Matchers::Message;

// Due to magic constant XOR aversion (i.e. fields are either entirely
// inverted or not at all, rather than select bits within them like in
// normal CHERI Concentrate), NULL is special in Morello, with bounds
// using the maximum encodable exponent, not the minimum necessary one
// like is used for the reset capability.
#ifndef TEST_CC_IS_MORELLO
TEST_CASE("Compressed NULL cap has canonical bounds", "[nullcap]") {
    _cc_cap_t null_cap;
    memset(&null_cap, 0, sizeof(null_cap));
    _cc_N(update_otype)(&null_cap, _CC_N(OTYPE_UNSEALED));
    null_cap._cr_top = _CC_N(NULL_TOP);
    null_cap.cr_pesbt = _CC_N(NULL_PESBT);
    bool ebt_exact = true;
    uint32_t computed_ebt = _cc_N(compute_ebt)(null_cap.cr_base, null_cap._cr_top, NULL, &ebt_exact);
    REQUIRE((computed_ebt & _CC_N(FIELD_EBT_MASK64)) == (null_cap.cr_pesbt & _CC_N(FIELD_EBT_MASK64)));
    REQUIRE(ebt_exact);
}
#endif

TEST_CASE("Compressed NULL cap encodes to zeroes", "[nullcap]") {
    _cc_cap_t null_cap = TestAPICC::make_null_derived_cap(0);
    auto pesbt = _cc_N(compress_mem)(&null_cap);
    auto pesbt_from_sail = TestAPICC::sail_compress_mem(null_cap);
    CHECK(pesbt == pesbt_from_sail);
    auto pesbt_without_xor = _cc_N(compress_raw)(&null_cap);
    auto pesbt_from_sail_without_xor = TestAPICC::sail_compress_raw(null_cap);
    CHECK(pesbt_without_xor == pesbt_from_sail_without_xor);
    fprintf(stderr, "NULL ENCODED: 0x%llx\n", (long long)pesbt_without_xor);
    CHECK(pesbt_without_xor == _CC_N(MEM_XOR_MASK));
    check(pesbt, (_cc_addr_t)0, "compressing NULL should result in zero pesbt");
    _cc_cap_t decompressed;
    memset(&decompressed, 'b', sizeof(decompressed));
    _cc_N(decompress_mem)(pesbt, 0, false, &decompressed);
    CHECK_FIELD(decompressed, base, 0);
    CHECK(decompressed.address() == 0);
    CHECK_FIELD(decompressed, software_permissions, 0);
    CHECK_FIELD(decompressed, permissions, 0);
    CHECK((decompressed.cr_pesbt & _CC_N(FIELD_EBT_MASK64)) == (_CC_N(NULL_PESBT) & _CC_N(FIELD_EBT_MASK64)));
    CHECK(_cc_N(get_reserved)(&decompressed) == 0); // reserved bits
    CHECK(decompressed.length() == _CC_N(NULL_LENGTH));
    CHECK(decompressed.top() == _CC_N(NULL_TOP));
    CHECK_FIELD(decompressed, type, _CC_N(OTYPE_UNSEALED));
}

TEST_CASE("Zeroes decode to NULL cap", "[nullcap]") {
    _cc_cap_t result = CompressedCapCC::decompress_mem(0, 0, false);
    fprintf(stderr, "Decompressed NULL cap:\n");
    dump_cap_fields(stderr, result);
    fprintf(stderr, "\n");
    CHECK_FIELD(result, base, 0);
    CHECK_FIELD(result, offset, 0);
    CHECK_FIELD(result, software_permissions, 0);
    CHECK_FIELD(result, permissions, 0);
    CHECK_FIELD(result, flags, 0);
    CHECK_FIELD(result, reserved_bits, 0);
    CHECK((result.cr_pesbt & _CC_N(FIELD_EBT_MASK64)) == (_CC_N(NULL_PESBT) & _CC_N(FIELD_EBT_MASK64)));
    CHECK_FIELD_RAW(result.length(), _CC_N(NULL_LENGTH));
    CHECK_FIELD(result, type, _CC_N(OTYPE_UNSEALED));
}

static void check_representable(_cc_addr_t base, _cc_length_t length, _cc_addr_t offset, bool should_work,
                                const std::string& ctx) {
    // INFO("Checking representability for " << ctx);
    // INFO("Base = " << base);
    // INFO("Length = " << length);
    // INFO("Expected to work = " << should_work);
    CAPTURE(base, length, should_work, ctx);
    bool exact_input = false;
    _cc_length_t top = base + length;
    const _cc_addr_t compressed = _cc_N(compute_ebt)(base, top, nullptr, &exact_input);
    REQUIRE(exact_input == should_work);

    _cc_cap_t decompressed;
    memset(&decompressed, 0, sizeof(decompressed));
    _cc_N(decompress_raw)(compressed, base + offset, false, &decompressed);
    CAPTURE(decompressed);
    bool unsealed_roundtrip =
        base == decompressed.cr_base && length == decompressed.length() && offset == decompressed.offset();
    CHECK(unsealed_roundtrip == should_work);
    _cc_cap_t cap_sealed;
    _cc_N(decompress_raw)(compressed, base + offset, true, &cap_sealed);
    _cc_N(update_otype)(&cap_sealed, _CC_N(OTYPE_SENTRY));
    _cc_N(decompress_raw)(cap_sealed.cr_pesbt, base + offset, true, &cap_sealed);
    bool sealed_roundtrip =
        base == decompressed.cr_base && length == decompressed.length() && offset == decompressed.offset();
    CHECK(sealed_roundtrip == unsealed_roundtrip);
}

static inline bool check_repr(bool sealed, _cc_addr_t base, _cc_addr_t length, _cc_addr_t offset) {
    (void)sealed;
    auto cap = TestAPICC::make_max_perms_cap(base, base + offset, base + length);
    return _cc_N(is_representable_cap_exact)(&cap);
}

TEST_CASE("Check max size cap representable", "[representable]") {
    // 0000000d b:0000000000000000 l:ffffffffffffffff |o:0000000000000000 t:ffffff
    check_representable(0, _CC_N(MAX_ADDRESS_PLUS_ONE), 0, true, "MAX_ADDRESS_PLUS_ONE length");
    check_representable(0, 0, 0, true, "zero length");
    check_representable(0, _CC_N(MAX_ADDR), 0, false, "MAX_ADDR length");
    check_representable((_cc_addr_t)0xffffffffff000000, 0x00000000000ffffff, 0, false, "length with too many bits");
}

TEST_CASE("Check omnipotent capability matches sail", "[sail]") {
    // 0000000d b:0000000000000000 l:ffffffffffffffff |o:0000000000000000 t:ffffff
    auto sail_reset_cap = TestAPICC::sail_reset_capability();
    auto cc_lib_reset_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    CHECK(TestAPICC::sail_reset_pesbt() == sail_reset_cap.cr_pesbt);
    CHECK(TestAPICC::sail_reset_pesbt() == cc_lib_reset_cap.cr_pesbt);
    CHECK(sail_reset_cap == cc_lib_reset_cap);
    CHECK(_cc_N(raw_equal(&sail_reset_cap, &cc_lib_reset_cap)));
    CHECK(cc_lib_reset_cap.cr_exp == _CC_N(RESET_EXP));
}

TEST_CASE("Check make_max_perms_cap() sets cr_exp correctly", "[sail]") {
    // make_max_perms_cap was not updating the cr_exp field and kept it as the NULL exponent.
    auto cap = TestAPICC::make_max_perms_cap(0, 16, 16);
    CHECK(cap.cr_exp == 0);
}

TEST_CASE("Check NULL mask matches sail", "[sail]") {
    CHECK(TestAPICC::sail_null_pesbt() == _CC_N(NULL_PESBT));
    CHECK(TestAPICC::sail_null_pesbt() == _CC_N(MEM_XOR_MASK));
}

TEST_CASE("Check reset PESBT matches sail", "[sail]") {
    TestAPICC::cap_t reset_cap = TestAPICC::sail_reset_capability();
    CHECK(TestAPICC::compress_raw(reset_cap) == _CC_N(RESET_PESBT));
    // reset_pesbt returns the internal "raw" pesbt value rather than the in-memory representation
    CHECK(TestAPICC::sail_reset_pesbt() == _CC_N(RESET_PESBT));
    CHECK(TestAPICC::compress_mem(reset_cap) == (_CC_N(RESET_PESBT) ^ _CC_N(MEM_XOR_MASK)));
    fprintf(stderr, "Decompressed reset cap:\n");
    dump_cap_fields(stderr, reset_cap);
    fprintf(stderr, "\n");
}

static inline TestAPICC::cap_t checkFastRepCheckSucceeds(const _cc_cap_t& cap, _cc_addr_t new_addr,
                                                         bool set_addr_should_retain_tag = true) {
    bool sail_fast_rep = TestAPICC::sail_fast_is_representable(cap, new_addr);
    bool cc_fast_rep = TestAPICC::fast_is_representable_new_addr(cap, new_addr);
    CHECK(sail_fast_rep == cc_fast_rep);
    CHECK(cc_fast_rep);
    // It should also be representable if we do the full check since the bounds interpretation does not change.
    // NB: Unlike precise_is_representable_new_addr, fast_is_representable_new_addr does not look at cr_bounds_valid.
    if (cap.cr_bounds_valid) {
        CHECK(TestAPICC::precise_is_representable_new_addr(cap, new_addr));
    }
    // Check that creating a new capability with same pesbt and new address decodes to the same bounds
    TestAPICC::cap_t new_cap_with_other_cursor = TestAPICC::decompress_raw(cap.cr_pesbt, new_addr, false);
    CHECK(new_cap_with_other_cursor.base() == cap.base());
    CHECK(new_cap_with_other_cursor.top() == cap.top());
    auto updated = cap;
    updated.cr_tag = true;
    // Updating the address should retain the tag and bounds should stay the same.
    // Calls to set_addr checksfor sealed, so unseal to ensure set_addr only checks representability.
    _cc_N(update_otype)(&updated, _CC_N(OTYPE_UNSEALED));
    _cc_N(set_addr)(&updated, new_addr);
    CHECK(updated.cr_tag == set_addr_should_retain_tag);
    if (set_addr_should_retain_tag) {
        CHECK(updated.base() == cap.base());
        CHECK(updated.top() == cap.top());
    }
    return cap;
}

static inline TestAPICC::cap_t checkFastRepCheckSucceeds(_cc_addr_t pesbt, _cc_addr_t addr, _cc_addr_t expected_base,
                                                         _cc_length_t expected_top, _cc_addr_t new_addr,
                                                         bool set_addr_should_retain_tag = true) {
    TestAPICC::cap_t cap = TestAPICC::decompress_raw(pesbt, addr, false);
    TestAPICC::cap_t sail_cap = TestAPICC::sail_decode_raw(pesbt, addr, false);
    CHECK(cap == sail_cap);
    CHECK(cap.base() == expected_base);
    CHECK(cap.top() == expected_top);
    checkFastRepCheckSucceeds(cap, new_addr, set_addr_should_retain_tag);
    return cap;
}

/// Check that both the fast and the full representability check fails for the given input.
static inline void checkRepCheckFails(_cc_addr_t pesbt, _cc_addr_t addr, _cc_addr_t expected_base,
                                      _cc_length_t expected_top, _cc_addr_t new_addr) {
    TestAPICC::cap_t cap = TestAPICC::decompress_raw(pesbt, addr, false);
    CHECK(cap.base() == expected_base);
    CHECK(cap.top() == expected_top);
    bool sail_fast_rep = TestAPICC::sail_fast_is_representable(cap, new_addr);
    bool cc_fast_rep = TestAPICC::fast_is_representable_new_addr(cap, new_addr);
    CHECK(sail_fast_rep == cc_fast_rep);
    CHECK(!cc_fast_rep);
    // It should also not be representable if we do the full check since the bounds interpretation changes.
    CHECK(!TestAPICC::precise_is_representable_new_addr(cap, new_addr));
    // Check that creating a new capability with same pesbt and new address decodes to different bounds
    TestAPICC::cap_t new_cap_with_other_cursor = TestAPICC::decompress_raw(pesbt, new_addr, false);
    CHECK((new_cap_with_other_cursor.base() != expected_base || new_cap_with_other_cursor.top() != expected_top));
}

TEST_CASE("Omnipotent cap representable", "") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    // Check that various address are representable with a full-address-space capability.
    checkFastRepCheckSucceeds(max_cap, 0);
    checkFastRepCheckSucceeds(max_cap, 1);
    checkFastRepCheckSucceeds(max_cap, _CC_MAX_ADDR - 1);
    checkFastRepCheckSucceeds(max_cap, _CC_MAX_ADDR);
    checkFastRepCheckSucceeds(max_cap, _CC_MAX_ADDR >> 1);
    // Alternating ones and zeroes
    checkFastRepCheckSucceeds(max_cap, (_cc_addr_t)UINT64_C(0xaaaaaaaaaaaaaaaa));
    // And a few arbitrary values
    checkFastRepCheckSucceeds(max_cap, 0x3fffca);
    checkFastRepCheckSucceeds(max_cap, 0x12345678);
    checkFastRepCheckSucceeds(max_cap, 1234);
    checkFastRepCheckSucceeds(max_cap, _CC_MAX_ADDR - 1234);
}

TEST_CASE("pesbt_is_correct ignores cr_extra", "") {
    TestAPICC::cap_t null_cap = TestAPICC::make_null_derived_cap(0);
    CHECK(_cc_N(pesbt_is_correct)(&null_cap));
    // Changing cr_extra previously triggered assertions, check that it is ignored.
    null_cap.cr_extra = 10;
    CHECK(_cc_N(pesbt_is_correct)(&null_cap));
}

TEST_CASE("common permissions for almighty", "[perms]") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    CHECK(max_cap.all_permissions() > 1);
    // Execute and ASR are supported in all implementations, check that they are set.
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)));
    CHECK(max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)));
    CHECK(max_cap.software_permissions() == _CC_N(UPERMS_ALL));
}

TEST_CASE("set zero and almighty permissions", "[perms]") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    CHECK(max_cap.all_permissions() > 1);
    CHECK((max_cap.all_permissions() & _CC_N(PERM_SW_ALL)) == _CC_N(PERM_SW_ALL));
    CHECK((max_cap.all_permissions() & _CC_N(PERMS_MASK)) == _CC_N(PERMS_MASK));
    // Should be able to clear all permissions
    CHECK(_cc_N(set_permissions)(&max_cap, 0) == true);
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)) == false);
    CHECK(max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)) == false);
    CHECK(max_cap.software_permissions() == 0);
    // Setting all permission bits should be fine and return max perms cap.
    CHECK(_cc_N(set_permissions)(&max_cap, _CC_N(PERMS_MASK)) == true);
    CHECK(max_cap.software_permissions() == _CC_N(UPERMS_ALL));
    CHECK((max_cap.all_permissions() & _CC_N(PERMS_MASK)) == _CC_N(PERMS_MASK));
}

TEST_CASE("set invalid perms", "[perms]") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
#ifndef NDEBUG
    // Setting reserved-zero permissions should be rejected
    CHECK_THROWS_MATCHES(_cc_N(set_permissions)(&max_cap, ~(_cc_addr_t)0), std::invalid_argument,
                         Message("invalid permissions"));
#endif
    // Should not have modified the permissions
    CHECK((max_cap.all_permissions() & _CC_N(PERMS_MASK)) == _CC_N(PERMS_MASK));
}

TEST_CASE("test old permissions API", "[perms]") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    _cc_addr_t all_perms = max_cap.all_permissions();
    _cc_addr_t all_perms_old_api = _cc_N(get_perms)(&max_cap); // ignoring reserved 1 bits
    CHECK(max_cap.all_permissions() > 1);
    CHECK((max_cap.all_permissions() & _CC_N(PERM_SW_ALL)) == _CC_N(PERM_SW_ALL));
    CHECK(_cc_N(get_uperms)(&max_cap) == _CC_N(UPERMS_ALL));
    // Should be able to clear all permissions, but user perms should be unaffected
    _cc_N(update_perms)(&max_cap, 0);
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)) == false);
    CHECK(max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)) == false);
    CHECK(_cc_N(get_uperms)(&max_cap) == _CC_N(UPERMS_ALL));
    CHECK((max_cap.all_permissions() & _CC_N(PERM_SW_ALL)) == _CC_N(PERM_SW_ALL));

    // Now clear SW perms and check it doesn't affect HW ones:
    max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    _cc_N(update_uperms)(&max_cap, 0);
    CHECK(max_cap.software_permissions() == 0);
    CHECK(max_cap.all_permissions() == (all_perms & ~_CC_N(PERM_SW_ALL)));
    CHECK(_cc_N(get_perms)(&max_cap) == all_perms_old_api);
    _cc_N(update_uperms)(&max_cap, _CC_N(UPERMS_ALL));
    CHECK(max_cap.software_permissions() == _CC_N(UPERMS_ALL));
    CHECK(_cc_N(get_perms)(&max_cap) == all_perms_old_api);
    if (_CC_N(UPERMS_ALL) != 0) {
        _cc_N(update_uperms)(&max_cap, 1);
        CHECK(max_cap.software_permissions() == 1);
        CHECK(_cc_N(get_perms)(&max_cap) == all_perms_old_api);
    }
    // The legacy API returns zero for NULL cap permissions since it does not include reserved one-bits
    TestAPICC::cap_t null_cap = TestAPICC::make_null_derived_cap(0);
    CHECK(_cc_N(get_perms)(&null_cap) == 0);
    CHECK(_cc_N(get_uperms)(&null_cap) == 0);
}

#ifndef TEST_CC_IS_MORELLO
TEST_CASE("test mode API", "[perms]") {
    constexpr bool is_isa_v9 = _CC_N(USES_V9_CORRECTION_FACTORS);
    const TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    const TestAPICC::cap_t null_cap = TestAPICC::make_null_derived_cap(0);
    // NULL cap always has a mode of zero (capmode for RISC-V, intmode for V9).
    CHECK(_cc_N(get_execution_mode)(&null_cap) == (_cc_mode)0);
    CHECK(_cc_N(get_execution_mode)(&null_cap) == (is_isa_v9 ? _CC_N(MODE_INT) : _CC_N(MODE_CAP)));
    // The infinite cap always has integer mode
    CHECK(_cc_N(get_execution_mode)(&max_cap) == _CC_N(MODE_INT));

    {
        auto cap = max_cap;
        CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_INT))); // No change
        CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_INT));
        CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_CAP))); // Change to capmode
        CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_CAP));
        CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_INT))); // Change back to capmode should be ok
        CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_INT));
    }
    {
        auto cap = null_cap;
        if (is_isa_v9) {
            // For ISAv9, the mode can always be updated.
            CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_INT));
            CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_CAP)) == true);
            CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_CAP));
        } else {
            // For the RISC-V standard version, we require execute permissions to change the mode
            CHECK(cap.has_permissions(_CC_N(PERM_EXECUTE)) == false);
            CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_CAP));
            CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_INT)) == false);
            CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_CAP));
            // Even setting the mode to the current mode should return failure.
            CHECK(_cc_N(set_execution_mode)(&cap, _CC_N(MODE_CAP)) == false);
            CHECK(_cc_N(get_execution_mode)(&cap) == _CC_N(MODE_CAP));
        }
    }
}

TEST_CASE("removing ASR should not throw", "[perms]") {
    TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)));
    CHECK(max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)));
    const _cc_addr_t initial_perms = max_cap.all_permissions();
    // Keeping the reserved one-bits set should not be an API violation.
    CHECK(_cc_N(set_permissions)(&max_cap, initial_perms & ~_CC_N(PERM_ACCESS_SYS_REGS)) == true);
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)));
    CHECK(!max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)));
}

TEST_CASE("removing ASR should not affect mode", "[perms]") {
    const TestAPICC::cap_t max_cap = TestAPICC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    CHECK(max_cap.has_permissions(_CC_N(PERM_EXECUTE)));
    CHECK(max_cap.has_permissions(_CC_N(PERM_ACCESS_SYS_REGS)));
    const _cc_addr_t initial_perms = max_cap.all_permissions();
    {
        auto cap = max_cap;
        _cc_mode initial_mode = _cc_N(get_execution_mode(&max_cap));
        CHECK(_cc_N(set_permissions)(&cap, initial_perms & ~_CC_N(PERM_ACCESS_SYS_REGS)) == true);
        CHECK(_cc_N(get_execution_mode(&cap)) == initial_mode); // mode should not have changed
    }
    for (auto mode : {_CC_N(MODE_INT), _CC_N(MODE_CAP)}) {
        auto cap = max_cap;
        CHECK(_cc_N(set_execution_mode(&cap, mode))); // setting mode should work
        CHECK(_cc_N(set_permissions)(&cap, initial_perms & ~_CC_N(PERM_ACCESS_SYS_REGS)) == true);
        CHECK(_cc_N(get_execution_mode(&cap)) == mode); // mode should not have changed
    }
}

#endif
