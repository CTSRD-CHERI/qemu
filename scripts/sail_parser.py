#!/usr/bin/env python3

# TODO: License

# Generate a specification file from sail by pattern matching on a function.
# Based on the pattern used to specify decode on ARM Morello.

# Usage: sail_parser [output_file] [input_file]

#Functions look a bit like this:

#function clause __DecodeA64 ((pc, ([bitone, bitone, bitzero, bitzero, bitzero, bitzero, bitone, bitzero, bitone, bitone, bitzero, bitzero, bitzero, bitzero, bitone, bitzero, bitzero, bitzero, bitone, bitone, bitzero, bitzero, _, _, _, _, _, bitzero, bitzero, bitzero, bitone, bitone] as opcode)) if SEE < 54) = {
#    SEE = 54;
#let opc = Slice(opcode, 13, 2);
#let Cn = Slice(opcode, 5, 5);
#decode_BLRR_C_C(opc, Cn)
#}

import sys
import re
from difflib import SequenceMatcher

# The name of the decode function
DocodeFunName = "__DecodeA64"
# Instruction names are prefixed with this
InstrNamePrefix = "decode_"
# Must have at least one occurance of this suffix to be a capability instruction
IstrNameSuffix = "_C"
# Or contain the following prefix (because one two instructions dont includes any caps but are cap instructions)
InstrNamesInclude = "((ALD)|(AST)|(STCT)|(LDCT))"

def print_usage():
    print("Usage: sail_parser [input_file] [output_file]")

def error(fun, message):
    print("Error parsing function: " + message)
    print(fun)

bitPatternExpr = re.compile(r'\[(.*?)\]')
sliceExpr = re.compile(r'let (.*?) = Slice\(.*?, ([0-9]*), ([0-9]*)\)')
intrDecodeExpr = re.compile(InstrNamePrefix + '([^_]*).*?' + IstrNameSuffix)
intrDecodeExpr2 = re.compile(InstrNamePrefix + '(' + InstrNamesInclude + '[^_]*)')
# Map of argument patterns to the rest of the instruction definition
decoded = {}

def add_to_set(name, bits, varLS):
    varTup = tuple(varLS)
    if varTup in decoded:
        decoded[varTup].append((name, bits))
    else:
        decoded[varTup] = [(name, bits)]

def match(str1,str2):
    seqMatch = SequenceMatcher(None,str1,str2)
    matches = seqMatch.get_matching_blocks()
    str = ""
    len = 0
    largestBlock = 0
    for match in matches:
        str += str1[match.a: match.a + match.size]
        len += match.size
        if(match.size > largestBlock):
            largestBlock = match.size
    return (str, len, largestBlock)

def fineCommonNames(nameList, newName):
    index = 0
    common_length = 0
    common_index = -1
    common_str = ""
    for name in nameList:
        (common, length, largestBlock) = match(newName, name)
        if (largestBlock >= 2) and (length > common_length):
            common_str = common
            common_index = index
            common_length = length
        index +=1

    if common_length >= 2:
        nameList[common_index] = common_str
    else:
        nameList.append(newName)
        common_index = index

    return common_index

def patternMatch(p1, p2):
    for i in range(32):
        if not(p1[i] == p2[i] or p1[i] == '.' or p2[i] == "."):
            return False
    return True

comments = True

ignore_ls = {
    "STXP" : -1,
    "STXR" : -1,
    "STLXP" : -1,
    "STLXR" : -1,
    "ALDUR": -1,
    "ASTUR": -1,
    "ASTR": [0,1,2,4,5,6,7],
    "ALDR": [0],
    "LDAR": -1,
    "STLR": -1,
    "LDAXR" : -1,
    "LDXP" : -1,
    "LDAXP" : -1,
    "ALDAR" : -1,
    "ASTLR" : -1,
    "LDXR" : -1,
    "ALDARB" : -1,
    "ASTLRB" : -1
}

extraOpts = {
    ("FLGS", 3) : [("op", 13, 2)],
    ("BLR", 11)  : [("op", 2, 2)],
    ("LDR", 8) : [("op", 12, 2)],
    ("LDP", 8) : [("op1", 24, 1), ("op2", 31, 2)],
    ("ASTR", 12) : [("op", 22, 1)],
    ("BR", 2) : [("link", 1, 1)],
}

