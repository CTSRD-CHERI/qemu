/*
 *  freebsd ELF definitions
 *
 *  Copyright (c) 2013-15 Stacey D. Son
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

#include "bsd-proc.h"
#ifdef TARGET_CHERI
#include "cheri/cherireg.h"
#include "cheri/cheric.h"
#include "cheri/cheri.h"
#endif

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

/* XXX Look at the other conflicting AT_* values. */
#define FREEBSD_AT_NCPUS     19
#define FREEBSD_AT_HWCAP     25
#define FREEBSD_AT_HWCAP2    26

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

#define DLINFO_ITEMS 14

#ifdef TARGET_CHERI
/*
 * prog_cap() is ported from sys/kern/imgact_elf.c in CTSRD-CHERI/CheriBSD.
 */
static cap_register_t *prog_cap(struct image_info *info, uint64_t perms)
{
    cap_register_t cap;
    abi_ulong prog_base;
    ssize_t prog_len;

    cap = *cheri_zerocap();

    prog_base = info->start_addr;
    prog_len = info->end_addr - prog_base;

    /* Ensure program base and length are representable. */
    prog_base = CHERI_REPRESENTABLE_BASE(prog_base, prog_len);
    prog_len = CHERI_REPRESENTABLE_LENGTH(prog_len);
    assert(prog_len != 0);
    return (cheri_capability_build_user_rwx(&cap, perms, prog_base, prog_len,
        info->start_addr - prog_base));
}

/*
 * interp_cap() is ported from sys/kern/imgact_elf.c in CTSRD-CHERI/CheriBSD.
 */
static cap_register_t *
interp_cap(struct image_info *info, target_ulong base, uint64_t perms)
{
    cap_register_t cap;
    target_ulong interp_base;
    ssize_t interp_len;

    cap = *cheri_zerocap();

    interp_base = base;
    interp_len = info->interp_end - interp_base;

    /* Ensure rtld base and length are representable. */
    interp_base = CHERI_REPRESENTABLE_BASE(interp_base, interp_len);
    interp_len = CHERI_REPRESENTABLE_LENGTH(interp_len);
    assert(interp_len != 0);

    return (cheri_capability_build_user_rwx(&cap, perms, interp_base,
        interp_len, base - interp_base));
}
#endif

static abi_ulong target_create_elf_tables(abi_ulong p, int argc, int envc,
                                   abi_ulong stringp,
                                   struct elfhdr * exec,
                                   abi_ulong load_addr,
                                   abi_ulong load_bias,
                                   abi_ulong interp_load_addr, int ibcs,
                                   struct image_info *info)
{
        abi_ulong features, sp;
        int size;
        int typesize, valsize;
#ifdef TARGET_CHERI
        cap_register_t cap;
        target_ulong dstauxents;
#endif

#ifdef TARGET_CHERI
        assert(ibcs);
        /*
         * Elf64C_Auxinfo.a_type with padding because Elf64C_Auxinfo.a_un is
         * aligned to an 128-bit address as it can store a capability.
         */
        typesize = sizeof(int64_t) * 2;
#else
        typesize = sizeof(int64_t);
#endif
        valsize = ABI_PTR_SIZE;

	target_auxents_sz = 0;
        sp = p;
        /*
         * Force 16 byte _final_ alignment here for generality.
         */
        sp = sp &~ (abi_ulong)15;
        size = (DLINFO_ITEMS + 1) * (typesize + valsize);
        size += (envc + argc + 2) * ABI_PTR_SIZE;
        size += (!ibcs ? 3 : 1) * ABI_PTR_SIZE;        /* argc itself */
        if (size & 15)
            sp -= 16 - (size & 15);

#ifdef TARGET_CHERI
#define NEW_AUX_ENT(type, val) do {                                          \
            sp -= valsize; put_user_s64(val, sp); put_user_u64(0, sp + 8);   \
            sp -= typesize; put_user_s64(type, sp); put_user_u64(0, sp + 8); \
            target_auxents_sz += typesize + valsize;                         \
          } while(0)
#define NEW_AUX_ENT_PTR(type, ptr) do {                                      \
            sp -= valsize;                                                   \
            put_user_c(ptr, sp);                                             \
            sp -= typesize;                                                  \
            put_user_s64(type, sp);                                          \
            put_user_u64(0, sp + 8);                                         \
            target_auxents_sz += typesize + valsize;                         \
          } while(0)
#else
#define NEW_AUX_ENT(type, val) do {                  \
            sp -= valsize; put_user_s64(val, sp);    \
            sp -= typesize; put_user_s64(type, sp);  \
            target_auxents_sz += typesize + valsize; \
          } while(0)
