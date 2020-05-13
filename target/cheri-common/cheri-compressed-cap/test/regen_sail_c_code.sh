#!/bin/sh

set -xe
command -v sail || "Cannot find sail binary"
if [ -z "$SAIL_RISCV_DIR" ]; then
  echo "Must set SAIL_RISCV_DIR"
  exit 1
fi

sail_srcs="sail-riscv/model/prelude.sail sail-riscv/model/prelude_mapping.sail sail-riscv/model/riscv_xlen64.sail sail-riscv/model/riscv_flen_D.sail src/cheri_prelude.sail src/cheri_types.sail src/cheri_prelude_128.sail src/cheri_cap_common.sail"
output_dir=$(pwd)
cd "$SAIL_RISCV_DIR"
# -c_no_rts
sail -c -c_no_main -c_prefix sailgen_ -c_specialize -c_preserve -verbose -o "$output_dir/sail_compression_128" $sail_srcs "$output_dir/compression_test.sail"
cd "$output_dir"
cp /Users/alex/cheri/output/sdk/opamroot/4.06.1/share/sail/lib/sail.h .
cp /Users/alex/cheri/output/sdk/opamroot/4.06.1/share/sail/lib/sail.c .