def finalise(file):
    names_seen = {}
    ops_seen = {}
    # Things that were not very well encoded in the sail

    # Map from patterns in strings in groups
    groups = []
    tot = 0
    result = ""
    for varLS in decoded:
        # Find some heuristic if this are obviously different function
        instrs = decoded[varLS]
        varLS = list(varLS)

        # Find a common name
        nameList = [instrs[0][0]]
        nameMap = []
        for (name, bits) in instrs:
            nameMap.append(fineCommonNames(nameList, name))

        for name in nameList:
            l = len(instrs)
            if (name,l) in extraOpts:
                for extra in extraOpts[(name,l)]:
                    ndx = 0
                    found = False
                    for (name, end, length) in varLS:
                        if(extra[1] > end):
                            varLS.insert(ndx, extra)
                            found = True
                            break
                        ndx +=1
                    if not found:
                        varLS.append(extra)

        commonName = '_'.join(nameList)

        if commonName in ops_seen:
            ops_seen[commonName] +=1
            commonName += str(ops_seen[commonName])
        else:
            ops_seen[commonName] = 0
        # Print the pattern
        result += "@op_" + commonName + "    "

        lastEnd = 32
        for (name, end, n) in varLS:
            for i in range(lastEnd-end):
                result += "."
                lastEnd-=1
            result += " " + name + ":" + str(n) + " "
            lastEnd -= n
        while lastEnd != 0:
            lastEnd -=1
            result += "."

        result += "\n"

        seen = {}
        # Calculate instruction patterns as long as they differ
        ndx = 0
        toOutput = []
        for (instrName, bits) in instrs:
            ndx +=1
            pattern = ""
            lastEnd = 32
            covered = 0
            for bit in bits:
                if covered != 0:
                    pattern += '.'
                    covered -=1
                    lastEnd -=1
                    continue

                inVar = False
                for [name, end, n] in varLS:
                    if end == lastEnd:
                        pattern += '.'
                        covered = n-1
                        inVar = True
                        continue

                bit = bit.strip()
                if not inVar:
                    if bit == '_':
                        pattern += '_'
                    elif bit == 'bitone':
                        pattern += '1'
                    elif bit == 'bitzero':
                        pattern += '0'

                lastEnd -=1

            if pattern in seen:
                toOutput[seen[pattern]][2].append(instrName)
                continue

            seen[pattern] = len(toOutput)
            useName = nameList[nameMap[ndx-1]]

            if useName in names_seen:
                names_seen[useName] += 1
                useName += str(names_seen[useName])
            else:
                names_seen[useName] = 0

            toOutput.append((useName, pattern, [instrName]))

        for (useName, pattern, allNames) in toOutput:
            print(useName, allNames)
            if len(toOutput) == 1 and len(nameList) != 1:
                useName = commonName
                if useName in names_seen:
                    names_seen[useName] += 1
                    useName += str(names_seen[useName])
                else:
                    names_seen[useName] = 0

            instrStr = []
            if comments:
                instrStr += ["",("# " + ", ".join(allNames))]
            instrStr.append(useName + " " + pattern + " @op_" + commonName)

            matched = False
            for (pats,instrStrs) in groups:
                for pat in pats:
                    if patternMatch(pattern, pat):
                        pats.append(pattern)
                        instrStrs.append(instrStr)
                        matched = True
                        break
                if matched:
                    break
            if not matched:
                groups.append(([pattern],[instrStr]))

            tot +=1
    result += "\n"
    # Now group instructions
    for (_,instrStrs) in groups:
        if len(instrStrs) != 1:
            result+= "{\n"
            for instrStr in instrStrs:
                result += "  " + "\n  ".join(instrStr) + "\n";
            result += "}\n"
        else:
            result += "\n".join(instrStrs[0]) + "\n"
    # Now output to file
    file.write(result)
    return tot

ignore_seen = {}

def should_ignore(name):
    if not name in ignore_ls:
        return False
    ns = ignore_ls[name]
    if ns == -1:
        return True
    if not name in ignore_seen:
        ignore_seen[name] = 0

    found_n = ignore_seen[name]
    ignore_seen[name] +=1

    return found_n in ns

def handle_fun(file, fun):
    # Grab the name of the function. Ignore those of the wrong form (probbaly not Morello instrs)
    instrMatch = intrDecodeExpr.search(fun)
    if instrMatch == None:
        instrMatch = intrDecodeExpr2.search(fun)
        if(instrMatch == None):
            return False

    instrName = instrMatch.group(1)

    if(should_ignore(instrName)):
        print("Ignoring" + instrName)
        return

    print("Procesing " + instrName)

    # Now grab the bit pattern
    bitsMatch = bitPatternExpr.search(fun)
    if bitsMatch == None:
        error(fun, "Function did have a bit pattern as expected")
        return False
    bits = bitsMatch.group(1).split(',')

    if (len(bits) != 32):
        return False

    varLS = []
    for sliceMatch in sliceExpr.finditer(fun):
        name = sliceMatch.group(1)
        start = int(sliceMatch.group(2))
        n = int(sliceMatch.group(3))
        varLS.append((name, start+n, n))


    # Group instructions with the same argument patterns if possible, if they only differ in few other bits

    add_to_set(instrName, bits, varLS)

    return False

def output_premable(file):
    file.write("# AUTO-GENERATED. DO NOT EDIT.\n")

def main():
    if len(sys.argv) != 3:
        print_usage()
        return

    pathIn = sys.argv[1]
    pathOut = sys.argv[2]

    with open (pathIn, 'r') as myfile:
        data = myfile.read()

    expr = re.compile('function clause ' + DocodeFunName + '.*?{.*?' + InstrNamePrefix + '.*?}', flags=re.DOTALL)

    total_success = 0
    total_found = 0

    with open(pathOut, 'w') as outFile:
        output_premable(outFile)
        for s in expr.findall(data):
            total_found +=1
            if handle_fun(outFile, s):
                total_success +=1

        tot = finalise(outFile)

    print("Total functions found = " + str(total_found) + ". Total successfully parsed = " + str(total_success) + ".")
    print("Total instructions output = " + str(tot))

if __name__ == '__main__':
    main()
