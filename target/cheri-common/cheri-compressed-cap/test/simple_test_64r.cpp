#include "simple_test_common.cpp"

#include "cap_m_ap.h"

//
// AP compression
//
// --- lvbits = 0, valid permission set ---
TEST_CASE_M_AP_COMP(LVB_0, 0, 0, CC64R_AP_Q0 | 0)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_R, CC64R_AP_Q0 | 1)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_W, CC64R_AP_Q0 | 4)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_R | CAP_AP_W, CC64R_AP_Q0 | 5)

TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C | CAP_AP_ASR, CC64R_AP_Q1 | 0)
TEST_CASE_M_AP_COMP(LVB_0, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C | CAP_AP_ASR, CC64R_AP_Q1 | 1)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM, CC64R_AP_Q1 | 2)
TEST_CASE_M_AP_COMP(LVB_0, 1, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM, CC64R_AP_Q1 | 3)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C, CC64R_AP_Q1 | 4)
TEST_CASE_M_AP_COMP(LVB_0, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C, CC64R_AP_Q1 | 5)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W, CC64R_AP_Q1 | 6)
TEST_CASE_M_AP_COMP(LVB_0, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W, CC64R_AP_Q1 | 7)

TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_C | CAP_AP_R, CC64R_AP_Q2 | 3)

TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_C | CAP_AP_R | CAP_AP_LM, CC64R_AP_Q3 | 3)
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_C | CAP_AP_R | CAP_AP_W | CAP_AP_LM, CC64R_AP_Q3 | 7)

// --- lvbits = 0, invalid permission set ---
// Invalid permissions must result in "no permissions".
// Only C is not a valid set of permissions.
TEST_CASE_M_AP_COMP_INVALID(LVB_0, 0, CAP_AP_C, 0);
// M must not be set in Q2
TEST_CASE_M_AP_COMP_INVALID_MODE(LVB_0, 1, CAP_AP_R | CAP_AP_C, CC64R_AP_Q2 | 3);
// EL, SL are not used for lvbits == 0, but treated as reseved-one bits -> returns valid result
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL, CC64R_AP_Q3 | 3);
TEST_CASE_M_AP_COMP(LVB_0, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL, CC64R_AP_Q3 | 7);
// there is no valid combination with ASR and without X
TEST_CASE_M_AP_COMP_INVALID(LVB_0, 0, CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_ASR, 0);

// --- lvbits = 1, valid permission set ---
TEST_CASE_M_AP_COMP(LVB_1, 0, 0, CC64R_AP_Q0 | 0)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R, CC64R_AP_Q0 | 1)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_W, CC64R_AP_Q0 | 4)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_W, CC64R_AP_Q0 | 5)

TEST_CASE_M_AP_COMP(LVB_1, 0,
                    CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_ASR | CAP_AP_EL | CAP_AP_SL,
                    CC64R_AP_Q1 | 0)
TEST_CASE_M_AP_COMP(LVB_1, 1,
                    CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_ASR | CAP_AP_EL | CAP_AP_SL,
                    CC64R_AP_Q1 | 1)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL, CC64R_AP_Q1 | 2)
TEST_CASE_M_AP_COMP(LVB_1, 1, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL, CC64R_AP_Q1 | 3)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL,
                    CC64R_AP_Q1 | 4)
TEST_CASE_M_AP_COMP(LVB_1, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL,
                    CC64R_AP_Q1 | 5)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W, CC64R_AP_Q1 | 6)
TEST_CASE_M_AP_COMP(LVB_1, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W, CC64R_AP_Q1 | 7)

TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_C, CC64R_AP_Q2 | 3)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_SL, CC64R_AP_Q2 | 6)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM, CC64R_AP_Q2 | 7)

TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL, CC64R_AP_Q3 | 3)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL, CC64R_AP_Q3 | 6)
TEST_CASE_M_AP_COMP(LVB_1, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL, CC64R_AP_Q3 | 7)
// --- lvbits = 1, invalid permission set ---
// SL without W does not exist
TEST_CASE_M_AP_COMP_INVALID(LVB_1, 0, CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL, 0);

