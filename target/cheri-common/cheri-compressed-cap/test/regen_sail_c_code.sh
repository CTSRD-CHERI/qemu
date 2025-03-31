#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
set -xe

if ! command -v sail > /dev/null 2>/dev/null; then
  echo "Could not find sail in PATH."
  exit 1
fi

SAIL_DIR=$(sail -dir)
if [ ! -e "${SAIL_DIR}/lib/sail.c" ]; then
  echo "Invalid sail installation, missing library files"
  exit 1
fi

SAIL_RISCV_V9_DIR=${SAIL_RISCV_V9_DIR:-${HOME}/cheri/sail-cheri-riscv/}
if [ ! -d "$SAIL_RISCV_V9_DIR" ]; then
  echo "Must set SAIL_RISCV_V9_DIR"
  exit 1
fi
SAIL_RISCV_STD_DIR=${SAIL_RISCV_STD_DIR:-${HOME}/cheri/sail-cheri-riscv-std/}
if [ ! -d "$SAIL_RISCV_STD_DIR" ]; then
  echo "Must set SAIL_RISCV_STD_DIR"
  exit 1
fi

sail_pre_srcs="sail-riscv/model/prelude.sail"
xlen64_srcs="sail-riscv/model/riscv_xlen64.sail sail-riscv/model/riscv_flen_D.sail"
xlen32_srcs="sail-riscv/model/riscv_xlen32.sail sail-riscv/model/riscv_flen_F.sail"
sail128_srcs="src/cheri_prelude.sail src/cheri_types.sail src/cheri_prelude_128.sail src/cheri_cap_common.sail"
sail64_srcs="src/cheri_prelude.sail src/cheri_types.sail src/cheri_prelude_64.sail src/cheri_cap_common.sail"
output_dir=${SCRIPT_DIR}/contrib
keep_required_functions=(-c_preserve capToBits
	-c_preserve capToMemBits
	-c_preserve capBitsToCapability
	-c_preserve memBitsToCapability
	-c_preserve fastRepCheck
	-c_preserve setCapAddr
	-c_preserve setCapBounds
	-c_preserve getCapBounds
	-c_preserve getCapFlags
	-c_preserve setCapPerms
	-c_preserve capToString
	-c_preserve doCSetBounds
	-c_preserve getRepresentableLength
	-c_preserve getRepresentableAlignmentMask)

sail_args=(-c -c_no_main -c_prefix sailgen_ -c_specialize -O -Oconstant_fold -verbose=2 -static)
cd "$SAIL_RISCV_V9_DIR"
# -c_no_rts
sail "${sail_args[@]}" "${keep_required_functions[@]}" -o "$output_dir/sail_compression_128" $sail_pre_srcs $xlen64_srcs $sail128_srcs "${SCRIPT_DIR}/compression_test_v9.sail"
sail "${sail_args[@]}" "${keep_required_functions[@]}" -o "$output_dir/sail_compression_64" $sail_pre_srcs $xlen32_srcs $sail64_srcs "${SCRIPT_DIR}/compression_test_v9.sail"
cd "$SAIL_RISCV_STD_DIR"
xlen64_srcs="$xlen64_srcs sail-riscv/model/riscv_xlen.sail"
xlen32_srcs="$xlen32_srcs sail-riscv/model/riscv_xlen.sail"
sail "${sail_args[@]}" "${keep_required_functions[@]}" -o "$output_dir/sail_compression_128r" $sail_pre_srcs $xlen64_srcs $sail128_srcs "${SCRIPT_DIR}/compression_test_std.sail"
sail "${sail_args[@]}" "${keep_required_functions[@]}" -o "$output_dir/sail_compression_64r" $sail_pre_srcs $xlen32_srcs $sail64_srcs "${SCRIPT_DIR}/compression_test_std.sail"
cd "$output_dir"

for i in sail.h sail.c sail_failure.c sail_failure.h elf.h rts.h; do
  cp "${SAIL_DIR}/lib/${i}" .
done
