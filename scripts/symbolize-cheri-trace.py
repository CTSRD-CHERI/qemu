#!/usr/bin/env python3
import argparse
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

address_regex = re.compile(b"^(0x[a-f0-9]+):")
UNKNOWN_ADDRESS = b"??:0\n"


def yamon_symbol_name(name: bytes, start_addr: int, addr: int):
    offset = str(addr - start_addr).encode("utf-8")
    return b"YAMON " + name + b" + " + offset + b"\n"


def symbolize_line(addr2line: subprocess.Popen, line: bytes) -> bytes:
    if not line.startswith(b"0x"):
        return line
    match = address_regex.match(line)
    if not match:
        return line
    address = match.group(1)
    addr2line.stdin.write(address + b"\n")
    addr2line.stdin.flush()
    symbol = addr2line.stdout.readline()
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
    val = os.getenv("ADDR2LINE")
    if not val:
        in_sdk = os.path.join(os.getenv("CHERI_SDK", "/"), "addr2line")
        if os.path.isfile(in_sdk):
            return in_sdk
    if not val:
        val = shutil.which("addr2line")
    return val


if __name__ == "__main__":
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--addr2line", metavar="ADDR2LINE", help="Path to addr2line", default=default_addr2line())
    parser.add_argument("LOGFILE", help="The LOGFILE to symbolize")
    # TODO: allow multiple (kernel + userspace?)
    parser.add_argument("BINARY", help="The binary to use for symbols to pass to the binary")
    args = parser.parse_args()

    with Path(args.LOGFILE).open("rb") as logfile:
        with subprocess.Popen([args.addr2line, "-e", args.BINARY], stdout=subprocess.PIPE,
                              stdin=subprocess.PIPE) as addr2line:
            for line in logfile.readlines():
                sys.stdout.buffer.write(symbolize_line(addr2line, line))
