function doCSetBounds(input: Capability, req_len: CapLenBits) -> (bool, Capability) = {
  let newBase = input.address;
  let newTop : CapLenBits = zero_extend(newBase) + req_len;
  // inCapBoundsNoWrap returns false if the input bounds are malformed.
  let inBounds = inCapBoundsNoWrap(input, newBase, unsigned(req_len));
  let (exact, newCap) : (bool, Capability) = setCapBounds(input, newBase, newTop);
  let cond = not(inBounds & exact) |
             boundsMalformed(newCap) |
             not(capReservedValid(newCap)) |
             capIsSealed(newCap);
  let (exact, newCap) = setCapBounds(input, newBase, newTop);
  (exact, clearTagIf(newCap, cond))
}

register TestCap : Capability
register TestAddr : CapAddrBits
register TestLen : CapLenBits

function main () : unit -> unit = {
  TestCap = null_cap;
  let cap = bitsToCap(TestCap.tag, TestAddr @ TestAddr);
  let cap_bits = capToMetadataBits(cap).bits;
  let (exact, cap) = doCSetBounds(TestCap, TestLen);
  let (representable, cap) = setCapAddr(TestCap, TestAddr);
  let bounds = getCapBounds(TestCap);
  let mode = getCapMode(TestCap);
  let cap_str = capToString(TestCap);
  let new_cap = setCapPermsAndMode(TestCap, (getArchPermsLegalized(TestCap), getCapMode(TestCap)));
  let perms_mode = getCapPermsAndMode(TestCap);
  let len = getRepresentableLength(TestAddr);
  let mask = getRepresentableAlignmentMask(TestAddr);
}
