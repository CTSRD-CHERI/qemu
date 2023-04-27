/* Functions we want to test: */
register TestCap : bits(129)

function main () : unit -> unit = {
  TestCap = Zeros(129);
  /* while (true) do { */
      let bits1 = CapGetValue(TestCap);
      let bits2 = CapGetPermissions(TestCap);
      let bits3 = CapGetObjectType(TestCap);
      let bits4 = CapGetTag(TestCap);
      let bits5 = CapSetBounds (TestCap, Zeros(65), true);
      let bits6 = CapGetBounds(TestCap);
  /* } */
}
