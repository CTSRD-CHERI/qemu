#include <ostream>
#include <iostream>

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& __out, unsigned __int128 i);

std::basic_ostream<char, std::char_traits<char>>& operator<<(std::basic_ostream<char, std::char_traits<char>>& __out, unsigned __int128 i) {
    return __out << "{" << (uint64_t)(i >> 64) << "," << (uint64_t)(i) << "}";
}

static bool failed = false;

template <typename T> static bool check(T expected, T actual, const char* msg) {
    if (expected == actual)
        return true;
    std::cerr << "ERROR: " << msg << ": 0x" << std::hex << actual << " != 0x" << expected << "\n";
    failed = true;
    return false;
}

__attribute__((used)) static cap_register_t decompress_representable(uint64_t pesbt, uint64_t cursor) {
    cap_register_t result;
    printf("Decompressing pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", pesbt, cursor);
    decompress_128cap(pesbt, cursor, &result);
    printf("Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
    printf("User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
    printf("Base:        0x%" PRIx64 "\n", result.cr_base);
    printf("Offset:      0x%" PRIx64 "\n", result.cr_offset);
    printf("Length:      0x%" PRIx64 "%016" PRIx64 "\n", (uint64_t)(result._cr_length >> 64), (uint64_t)result._cr_length);
    printf("Sealed:      %d\n", (int)cc128_is_cap_sealed(&result));
    printf("OType:      0x%" PRIx32 "\n", result.cr_otype);
    printf("\n");
    // Check that the result is the same again when compressed
    uint64_t new_pesbt = compress_128cap(&result);
    check(pesbt, new_pesbt, "recompressing resulted in different pesbt");
    check(cursor, result.cr_base + result.cr_offset, "recompressing resulted in different cursor");
    return result;
}

#define DO_STRINGIFY2(x) #x
#define DO_STRINGIFY1(x) DO_STRINGIFY2(x)
#define STRINGIFY(x) DO_STRINGIFY1(x)

#define CHECK_FIELD_RAW(value, expected) check(expected, value, "Line " STRINGIFY(__LINE__) ": " #value " is wrong")
#define CHECK_FIELD(cap, field, expected) CHECK_FIELD_RAW((uint64_t)cap.cr_##field, (uint64_t)expected)
