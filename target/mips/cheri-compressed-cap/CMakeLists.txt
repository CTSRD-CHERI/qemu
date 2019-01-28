cmake_minimum_required(VERSION 3.8)
project(cheri_compressed_cap C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_compile_options(-pedantic -Wall -Wextra -Weverything -Wno-old-style-cast)
add_library(cheri_compressed_cap cheri_compressed_cap.c cheri_compressed_cap.h)

add_executable(decompress_c128_cap decompress_c128_cap.c)
include(GNUInstallDirs)
install(TARGETS decompress_c128_cap RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

enable_testing()
add_executable(simple_test test/simple_test.cpp)
add_test(NAME simple-test COMMAND simple_test)

add_executable(simple_test_old_format test/simple_test.cpp)
target_compile_definitions(simple_test_old_format PRIVATE -DCC128_OLD_FORMAT)
add_test(NAME simple-test-old-format COMMAND simple_test_old_format)