//
// AP decompression
//
// --- lvbits = 0, valid encoding ---
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q0 | 0, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q0 | 1, 0, CAP_AP_R)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q0 | 4, 0, CAP_AP_W)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q0 | 5, 0, CAP_AP_R | CAP_AP_W)

TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 0, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C | CAP_AP_ASR)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 1, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C | CAP_AP_ASR)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 2, 0, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 3, 1, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 4, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 5, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_LM | CAP_AP_C)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 6, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q1 | 7, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W)

TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q2 | 3, 0, CAP_AP_C | CAP_AP_R)

TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q3 | 3, 0, CAP_AP_C | CAP_AP_R | CAP_AP_LM)
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q3 | 7, 0, CAP_AP_C | CAP_AP_R | CAP_AP_W | CAP_AP_LM)

// --- lvbits = 0, invalid encoding ---
// invalid in Q0 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q0 | 6, 0, 0)
// invalid in Q2 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q2 | 2, 0, 0)
// invalid in Q3 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_0, CC64R_AP_Q3 | 1, 0, 0)

// --- lvbits = 1, valid encoding ---
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 0, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 1, 0, CAP_AP_R)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 4, 0, CAP_AP_W)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 5, 0, CAP_AP_R | CAP_AP_W)

TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 0, 0,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_ASR | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 1, 1,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_ASR | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 2, 0, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 3, 1, CAP_AP_X | CAP_AP_R | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 4, 0,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 5, 1,
                      CAP_AP_X | CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 6, 0, CAP_AP_X | CAP_AP_R | CAP_AP_W)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q1 | 7, 1, CAP_AP_X | CAP_AP_R | CAP_AP_W)

TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 3, 0, CAP_AP_R | CAP_AP_C)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 6, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 7, 0, CAP_AP_R | CAP_AP_W | CAP_AP_C | CAP_AP_LM)

TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 3, 0, CAP_AP_LM | CAP_AP_C | CAP_AP_R | CAP_AP_EL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 6, 0, CAP_AP_LM | CAP_AP_C | CAP_AP_R | CAP_AP_W | CAP_AP_EL | CAP_AP_SL)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 7, 0, CAP_AP_LM | CAP_AP_C | CAP_AP_R | CAP_AP_W | CAP_AP_EL)

// --- lvbits = 1, invalid encoding ---

// invalid in Q0 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 2, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 3, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 6, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q0 | 7, 0, 0)
// invalid in Q2 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 0, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 1, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 2, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 4, 0, 0) // needs LVLBITS=2
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q2 | 5, 0, 0) // needs LVLBITS=2
// invalid in Q3 -> no permissions
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 0, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 1, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 2, 0, 0)
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 4, 0, 0) // needs LVLBITS=2
TEST_CASE_M_AP_DECOMP(LVB_1, CC64R_AP_Q3 | 5, 0, 0) // needs LVLBITS=2

TEST_CASE("bounds encoding, internal exponent, T8 = 1", "[bounds]") {
    /* params are base, cursor, top */
    _cc_cap_t cap = CompressedCap64r::make_max_perms_cap(0x0, 0x1000, 0x8000);

    /*
     * 11 SDP
     * 01001 AP quadrant 1, almighty, integer mode
     * 0000 reserved
     * 0 S
     * 0 EF
     * 1 T8
     * 000000 T[7:2]
     * 00 TE
     * 00000000 B[9:2]
     * 01 BE
     *
     * internal exponent, E = 24 - 0b10001 = 7
     * LCout = 0, LMSB = 1
     * c_t = 0, c_b = 0
     */
    CHECK(cap.cr_pesbt == 0xd2040001);
    cc64r_update_perms(&cap, 0);
    cc64r_update_uperms(&cap, 0);
    CHECK(cap.cr_pesbt == 0x00040001);
}

TEST_CASE("bounds encoding, exponent > 0, T8==0", "[bounds]") {
    _cc_cap_t cap = CompressedCap64r::make_max_perms_cap(0x4C000000, 0x90000000, 0xAC000000);

    /*
     * 00 SDP
     * 01001 AP quadrant 1, almighty, integer mode
     * 0000 reserved
     * 0 S
     * 0 EF
     * 0 T8
     * 101100 T[7:2]
     * 00 TE
     * 01001100 B[9:2]
     * 10 BE
     *
     * internal exponent, E = 24 - 0b00010 = 22
     * LCout = 0, LMSB = 1
     * c_t = 0, c_b = 0
     */
    CHECK(cap.cr_pesbt == 0xd202c132);
    cc64r_update_perms(&cap, 0);
    cc64r_update_uperms(&cap, 0);
    CHECK(cap.cr_pesbt == 0x0002c132);
}

