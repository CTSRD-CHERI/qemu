#include "../cheri_compressed_cap.h"
#include <cinttypes>
#include <cstdint>
#include <cstdlib>
#include <iostream>

static bool failed = false;

template <typename T> static void check(T expected, T actual, const char* msg) {
    if (expected == actual)
        return;
    std::cerr << "ERROR: " << msg << ": 0x" << std::hex << actual << " != 0x" << expected << "\n";
    failed = true;
}

#define CHECK_FIELD(cap, field, expected) check((uint64_t)expected, (uint64_t)cap.cr_##field, #field " is wrong")

static cap_register_t decompress_representable(uint64_t pesbt, uint64_t cursor) {
    cap_register_t result;
    printf("Decompressing pesbt = %016" PRIx64 ", cursor = %016" PRIx64 "\n", pesbt, cursor);
    decompress_128cap(pesbt, cursor, &result);
    printf("Permissions: 0x%" PRIx32 "\n", result.cr_perms); // TODO: decode perms
    printf("User Perms:  0x%" PRIx32 "\n", result.cr_uperms);
    printf("Base:        0x%" PRIx64 "\n", result.cr_base);
    printf("Offset:      0x%" PRIx64 "\n", result.cr_offset);
    printf("Length:      0x%" PRIx64 "\n", result.cr_length);
    printf("Sealed:      %d\n", (int)cc128_is_cap_sealed(&result));
    if (cc128_is_cap_sealed(&result)) {
        printf("OType:      0x%" PRIx32 "\n", result.cr_otype);
    }
    printf("\n");
    // Check that the result is the same again when compressed
    uint64_t new_pesbt = compress_128cap(&result);
    check(pesbt, new_pesbt, "recompressing resulted in different pesbt");
    check(cursor, result.cr_base + result.cr_offset, "recompressing resulted in different cursor");
    return result;
}

static void test1() {
    auto result = decompress_representable(0xffae000000000000, 0x9000000040001650);
    CHECK_FIELD(result, base, 0);
    CHECK_FIELD(result, offset, 0x9000000040001650);
    CHECK_FIELD(result, uperms, 0xf);
    CHECK_FIELD(result, perms, 0x7d7);
    CHECK_FIELD(result, sealed, 0);
    CHECK_FIELD(result, length, UINT64_MAX);
    CHECK_FIELD(result, otype, 0);
}

static void test2() {
    auto result = decompress_representable(0xfffa200b1f001633, 0x9000000040001636);
    CHECK_FIELD(result, base, 0x9000000040001636);
    CHECK_FIELD(result, offset, 0);
    CHECK_FIELD(result, uperms, 0xf);
    CHECK_FIELD(result, perms, 0x7fd);
    CHECK_FIELD(result, length, 6);
    CHECK_FIELD(result, sealed, 0);
    CHECK_FIELD(result, otype, 0);
}

static void test3() {
    auto result = decompress_representable(0xfffa201a19003035, 0x9000000040003296);
    CHECK_FIELD(result, base, 0x9000000040003030);
    CHECK_FIELD(result, offset, 0x266);
    CHECK_FIELD(result, uperms, 0xf);
    CHECK_FIELD(result, perms, 0x7fd);
    CHECK_FIELD(result, length, 0x400);
    CHECK_FIELD(result, sealed, 0);
    CHECK_FIELD(result, otype, 0);
}

// 4 and 5 are from test_cp2_cincoffset_rep_underflow.log
static void test4() {
    // 0x9000000040000f6c:  cincoffset c2,c1,t4
    //    Write C02|v:1 s:0 p:00007fff b:0000000000000000 l:0000000130000000
    //             |o:0000000000001010 t:0
    // 0x9000000040000f70:  csc        c2,a1,0(c18)
    //    Cap Memory Write [9000000040001030] = v:1 PESBT:fffe030000000004 Cursor:0000000000001010
    auto result = decompress_representable(0xfffe030000000004, 0x1010);
    CHECK_FIELD(result, base, 0);
    CHECK_FIELD(result, offset, 0x1010);
    CHECK_FIELD(result, uperms, 0xf);
    CHECK_FIELD(result, perms, 0x7ff);
    CHECK_FIELD(result, length, 0x130000000);
    CHECK_FIELD(result, sealed, 0);
    CHECK_FIELD(result, otype, 0);
}

static void test5() {
    // 0x9000000040000f88:  cincoffset c2,c1,t4
    //    Write C02|v:1 s:0 p:00007fff b:0000000000000000 l:0000000130000000
    //             |o:0000000000000ff0 t:0
    // 0x9000000040000f8c:  csc        c2,a1,64(c18)
    //    Cap Memory Write [9000000040001070] = v:1 PESBT:fffe030000000004 Cursor:0000000000000ff0
    auto result = decompress_representable(0xfffe030000000004, 0xff0);
    CHECK_FIELD(result, base, 0);
    CHECK_FIELD(result, offset, 0xff0);
    CHECK_FIELD(result, uperms, 0xf);
    CHECK_FIELD(result, perms, 0x7ff);
    CHECK_FIELD(result, length, 0x130000000);
    CHECK_FIELD(result, sealed, 0);
    CHECK_FIELD(result, otype, 0);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    return failed ? 1 : 0;
}
