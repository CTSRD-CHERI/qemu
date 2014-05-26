/*
 *  freebsd ELF definitions
 *
 *  Copyright (c) 2013 Stacey D. Son
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TARGET_OS_ELF_H_
#define _TARGET_OS_ELF_H_

#include "target_arch_elf.h"
#include "elf.h"

/* this flag is uneffective under linux too, should be deleted */
#ifndef MAP_DENYWRITE
#define MAP_DENYWRITE 0
#endif

/* should probably go in elf.h */
#ifndef ELIBBAD
#define ELIBBAD 80
#endif

#ifndef ELF_PLATFORM
#define ELF_PLATFORM (NULL)
#endif

#ifndef ELF_HWCAP
#define ELF_HWCAP 0
#endif

#ifdef TARGET_ABI32
#undef ELF_CLASS
#define ELF_CLASS ELFCLASS32
#undef bswaptls
#define bswaptls(ptr) bswap32s(ptr)
#endif

struct exec
{
  unsigned int a_info;   /* Use macros N_MAGIC, etc for access */
  unsigned int a_text;   /* length of text, in bytes */
  unsigned int a_data;   /* length of data, in bytes */
  unsigned int a_bss;    /* length of uninitialized data area, in bytes */
  unsigned int a_syms;   /* length of symbol table data in file, in bytes */
  unsigned int a_entry;  /* start address */
  unsigned int a_trsize; /* length of relocation info for text, in bytes */
  unsigned int a_drsize; /* length of relocation info for data, in bytes */
};


#define N_MAGIC(exec) ((exec).a_info & 0xffff)
#define OMAGIC 0407
#define NMAGIC 0410
#define ZMAGIC 0413
#define QMAGIC 0314

/* max code+data+bss space allocated to elf interpreter */
#define INTERP_MAP_SIZE (32 * 1024 * 1024)

/* max code+data+bss+brk space allocated to ET_DYN executables */
#define ET_DYN_MAP_SIZE (128 * 1024 * 1024)

/* Necessary parameters */
#define TARGET_ELF_EXEC_PAGESIZE TARGET_PAGE_SIZE
#define TARGET_ELF_PAGESTART(_v) ((_v) & \
        ~(unsigned long)(TARGET_ELF_EXEC_PAGESIZE-1))
#define TARGET_ELF_PAGEOFFSET(_v) ((_v) & (TARGET_ELF_EXEC_PAGESIZE-1))

#define INTERPRETER_NONE 0
#define INTERPRETER_AOUT 1
#define INTERPRETER_ELF 2

#define DLINFO_ITEMS 12

static abi_ulong target_create_elf_tables(abi_ulong p, int argc, int envc,
                                   abi_ulong stringp,
                                   struct elfhdr * exec,
                                   abi_ulong load_addr,
                                   abi_ulong load_bias,
                                   abi_ulong interp_load_addr, int ibcs,
                                   struct image_info *info)
{
        abi_ulong sp;
        int size;
        const int n = sizeof(elf_addr_t);

        sp = p;
        /*
         * Force 16 byte _final_ alignment here for generality.
         */
        sp = sp &~ (abi_ulong)15;
        size = (DLINFO_ITEMS + 1) * 2;
        size += envc + argc + 2;
        size += (!ibcs ? 3 : 1);        /* argc itself */
        size *= n;
        if (size & 15)
            sp -= 16 - (size & 15);

        /* This is correct because Linux defines
         * elf_addr_t as Elf32_Off / Elf64_Off
         */
#define NEW_AUX_ENT(id, val) do {               \
            sp -= n; put_user_ual(val, sp);     \
            sp -= n; put_user_ual(id, sp);      \
          } while(0)

        NEW_AUX_ENT (AT_NULL, 0);

        /* There must be exactly DLINFO_ITEMS entries here.  */
        NEW_AUX_ENT(AT_PHDR, (abi_ulong)(load_addr + exec->e_phoff));
        NEW_AUX_ENT(AT_PHENT, (abi_ulong)(sizeof (struct elf_phdr)));
        NEW_AUX_ENT(AT_PHNUM, (abi_ulong)(exec->e_phnum));
        NEW_AUX_ENT(AT_PAGESZ, (abi_ulong)(TARGET_PAGE_SIZE));
        NEW_AUX_ENT(AT_BASE, (abi_ulong)(interp_load_addr));
        NEW_AUX_ENT(AT_FLAGS, (abi_ulong)0);
        NEW_AUX_ENT(AT_ENTRY, load_bias + exec->e_entry);
#ifndef TARGET_PPC
        NEW_AUX_ENT(AT_UID, (abi_ulong) getuid());
        NEW_AUX_ENT(AT_EUID, (abi_ulong) geteuid());
        NEW_AUX_ENT(AT_GID, (abi_ulong) getgid());
        NEW_AUX_ENT(AT_EGID, (abi_ulong) getegid());
#endif
#ifdef ARCH_DLINFO
        /*
         * ARCH_DLINFO must come last so platform specific code can enforce
         * special alignment requirements on the AUXV if necessary (eg. PPC).
         */
        ARCH_DLINFO;
#endif
#undef NEW_AUX_ENT

        sp = loader_build_argptr(envc, argc, sp, stringp, !ibcs);
        return sp;
}

#endif /* _TARGET_OS_ELF_H_ */