TEST_CASE("bounds encoding, exponent > 0, T8==0, c_b==-1", "[bounds]") {
    _cc_cap_t cap = CompressedCap64r::make_max_perms_cap(0x9F000000, 0xA0000000, 0xAC000000);

    /*
     * 00 SDP
     * 01001 AP quadrant 1, almighty, integer mode
     * 0000 reserved
     * 0 S
     * 0 EF
     * 0 T8
     * 100000 T[7:2]
     * 01 TE
     * 11111000 B[9:2]
     * 01 BE
     *
     * internal exponent, E = 24 - 0b00101 = 19
     * LCout = 1, LMSB = 1
     * A < R is true, B < R is false, T < R is true
     * c_t = 0, c_b = -1
     */
    CHECK(cap.cr_pesbt == 0xd20207e1);
    cc64r_update_perms(&cap, 0);
    cc64r_update_uperms(&cap, 0);
    CHECK(cap.cr_pesbt == 0x000207e1);
}

TEST_CASE("Incorrect bounds bits", "[bounds]") {
    // Regression test: the new format was still using the old V9 correctionTop algorithm.
    constexpr _cc_addr_t input_pesbt = 0x97bd62bc;
    constexpr _cc_addr_t input_cursor = 0xb43a7561;
    auto bounds_bits = TestAPICC::extract_bounds_bits(input_pesbt);
    auto sail_bounds_bits = TestAPICC::sail_extract_bounds_bits(input_pesbt);
    CHECK(bounds_bits == sail_bounds_bits);
    CHECK(bounds_bits.E == 0);
    CHECK(bounds_bits.IE == 0);
    CHECK(bounds_bits.B == 700);
    CHECK(bounds_bits.T == 88); // Previously reported 856
    auto cap = TestAPICC::decompress_raw(input_pesbt, input_cursor, false);
    auto sail_cap = TestAPICC::sail_decode_raw(input_pesbt, input_cursor, false);
    CHECK(cap == sail_cap);
    CHECK(cap.base() == 0x000000b43a72bc);
    // This previously reported top=0x000000000b43a7358 (72bc instead of 7458)
    CHECK(cap.top() == 0x000000000b43a7458);
    CHECK((int64_t)cap.offset() == 0x2a5);
}

TEST_CASE("Malformed bounds L8", "[bounds]") {
    // Regression test: the new format was still using the old V9 correctionTop algorithm.
    constexpr _cc_addr_t input_pesbt = 0x1ab768a0;
    constexpr _cc_addr_t input_cursor = 0xc58dfe0;
    auto bounds_bits = TestAPICC::extract_bounds_bits(input_pesbt);
    auto sail_bounds_bits = TestAPICC::sail_extract_bounds_bits(input_pesbt);
    CHECK(bounds_bits == sail_bounds_bits);
    CHECK(bounds_bits.E == 0);
    CHECK(bounds_bits.IE == 1);
    CHECK(bounds_bits.B == 160);
    CHECK(bounds_bits.T == 472);
    auto cap = TestAPICC::decompress_raw(input_pesbt, input_cursor, false);
    auto sail_cap = TestAPICC::sail_decode_raw(input_pesbt, input_cursor, false);
    CHECK(cap == sail_cap);
    CHECK(!cap.cr_bounds_valid);                  // Should be malformed
    CHECK(cap.base() == 0);                       // this previously reported base=0x0000000c58e0a0
    CHECK(cap.top() == 0);                        // This previously reported top=0x0000000000c58e1d8
    CHECK((int64_t)cap.offset() == input_cursor); // Previously reported 0xffffffffffffff40
    CHECK(cap.reserved_bits() == 5);
    CHECK(cap.type() == CC64R_OTYPE_SENTRY);
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
