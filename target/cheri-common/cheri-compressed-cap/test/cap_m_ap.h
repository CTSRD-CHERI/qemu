#pragma once

#define CAP_AP_C _CC_N(PERM_CAPABILITY)
#define CAP_AP_W _CC_N(PERM_WRITE)
#define CAP_AP_R _CC_N(PERM_READ)
#define CAP_AP_X _CC_N(PERM_EXECUTE)
#define CAP_AP_ASR _CC_N(PERM_ACCESS_SYS_REGS)
#define CAP_AP_LM _CC_N(PERM_LOAD_MUTABLE)
#define CAP_AP_EL _CC_N(PERM_ELEVATE_LEVEL)
#define CAP_AP_SL _CC_N(PERM_STORE_LEVEL)

#define LVB_0 0
#define LVB_1 1

static inline _cc_addr_t compute_supported_permissions(unsigned level_bits) {
    _cc_addr_t supported_permissions = CAP_AP_C | CAP_AP_W | CAP_AP_R | CAP_AP_X | CAP_AP_ASR | CAP_AP_LM;
    if (level_bits > 0)
        supported_permissions |= CAP_AP_EL | CAP_AP_SL;
    return supported_permissions;
}

static void testCompression(unsigned level_bits, _cc_mode mode, _cc_addr_t permissions, _cc_addr_t expected_encoding,
                            bool valid_permissions_set, bool valid_mode = true) {
    // ensure we didn't request unsupported perms:
    const _cc_addr_t supported_permissions = compute_supported_permissions(level_bits);
    REQUIRE(level_bits <= 1);
    _cc_cap_t cap = TestAPICC::make_null_derived_cap(0);
    cap.cr_lvbits = level_bits;
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, AP_M) == 0);
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, SDP) == 0);
    CHECK(_cc_N(set_permissions)(&cap, permissions) == valid_permissions_set);
    const bool has_x = cap.has_permissions(_CC_N(PERM_EXECUTE));
    CHECK(_cc_N(set_execution_mode)(&cap, mode) == has_x);
    if (valid_permissions_set) {
        CHECK(cap.has_permissions(permissions));
    } else {
        CHECK((cap.all_permissions() & supported_permissions) == 0); // Invalid permissions should report 0
    }
    if (!valid_mode) {
        CHECK(_cc_N(get_execution_mode)(&cap) != mode);
    }
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, AP_M) == expected_encoding);
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, SDP) == 0);
}

static void testDecompression(unsigned level_bits, _cc_addr_t ap_m_encoding, _cc_mode expected_mode,
                              _cc_addr_t expected_permissions) {
    // ensure we didn't request unsupported perms:
    const _cc_addr_t supported_permissions = compute_supported_permissions(level_bits);
    REQUIRE((expected_permissions & supported_permissions) == expected_permissions);
    REQUIRE(level_bits <= 1);
    _cc_cap_t cap = TestAPICC::make_null_derived_cap(0);
    cap.cr_lvbits = level_bits;
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, AP_M) == 0);
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, SDP) == 0);
    cap.cr_pesbt = _CC_DEPOSIT_FIELD(cap.cr_pesbt, ap_m_encoding, AP_M);
    CHECK(cap.has_permissions(expected_permissions));
    CHECK((cap.all_permissions() & supported_permissions) == expected_permissions); // ignore reserved-one bits
    CHECK(_cc_N(get_execution_mode)(&cap) == expected_mode);
    CHECK(_CC_EXTRACT_FIELD(cap.cr_pesbt, AP_M) == ap_m_encoding);
    CHECK(cap.software_permissions() == 0); // should not have been set
}

#define TEST_CASE_M_AP_COMP(_cr_lvbits, _cr_m, _cr_arch_perm, _cr_ap_m)                                                \
    TEST_CASE(#_cr_lvbits "-" #_cr_arch_perm "-" #_cr_m " compress", "[compress]") {                                   \
        testCompression(_cr_lvbits, (_cc_mode)_cr_m, _cr_arch_perm, _cr_ap_m, true);                                   \
    }

#define TEST_CASE_M_AP_COMP_INVALID(_cr_lvbits, _cr_m, _cr_arch_perm, _cr_ap_m)                                        \
    TEST_CASE(#_cr_lvbits "-" #_cr_arch_perm "-" #_cr_m " invalid compress", "[compress]") {                           \
        testCompression(_cr_lvbits, (_cc_mode)_cr_m, _cr_arch_perm, _cr_ap_m, false);                                  \
    }
#define TEST_CASE_M_AP_COMP_INVALID_MODE(_cr_lvbits, _cr_m, _cr_arch_perm, _cr_ap_m)                                   \
    TEST_CASE(#_cr_lvbits "-" #_cr_arch_perm "-" #_cr_m " invalid mode compress", "[compress]") {                      \
        testCompression(_cr_lvbits, (_cc_mode)_cr_m, _cr_arch_perm, _cr_ap_m, true, false);                            \
    }

#define TEST_CASE_M_AP_DECOMP(_cr_lvbits, _cr_ap_m, _cr_m, _cr_arch_perm)                                              \
    TEST_CASE(#_cr_lvbits "-" #_cr_ap_m " decompress", "[decompress]") {                                               \
        testDecompression(_cr_lvbits, _cr_ap_m, (_cc_mode)_cr_m, _cr_arch_perm);                                       \
    }
