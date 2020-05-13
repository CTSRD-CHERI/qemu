cmake_minimum_required(VERSION 3.10)
project(cheri_compressed_cap C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
if(APPLE)
    # XXX: ugly hack, would be nice if llvm could find this
    include_directories(/Library/Developer/CommandLineTools/usr/include/c++/v1)
endif()

include(CheckCXXCompilerFlag)
# CMAKE_REQUIRED_LINK_OPTIONS needs cmake 3.13
set(CMAKE_REQUIRED_LIBRARIES -fsanitize=undefined)
check_cxx_compiler_flag(-fsanitize=undefined HAVE_UBSAN)
set(CMAKE_REQUIRED_LIBRARIES -fsanitize=address)
check_cxx_compiler_flag(-fsanitize=address HAVE_ASAN)
set(CMAKE_REQUIRED_LIBRARIES -fsanitize=memory)
check_cxx_compiler_flag(-fsanitize=memory HAVE_MSAN)
set(CMAKE_REQUIRED_LIBRARIES -fsanitize=fuzzer-no-link)
check_cxx_compiler_flag(-fsanitize=fuzzer-no-link HAVE_LIBFUZZER)

if (HAVE_UBSAN)
    add_compile_options(-fsanitize=undefined)
    if (${CMAKE_VERSION} VERSION_GREATER 3.12)
        add_link_options(-fsanitize=undefined)
    else()
        link_libraries(-fsanitize=undefined)
    endif()
endif()

add_compile_options(-pedantic -Wall -Wextra -Wno-old-style-cast -Wno-c++98-compat-pedantic -Wno-c++98-c++11-compat-binary-literal -Wno-c11-extensions -Wno-padded)
# TODO: -Weverything
add_library(cheri_compressed_cap14 cheri_compressed_cap.c cheri_compressed_cap.h)

add_executable(decompress_c128_cap decompress_c128_cap.c)
include(GNUInstallDirs)
install(TARGETS decompress_c128_cap RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Test against sail generated C code:
# This needs GMP (and CMake doesn't include a find module)
find_package(PkgConfig)
pkg_check_modules(PC_GMP gmp)
find_path(GMP_INCLUDE_DIR gmp.h
          HINTS ${PC_GMP_INCLUDEDIR} ${PC_GMP_INCLUDE_DIRS}
          PATH_SUFFIXES gmp)
message(STATUS "GMP include dir: ${GMP_INCLUDE_DIR}")
find_library(GMP_LIBRARY NAMES gmp
             HINTS ${PC_GMPL_LIBDIR} ${PC_GMP_LIBRARY_DIRS})
message(STATUS "GMP library: ${GMP_LIBRARY}")

if (NOT GMP_LIBRARY)
    message(FATAL_ERROR "Cannot find GMP library")
endif()
if (NOT GMP_INCLUDE_DIR)
    message(FATAL_ERROR "Cannot find GMP includes")
endif()
add_library(GMP::GMP UNKNOWN IMPORTED)
set_target_properties(GMP::GMP PROPERTIES
  IMPORTED_LOCATION "${GMP_LIBRARY}"
  INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_DIR}"
)


function(add_sail_wrapper128 _suffix _sanitizer_flags)
    add_library(sail_wrapper128${_suffix} STATIC test/sail_wrapper.c test/sail.c)
    if (_sanitizer_flags)
        target_compile_options(sail_wrapper128${_suffix} PRIVATE "${_sanitizer_flags}")
        target_link_libraries(sail_wrapper128${_suffix} PRIVATE "${_sanitizer_flags}")
    endif()
    target_link_libraries(sail_wrapper128${_suffix} PUBLIC GMP::GMP)  # needs GMP, should propagate to all users
    target_compile_options(sail_wrapper128${_suffix} PRIVATE -Wno-unused-function -Wno-unused-parameter -Wno-sign-compare -Wno-unused-label -Wno-extra-semi -Wno-gnu-binary-literal)
endfunction()

add_sail_wrapper128("" "")
if (HAVE_ASAN)
    add_sail_wrapper128("-asan-ubsan" "-fsanitize=undefined,address")
endif()
if (HAVE_MSAN)
    add_sail_wrapper128("-msan-ubsan" "-fsanitize=undefined,memory")
endif()

# Add a test exe for sail generated code
add_executable(test_sail_wrapper128 decompress_c128_cap.c)
target_compile_definitions(test_sail_wrapper128 PRIVATE DECOMPRESS_WITH_SAIL_GENERATED_CODE=1)
target_link_libraries(test_sail_wrapper128 PRIVATE sail_wrapper128)

enable_testing()

function(add_cc_test _tgt _src)
    if (HAVE_ASAN)
        add_executable(${_tgt}-asan-ubsan ${_src})
        target_compile_options(${_tgt}-asan-ubsan PRIVATE -fsanitize=undefined,address)
        target_link_libraries(${_tgt}-asan-ubsan PRIVATE -fsanitize=undefined,address)
        target_link_libraries(${_tgt}-asan-ubsan PRIVATE sail_wrapper128-asan-ubsan)
        add_test(NAME test-${_tgt}-asan-ubsan COMMAND ${_tgt}-asan-ubsan)
    endif()
    if (HAVE_MSAN)
        add_executable(${_tgt}-msan-ubsan ${_src})
        target_compile_options(${_tgt}-msan-ubsan PRIVATE -fsanitize=undefined,memory)
        target_link_libraries(${_tgt}-msan-ubsan PRIVATE -fsanitize=undefined,memory)
        target_link_libraries(${_tgt}-msan-ubsan PRIVATE sail_wrapper128-msan-ubsan)
        add_test(NAME test-${_tgt}-msan-ubsan COMMAND ${_tgt}-msan-ubsan)
    endif()
    # UBSan will be added if available -> no suffix
    add_executable(${_tgt} ${_src})
    add_test(NAME test-${_tgt} COMMAND ${_tgt})
    target_link_libraries(${_tgt} PRIVATE sail_wrapper128)
endfunction()

add_cc_test(simple_test test/simple_test.cpp)

add_cc_test(setbounds_test test/setbounds_test.cpp)

add_cc_test(random_inputs_test test/random_inputs_test.cpp)

if (HAVE_LIBFUZZER)
    if (HAVE_ASAN)
        add_sail_wrapper128("-fuzzer-asan-ubsan" "-fsanitize=undefined,address,fuzzer")
        add_executable(fuzz_decompress_asan test/fuzz-decompress.cpp)
        target_compile_options(fuzz_decompress_asan PRIVATE -fsanitize=undefined,address,fuzzer)
        target_link_libraries(fuzz_decompress_asan PRIVATE -fsanitize=undefined,address,fuzzer)
        target_link_libraries(fuzz_decompress_asan PRIVATE sail_wrapper128-fuzzer-asan-ubsan)
    endif()
    if (HAVE_MSAN)
        add_sail_wrapper128("-fuzzer-msan-ubsan" "-fsanitize=undefined,memory,fuzzer")
        add_executable(fuzz_decompress_msan test/fuzz-decompress.cpp)
        target_compile_options(fuzz_decompress_msan PRIVATE -fsanitize=undefined,memory,fuzzer)
        target_link_libraries(fuzz_decompress_msan PRIVATE -fsanitize=undefined,memory,fuzzer)
        target_link_libraries(fuzz_decompress_msan PRIVATE sail_wrapper128-fuzzer-msan-ubsan)
    endif()
endif()
