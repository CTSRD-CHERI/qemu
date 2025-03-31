function doCSetBounds(input: Capability, req_len: CapLenBits) -> (bool, Capability) = {
  let newBase = input.address;
  let newTop : CapLenBits = zero_extend(newBase) + req_len;
  let inBounds = inCapBounds(input, newBase, unsigned(req_len));

  let inCap = clearTagIfSealed(input);
  let (exact, newCap) = setCapBounds(inCap, newBase, newTop);
  (exact, clearTagIf(newCap, not(inBounds)))
}

register TestCap : Capability
register TestAddr : CapAddrBits
register TestLen : CapLenBits

function main () : unit -> unit = {
  TestCap = null_cap;
  let bits1 = capToBits(TestCap);
  let bits2 = capToMemBits(TestCap);
  let cap1 = capBitsToCapability(TestCap.tag, bits1);
  let cap2 = memBitsToCapability(TestCap.tag, bits2);
  let rep = fastRepCheck(TestCap, TestAddr);
  let (exact, cap) = doCSetBounds(TestCap, TestLen);
  let (representable, cap) = setCapAddr(TestCap, TestAddr);
  let (base, top) = getCapBounds(TestCap);
  let flags = getCapFlags(TestCap);
  let new_cap = setCapPerms(TestCap, ones()); /* rt_val[30..0] */
  let cap_str = capToString(TestCap);
  let len = getRepresentableLength(TestAddr);
  let mask = getRepresentableAlignmentMask(TestAddr);
}
