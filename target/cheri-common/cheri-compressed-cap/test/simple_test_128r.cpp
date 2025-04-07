#include "simple_test_common.cpp"

#include "cap_m_ap.h"

/*
 * For 64-bit risc-v cheri, the conversion between AP bitfield and its encoding
 * is a trivial 1:1 mapping. Some spot checks will do, there's no point in
 * checking all combinations.
 */
enum {
    ENC_C = _CC_BIT64(0),
    ENC_W = _CC_BIT64(1),
    ENC_R = _CC_BIT64(2),
    ENC_X = _CC_BIT64(3),
    ENC_ASR = _CC_BIT64(4),
    ENC_LM = _CC_BIT64(5),
    ENC_EL = _CC_BIT64(6),
    ENC_SL = _CC_BIT64(7),
    INT_MODE_ENCODED = _CC_BIT64(_CC_N(FIELD_AP_SIZE)),
};
// AP compression
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_R, ENC_R)
TEST_CASE_M_AP_COMP(LVB_0, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_ASR,
                    INT_MODE_ENCODED | ENC_X | ENC_R | ENC_W | ENC_C | ENC_ASR)
// For lvlbits=0, we don't encode SL or EL
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_SL | CAP_AP_EL,
                    ENC_X | ENC_R | ENC_W | ENC_C | ENC_LM)
// But we do for lvlbits=1
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_SL | CAP_AP_EL,
                    ENC_X | ENC_R | ENC_W | ENC_C | ENC_LM | ENC_SL | ENC_EL)

// AP decompression
TEST_CASE_M_AP_DECOMP(LVB_0, INT_MODE_ENCODED | ENC_X | ENC_R | ENC_W | ENC_C | ENC_ASR, 1,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_ASR)
TEST_CASE_M_AP_DECOMP(LVB_0, ENC_X | ENC_R | ENC_W, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W)

TEST_CASE_M_AP_DECOMP(LVB_1, INT_MODE_ENCODED | ENC_X | ENC_R | ENC_W | ENC_C | ENC_ASR | ENC_SL, 1,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_ASR | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, ENC_X | ENC_R | ENC_W | ENC_EL, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_EL)

TEST_CASE("Reprentability with TOP>MAX_TOP", "[representable]") {
    auto cap = TestAPICC::make_max_perms_cap(0xffff002d01ffc000, 0xffff002d02013ff6, 0xffff002d027fc000,
                                             TestAPICC::MODE_INT, /*lvbits=*/0);
    CHECK(cap.cr_pesbt == 0x1f3f00003ff7ff9);
    CHECK(!TestAPICC::sail_precise_is_representable(cap, 0));
    CHECK(!_cc_N(_fast_is_representable_new_addr)(&cap, 0));
    // The following line used to assert with cdp->_cr_top <= ((cc128_length_t)1u << 64)
    CHECK(!_cc_N(_precise_is_representable_new_addr)(&cap, 0));
    // Decode a new capability with the same pesbt value and check that the bounds differ (and are > MAX_TOP)
    const _cc_cap_t cap2 = TestAPICC::decompress_raw(cap.cr_pesbt, 0, false);
    CHECK(cap2.base() != cap.base());
    CHECK(cap2.top() != cap.top());
    CHECK(cap2.top() > _CC_MAX_TOP);
    CHECK(cap2.base() == 0xffffffffffffc000);
    CHECK(cap2.top() == (_CC_MAX_TOP | 0x7fc000));
}

TEST_CASE("New adjustment factors", "[bounds]") {
    // Regression test: the new format was still using the old V9 correctionTop algorithm.
    auto cap = TestAPICC::decompress_raw(0xd1ce2baa14c79148, 0x934bb9c2f0eb84d6, false);
    auto sail_cap = TestAPICC::sail_decode_raw(0xd1ce2baa14c79148, 0x934bb9c2f0eb84d6, false);
    CHECK(cap == sail_cap);
    // This previously reported base=0x934bb9c2f0eb5148 (5148 instead of 9148)
    CHECK(cap.base() == 0x934bb9c2f0eb9148);
    // This previously reported top=0x934bb9c2f0eb84d6 (84d6 instead of 931e)
    CHECK(cap.top() == 0x934bb9c2f0eb931e);
    // This previously reported offset=0x338e
    CHECK((int64_t)cap.offset() == -0xc72);
}

TEST_CASE("New adjustment factors 2", "[bounds]") {
    // Regression test: the new format was still using the old V9 correctionTop algorithm.
    // cursor: 0x3e6420dc58fe385b
    auto cap = TestAPICC::decompress_raw(0x1f1da60bcff2c08e, 0x3e6420dc58fe385b, false);
    auto sail_cap = TestAPICC::sail_decode_raw(0x1f1da60bcff2c08e, 0x3e6420dc58fe385b, false);
    CHECK(cap == sail_cap);
    // This previously reported base=0x3e6420dc58fe008e (008e instead of 408e)
    CHECK(cap.base() == 0x3e6420dc58fe408e);
    // This previously reported top=0x3e6420dc58fe0fcb (0fcb instead of 4fcb)
    CHECK(cap.top() == 0x3e6420dc58fe4fcb);
    // This previously reported offset=37cd
    CHECK((int64_t)cap.offset() == -0x833);
}

