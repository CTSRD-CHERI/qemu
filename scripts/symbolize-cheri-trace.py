#!/usr/bin/env python3
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

address_regex = re.compile(b"^\\[[\\w\\d:]+\\]\\s+(0x[a-f0-9]+):")
# UNKNOWN_ADDRESS = b"??:0\n"
UNKNOWN_ADDRESS = b"?? at ??:0:0\n"


def yamon_symbol_name(name: bytes, start_addr: int, addr: int):
    offset = str(addr - start_addr).encode("utf-8")
    return b"YAMON " + name + b" + " + offset + b"\n"


def symbolize_line(llvm_symbolizer: subprocess.Popen, line: bytes) -> bytes:
    # print(line, file=sys.stderr)
    if not line.startswith(b"["):
        return line
    match = address_regex.match(line)
    # print("Match=", match, file=sys.stderr)
    if not match:
        return line
    # print("Group=", match.group(1), file=sys.stderr)
    address = match.group(1)
    llvm_symbolizer.stdin.write(address + b"\n")
    llvm_symbolizer.stdin.flush()
    symbol = llvm_symbolizer.stdout.readline()
    next_line = llvm_symbolizer.stdout.readline()
    while next_line != b"\n":
        # Could print an additional line (or multiple) containing (inlined by)
        assert next_line.startswith(b" (inlined by)"), b"Unexpected llvm-symbolizer output: " + next_line
        symbol = symbol + b"\t\t\t\t" + next_line  # ensure the trailing newline is removed from symbol (but not second_line)
        next_line = llvm_symbolizer.stdout.readline()
    # Now the remaining line must be empty:
    assert next_line == b"\n", b"Should have been followed by an empty line but got: " + next_line
    # Also symbolize bootloader addresses
    if symbol == UNKNOWN_ADDRESS:
        addrhex = int(address, 16)
        if 0xffffffffbfc00580 <= addrhex < 0xffffffffbfc00800:
            symbol = yamon_symbol_name(b"start", 0xffffffffbfc00580, addrhex)
        elif 0xffffffffbfc00800 <= addrhex < 0xffffffffbfc00808:
            symbol = b"YAMON unimplemented\n"
        elif 0xffffffffbfc00808 <= addrhex < 0xffffffffbfc0083c:
            symbol = yamon_symbol_name(b"print", 0xffffffffbfc00808, addrhex)
        elif 0xffffffffbfc0083c <= addrhex < 0xffffffffbfc00870:
            symbol = yamon_symbol_name(b"print_count", 0xffffffffbfc0083c, addrhex)
        elif 0xffffffffbfc00870 <= addrhex < 0xffffffffbfc00898:
            symbol = yamon_symbol_name(b"outch", 0xffffffffbfc00870, addrhex)

    length = len(line)
    if length < 30:
        separator = b"\t\t\t\t# "
    elif length < 37:
        separator = b"\t\t\t# "
    elif length < 43:
        separator = b"\t\t# "
    else:
        separator = b"\t# "
    return line[:-1] + separator + symbol


def default_addr2line():
    val = os.getenv("LLVM_SYMBOLIZER")
    if not val:
        in_sdk = os.path.join(os.getenv("CHERI_SDK", "/"), "llvm-symbolizer")
        if os.path.isfile(in_sdk):
            return in_sdk
    if not val:
        val = shutil.which("addr2line")
    return val


if __name__ == "__main__":
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--llvm-symbolizer", metavar="LLVM_SYMBOLIZER", help="Path to llvm-symbolizer", default=default_addr2line())
    parser.add_argument("LOGFILE", help="The LOGFILE to symbolize")
    # TODO: allow multiple (kernel + userspace?)
    parser.add_argument("BINARY", help="The binary to use for symbols to pass to the binary")
    args = parser.parse_args()

    with Path(args.LOGFILE).open("rb") as logfile:
        with subprocess.Popen([args.llvm_symbolizer, "-obj=" + args.BINARY,
                                "-pretty-print", "-demangle", "--output-style=LLVM"],
                              stdout=subprocess.PIPE, stdin=subprocess.PIPE) as llvm_symbolizer:
            for line in logfile.readlines():
                sys.stdout.buffer.write(symbolize_line(llvm_symbolizer, line))
