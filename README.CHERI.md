Qemu-CHERI
==========

CHERI ISA support for Qemu.


Building and Installing qemu-system-cheri (recommended)
-----------------------------------------
The recommended way of building QEMU uses cheribuild (http://github.com/CTSRD-CHERI/cheribuild).
To build and install QEMU you can run `cheribuild.py qemu`.
By default this will install QEMU for CHERI-RISC-V and CHERI-MIPS to `$HOME/cheri/output/sdk/bin/qemu-system-<arch>`.
For information on how to customize install and build directory paths please read the [cheribuild README](http://github.com/CTSRD-CHERI/cheribuild)


Building and Installing qemu-system-cheri manually (not recommended)
-----------------------------------------

These notes are based on using a Mac OS X for the host system. Most
of this should apply to a FreeBSD or Linux host as well.

1. Install the Xcode toolchain and homebrew. Install the following
   dependencies, if you haven't already:

```
    $ brew install pkg-config

    $ brew install libtool

    $ brew install glib

    $ brew link glib
```

2. Determine the commands that need to be run by executing `cheribuild.py qemu --pretend` and copying those.
