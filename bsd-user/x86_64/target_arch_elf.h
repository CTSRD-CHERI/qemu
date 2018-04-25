/*
 *  x86_64 ELF definitions
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
#ifndef _TARGET_ARCH_ELF_H_
#define _TARGET_ARCH_ELF_H_

#define ELF_START_MMAP 0x2aaaaab000ULL
#define elf_check_arch(x) ( ((x) == ELF_ARCH) )

#define ELF_CLASS      ELFCLASS64
#define ELF_DATA       ELFDATA2LSB
#define ELF_ARCH       EM_X86_64

#define USE_ELF_CORE_DUMP
#define ELF_EXEC_PAGESIZE       4096

/* XXX */
#ifndef __FreeBSD__
#define ELF_PLATFORM target_elf_get_platform()

static const char *target_elf_get_platform(void)
{
    static char elf_platform[] = "i386";
    int family = (thread_env->cpuid_version >> 8) & 0xff;
    if (family > 6)
        family = 6;
    if (family >= 3)
        elf_platform[1] = '0' + family;
    return elf_platform;
}

#define ELF_HWCAP target_elf_get_hwcap()

static uint32_t target_elf_get_hwcap(void)
{
    return thread_env->features[FEAT_1_EDX];
}
#endif /* ! __FreeBSD__ */

#endif /* _TARGET_ARCH_ELF_H_ */
