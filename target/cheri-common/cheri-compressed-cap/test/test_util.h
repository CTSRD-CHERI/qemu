#include <iostream>
#include <ostream>
#include <string>

std::ostream& operator<<(std::ostream& os, cc128_length_t value);

std::ostream& operator<<(std::ostream& os, cc128_length_t value) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "{0x%" PRIx64 " %016" PRIx64 "}", (uint64_t)(value >> 64), (uint64_t)(value));
    return os << buffer;
}

static const char* otype_suffix(uint32_t otype) {
    // Two separate switches since if the number of otype bits is the same
    // we cannot have both of the case statements in one switch
    switch (otype) {
    case CC128_OTYPE_UNSEALED: return " (CC128_OTYPE_UNSEALED)";
    case CC128_OTYPE_SENTRY: return " (CC128_OTYPE_SENTRY)";
    case CC128_OTYPE_RESERVED2: return " (CC128_OTYPE_RESERVED2)";
    case CC128_OTYPE_RESERVED3: return " (CC128_OTYPE_RESERVED3)";
    default: break;
    }
    return "";
}

std::ostream& operator<<(std::ostream& os, const cc128_bounds_bits& value);
std::ostream& operator<<(std::ostream& os, const cc128_bounds_bits& value) {
    os << "{ B: " << (unsigned)value.B << " T: " << (unsigned)value.T << " E: " << (unsigned)value.E
       << " IE: " << (unsigned)value.IE << " }";
    return os;
}
std::ostream& operator<<(std::ostream& os, const cc64_bounds_bits& value);
std::ostream& operator<<(std::ostream& os, const cc64_bounds_bits& value) {
    os << "{ B: " << (unsigned)value.B << " T: " << (unsigned)value.T << " E: " << (unsigned)value.E
       << " IE: " << (unsigned)value.IE << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const cc128_cap_t& value);
std::ostream& operator<<(std::ostream& os, const cc128_cap_t& value) {
    char buffer[4096];
    cc128_length_t top_full = value._cr_top;
    snprintf(buffer, sizeof(buffer),
             "\tPermissions: 0x%" PRIx32 "\n"
             "\tUser Perms:  0x%" PRIx32 "\n"
             "\tBase:        0x%016" PRIx64 "\n"
             "\tOffset:      0x%016" PRIx64 "\n"
             "\tLength:      0x%" PRIx64 "%016" PRIx64 " %s\n"
             "\tTop:         0x%" PRIx64 "%016" PRIx64 " %s\n"
             "\tSealed:      %d\n"
             "\tOType:       0x%" PRIx32 "%s\n",
             value.cr_perms, value.cr_uperms, value.cr_base, (uint64_t)value.offset(), (uint64_t)(value.length() >> 64),
             (uint64_t)value.length(), value.length() > UINT64_MAX ? " (greater than UINT64_MAX)" : "",
             (uint64_t)(top_full >> 64), (uint64_t)top_full, top_full > UINT64_MAX ? " (greater than UINT64_MAX)" : "",
             (int)(value.is_sealed()), value.cr_otype, otype_suffix(value.cr_otype));
    os << "{\n" << buffer << "}";
    return os;
}
std::ostream& operator<<(std::ostream& os, const cc64_cap_t& value);
std::ostream& operator<<(std::ostream& os, const cc64_cap_t& value) {
    char buffer[4096];
    snprintf(buffer, sizeof(buffer),
             "\tPermissions: 0x%" PRIx32 "\n"
             "\tUser Perms:  0x%" PRIx32 "\n"
             "\tBase:        0x%08" PRIx32 "\n"
             "\tOffset:      0x%08" PRIx32 "\n"
             "\tLength:      0x%08" PRIx64 " %s\n"
             "\tTop:         0x%08" PRIx64 " %s\n"
             "\tSealed:      %d\n"
             "\tOType:       0x%" PRIx32 "%s\n",
             value.cr_perms, value.cr_uperms, value.base(), (uint32_t)value.offset(), (uint64_t)value.length(),
             value.length() > UINT32_MAX ? " (greater than UINT32_MAX)" : "", (uint64_t)value.top(),
             value.top() > UINT32_MAX ? " (greater than UINT32_MAX)" : "", (int)(value.is_sealed()), value.cr_otype,
             otype_suffix(value.cr_otype));
    os << "{\n" << buffer << "}";
    return os;
}