TEST_CASE("Malformed bounds return zero", "[bounds]") {
    // Regression test: this pesbt value did not report malformed bounds
    auto cap = TestAPICC::decompress_raw(0x5042f7ed6b1027a9, 0, false);
    auto sail_cap = TestAPICC::sail_decode_raw(0x5042f7ed6b1027a9, 0, false);
    CHECK(cap == sail_cap);
    CHECK(!cap.cr_bounds_valid);
    CHECK(cap.base() == 0);
    CHECK(cap.top() == 0);
    CHECK(cap.cr_exp == 51);
}

TEST_CASE("bounds encoding exponent 0", "[bounds]") {
    /* params are base, cursor, top */
    _cc_cap_t cap = TestAPICC::make_max_perms_cap(0x0, 0x10, 0x20, TestAPICC::MODE_INT, /*lvbits=*/0);

    /*
     * EF == 1 -> exponent 0
     * { T, TE } == 0b0000_0010_0000
     * { B, BE } == 0
     * all of LCout, LMSB, c_t, c_b are 0
     *
     * top == 0x20, base == 0x0
     */
    CHECK(cap.cr_pesbt == 0x01f3f00004080000);
    CHECK(cc128r_set_execution_mode(&cap, CC128R_MODE_CAP));
    cc128r_set_permissions(&cap, 0);
    CHECK(cap.cr_pesbt == 0x0000000004080000);
}

TEST_CASE("bounds encoding exponent > 0", "[bounds]") {
    _cc_cap_t cap = TestAPICC::make_max_perms_cap(0x8000, 0x41DF, 0xA6400, TestAPICC::MODE_INT, /*lvbits=*/0);

    /*
     * EF == 0 -> internal exponent
     * E = 52 - 45 = 7
     *
     * T[11:3] == 0 1001 1001
     * TE == 101
     * B[13:3] == 000 0010 0000
     * BE == 101
     *
     * LCout = 0, LMSB = 1
     * c_t = 0, c_b = 0
     */
    CHECK(cap.cr_pesbt == 0x01f3f00001334105);
    CHECK(cc128r_set_execution_mode(&cap, CC128R_MODE_CAP));
    cc128r_set_permissions(&cap, 0);
    CHECK(cap.cr_pesbt == 0x0000000001334105);
}

TEST_CASE("max perms value", "[perms]") {
    _cc_cap_t cap = CompressedCapCC::make_max_perms_cap(0, 0, _CC_MAX_TOP);
    // GCPERM on the almighty capability should report ones in the low 24 bits
    CHECK(cap.all_permissions() == _CC_BITMASK64(24));
    CHECK(cap.all_permissions() & CC128R_PERM_ACCESS_SYS_REGS);
    CHECK(cap.all_permissions() & CC128R_PERM_CAPABILITY);
    CHECK(cap.all_permissions() & CC128R_PERM_ELEVATE_LEVEL);
    CHECK(cap.all_permissions() & CC128R_PERM_EXECUTE);
    CHECK(cap.all_permissions() & CC128R_PERM_LEVEL);
    CHECK(cap.all_permissions() & CC128R_PERM_LOAD_MUTABLE);
    CHECK(cap.all_permissions() & CC128R_PERM_READ);
    CHECK(cap.all_permissions() & CC128R_PERM_STORE_LEVEL);
    CHECK(cap.all_permissions() & CC128R_PERM_WRITE);
}

TEST_CASE("No longer using fast rep check", "[repr]") {
    // The fast representability check is completely wrong for CC128R and is no longer used.
    // These values were found via fuzzing: fast succeeds, precise fails:
    const _cc_addr_t pesbt = 0x00000000002323;
    const _cc_addr_t cursor = 0x2323232323230000;
    const _cc_addr_t new_addr = 0x2b2333232323232b;
    auto cap = TestAPICC::decompress_raw(pesbt, cursor, true);
    auto sail_cap = TestAPICC::sail_decode_raw(pesbt, cursor, true);
    CHECK(cap == sail_cap);
    CHECK(cap.cr_bounds_valid);
    CHECK(cap.base() == 0xc640000000000000);
    CHECK(cap.top() == _CC_MAX_TOP);
    CHECK(cap.cr_exp == 49);
    CHECK(!_cc_N(_precise_is_representable_new_addr)(&cap, new_addr));
    // The sail API always uses the precise check even in the sail_fast_is_representable
    CHECK(!TestAPICC::sail_precise_is_representable(cap, new_addr));
    CHECK(!cc128r_is_representable_with_addr(&cap, new_addr, true));
    CHECK(!cc128r_is_representable_with_addr(&cap, new_addr, false));
    // TODO: should not expose the fast rep check for cc128r, for now just have it be the same as precise
    CHECK(!_cc_N(_fast_is_representable_new_addr)(&cap, new_addr));
    CHECK(!TestAPICC::sail_fast_is_representable(cap, new_addr));
}
