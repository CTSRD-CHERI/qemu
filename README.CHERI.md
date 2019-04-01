Qemu-CHERI
==========

CHERI ISA support for Qemu.


Building and Installing qemu-system-cheri (recommended)
-----------------------------------------
The recommended way of building QEMU uses cheribuild (http://github.com/CTSRD-CHERI/cheribuild).
To build and install QEMU you can run `cheribuild.py qemu`.
By default this will install to `$HOME/cheri/output/sdk/bin/qemu-system-cheri128`.
For information on how to customize install and build directory paths please read the [cheribuild README](http://github.com/CTSRD-CHERI/cheribuild)




Building and Installing qemu-system-cheri (manually)
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

2. Clone the qemu-cheri repository (which should default to the
   'qemu-cheri' branch):

```
    $ git clone http://github.com/CTSRD-CHERI/qemu
```

3. Configure qemu. This only configures the 'qemu-system-cheri'
   target and assumes that it will be installed in '/usr/local'.

```
    $ cd qemu

    $ ./configure --target-list=cheri-softmmu --prefix=/usr/local --disable-linux-user --disable-linux-aio --disable-kvm --disable-xen --extra-cflags=-g
```

3. Make and install:

```
    $ make

    $ make install
```