static inline bool operator==(const cc128_bounds_bits& a, const cc128_bounds_bits& b) {
    return a.B == b.B && a.E == b.E && a.T == b.T && a.IE == b.IE;
}
static inline bool operator==(const cc64_bounds_bits& a, const cc64_bounds_bits& b) {
    return a.B == b.B && a.E == b.E && a.T == b.T && a.IE == b.IE;
}

#include "catch.hpp"

static bool failed = false;

template <typename T> static inline bool check(T expected, T actual, const std::string& msg) {
    if (expected == actual)
        return true;
    std::cerr << "ERROR: " << msg << ": expected 0x" << std::hex << expected << " != 0x" << actual << "\n";
    failed = true;
    return false;
}

template <class T, std::size_t N> constexpr inline size_t array_lengthof(T (&)[N]) { return N; }

template <class Cap> static void dump_cap_fields(const Cap& result) {
    fprintf(stderr, "Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
    fprintf(stderr, "User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
    fprintf(stderr, "Base:        0x%016" PRIx64 "\n", (uint64_t)result.base());
    fprintf(stderr, "Offset:      0x%016" PRIx64 "\n", (uint64_t)result.offset());
    fprintf(stderr, "Cursor:      0x%016" PRIx64 "\n", (uint64_t)result.address());
    cc128_length_t len_full = result.length();
    fprintf(stderr, "Length:      0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(len_full >> 64), (uint64_t)len_full,
            len_full > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
    cc128_length_t top_full = result.top();
    fprintf(stderr, "Top:         0x%" PRIx64 "%016" PRIx64 " %s\n", (uint64_t)(top_full >> 64), (uint64_t)top_full,
            top_full > UINT64_MAX ? " (greater than UINT64_MAX)" : "");
    fprintf(stderr, "Sealed:      %d\n", (int)result.is_sealed());
    fprintf(stderr, "OType:       0x%" PRIx32 "%s\n", result.type(), otype_suffix(result.type()));
    fprintf(stderr, "\n");
}

__attribute__((used)) static cap_register_t decompress_representable(uint64_t pesbt_already_xored, uint64_t cursor) {
    cap_register_t result;
    printf("Decompressing pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", pesbt_already_xored, cursor);
    cc128_decompress_raw(pesbt_already_xored, cursor, false, &result);
    dump_cap_fields(result);
    // Check that the result is the same again when compressed
    uint64_t new_pesbt_already_xored = cc128_compress_raw(&result);
    CHECK(pesbt_already_xored == new_pesbt_already_xored);
    CHECK(cursor == result.address());
    return result;
}

inline cap_register_t make_max_perms_cap(uint64_t base, uint64_t offset, cc128_length_t length) {
    cap_register_t creg;
    memset(&creg, 0, sizeof(creg));
    creg.cr_base = base;
    creg._cr_cursor = base + offset;
    creg._cr_top = base + length;
    creg.cr_perms = CC128_PERMS_ALL;
    creg.cr_uperms = CC128_UPERMS_ALL;
    creg.cr_otype = CC128_OTYPE_UNSEALED;
    creg.cr_tag = true;
    bool exact_input = false;
    creg.cr_ebt = cc128_compute_ebt(creg.cr_base, creg._cr_top, NULL, &exact_input);
    assert(exact_input && "Invalid arguments");
    assert(cc128_is_representable_cap_exact(&creg));
    return creg;
}

#define DO_STRINGIFY2(x) #x
#define DO_STRINGIFY1(x) DO_STRINGIFY2(x)
#define STRINGIFY(x) DO_STRINGIFY1(x)

#define CHECK_FIELD_RAW(value, expected) CHECK(value == expected)
#define CHECK_FIELD(cap, field, expected) CHECK((uint64_t)expected == cap.field())

enum {
#ifdef CC128_OLD_FORMAT
    TESTING_OLD_FORMAT = 1
#else
    TESTING_OLD_FORMAT = 0
#endif
};