#endif

        NEW_AUX_ENT (AT_NULL, 0);

        /* There must be exactly DLINFO_ITEMS entries here.  */

#ifdef TARGET_CHERI
        NEW_AUX_ENT_PTR(AT_PHDR, cheri_setaddress(prog_cap(info,
            CHERI_CAP_USER_DATA_PERMS), load_addr + exec->e_phoff));
#else
        NEW_AUX_ENT(AT_PHDR, (abi_ulong)(load_addr + exec->e_phoff));
#endif
        NEW_AUX_ENT(AT_PHENT, (abi_ulong)(sizeof (struct elf_phdr)));
        NEW_AUX_ENT(AT_PHNUM, (abi_ulong)(exec->e_phnum));
        NEW_AUX_ENT(AT_PAGESZ, (abi_ulong)(TARGET_PAGE_SIZE));
#ifdef TARGET_CHERI
        if (info->interp_end == 0) {
            if (exec->e_type != ET_DYN) {
                cap = *cheri_zerocap();
            } else {
                cap = *prog_cap(info, CHERI_CAP_USER_DATA_PERMS |
                    CHERI_CAP_USER_CODE_PERMS);
            }
        } else {
            cap = *interp_cap(info, interp_load_addr,
                CHERI_CAP_USER_DATA_PERMS | CHERI_CAP_USER_CODE_PERMS);
        }
        NEW_AUX_ENT_PTR(AT_BASE, cheri_setaddress(&cap, interp_load_addr));
#else
        NEW_AUX_ENT(AT_BASE, (abi_ulong)(interp_load_addr));
#endif
        NEW_AUX_ENT(AT_FLAGS, (abi_ulong)0);
        NEW_AUX_ENT(FREEBSD_AT_NCPUS, (abi_ulong)bsd_get_ncpu());
#ifdef TARGET_CHERI
        NEW_AUX_ENT_PTR(AT_ENTRY, cheri_setflags(cheri_setaddress(
            prog_cap(info, CHERI_CAP_USER_CODE_PERMS),
            load_bias + exec->e_entry), CHERI_FLAGS_CAP_MODE));
#else
        NEW_AUX_ENT(AT_ENTRY, load_bias + exec->e_entry);
#endif
        features = ELF_HWCAP;
#if defined(TARGET_ARM) && !defined(TARGET_AARCH64)
        {
            ARMCPU *cpu = ARM_CPU(thread_cpu);
            if (arm_feature(&cpu->env, ARM_FEATURE_VFP3))
                features |= ARM_HWCAP_ARM_VFPv3;
            if (arm_feature(&cpu->env, ARM_FEATURE_VFP4))
                features |= ARM_HWCAP_ARM_VFPv4;
        }
#endif
        NEW_AUX_ENT(FREEBSD_AT_HWCAP, features);
#ifdef TARGET_CHERI
        dstauxents = sp - 4 * (typesize + valsize);
        NEW_AUX_ENT(AT_ARGC, (target_ulong)argc);
        NEW_AUX_ENT_PTR(AT_ARGV, cheri_ptr((void *)(dstauxents -
            (envc + 1) * ABI_PTR_SIZE - (argc + 1) * ABI_PTR_SIZE),
            (argc + 1) * ABI_PTR_SIZE));
        NEW_AUX_ENT(AT_ENVC, (target_ulong)envc);
        NEW_AUX_ENT_PTR(AT_ENVV, cheri_ptr((void *)(dstauxents -
            (envc + 1) * ABI_PTR_SIZE), (envc + 1) * ABI_PTR_SIZE));
#else
#ifndef TARGET_PPC
        NEW_AUX_ENT(AT_UID, (abi_ulong) getuid());
        NEW_AUX_ENT(AT_EUID, (abi_ulong) geteuid());
        NEW_AUX_ENT(AT_GID, (abi_ulong) getgid());
        NEW_AUX_ENT(AT_EGID, (abi_ulong) getegid());
#endif
#endif /* TARGET_CHERI */
	target_auxents = sp; /* Note where the aux entries are in the target */
#ifdef ARCH_DLINFO
        /*
         * ARCH_DLINFO must come last so platform specific code can enforce
         * special alignment requirements on the AUXV if necessary (eg. PPC).
         */
        ARCH_DLINFO;
#endif
#ifdef TARGET_CHERI
#undef NEW_AUX_ENT_PTR
#endif
#undef NEW_AUX_ENT

        sp = loader_build_argptr(envc, argc, sp, stringp, !ibcs);
        return sp;
}

#endif /* _TARGET_OS_ELF_H_ */
