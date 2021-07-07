#!/usr/bin/env python3

# This script can annotate a trace produced by -d instr with source code
# Usage: annotrace.py binary trace

import subprocess
import sys
import os
import getopt

default_bin_dir = "~/cheri/output/morello-sdk/bin"
# Trace displayed in two columns, the first of which is this width
default_trace_width = 100

def create_addr2line(bin_dir, bin):
    command = default_bin_dir + "/addr2line -e " + bin
    return subprocess.Popen(
        command,
        shell=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )

def addr2line(proc, addr):
    proc.stdin.write(f"{addr}\n".encode("utf-8"))
    proc.stdin.flush()
    line = proc.stdout.readline().decode("utf-8")

    split = line.rstrip("\n").split(":")

    if (len(split) != 2 or split[0] == "??"):
        return None
    else:
        return (split[0], split[1])

def get_file_lines(path):
    try:
        file = open(path)
        lines = file.read().splitlines()
        return lines
    except Exception as e:
        return None

file_cache = {}

# A heuristic to see if this line terminates a statement
def terminates(line, n):
    if n > 5:
        return True
    stripped = line.split("//")[0].split("/*")[0].strip("\n \t")
    return stripped.endswith("}") or stripped.endswith("{") or stripped.endswith(";")

def lookup_line(line):
    (path, num) = line
    if not path in file_cache:
        file_cache[path] = get_file_lines(path)
    lines = file_cache[path]

    num_int = int(num) - 1

    line_max = len(lines)
    if lines and line_max > num_int:
        line = lines[num_int]
        num_int +=1
        n = 0
        result = [(num).ljust(5) + ":" + line]
        while (num_int != line_max) and not(terminates(line, n)):
            n +=1
            line = lines[num_int]
            result.append("     " + line)
        return result
    else:
        return ["lookup fail"]

def annotrace(args):
    bin = args[0]
    trace = args[1]

    proc = create_addr2line(default_bin_dir, bin)

    file_in = open(trace)

    last = ("undef", 0)
    spill_lines = []
    pad = "".ljust(default_trace_width)

    for line_in in file_in:
        if line_in.startswith("["):
            addr = line_in.split(" ")[1].rstrip(":")
            a2l = addr2line(proc, addr)

            new_spills = []
            if a2l:
                if a2l != last:
                    new_spills = lookup_line(a2l)
            else:
                a2l = ("??? Unknown file ???", 0)
                new_spills = []

            if (a2l != last):
                for sl in spill_lines:
                    print(pad + sl)
                spill_lines = new_spills

            if (a2l[0] != last[0]):
                fp = (a2l[0] + ":").rjust(default_trace_width, ".")
                print("\n" + fp)

            last = a2l

        if len(spill_lines) > 0:
            code_line = spill_lines.pop(0)
        else:
            code_line = "     ."
        line_out = line_in.rstrip("\n").ljust(default_trace_width) + code_line

        print(line_out)
if __name__ == "__main__":
    annotrace(sys.argv[1:])
