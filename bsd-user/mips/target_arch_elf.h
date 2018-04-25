/*
 *  mips ELF definitions
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

#define elf_check_arch(x) ( (x) == EM_MIPS )
#define ELF_START_MMAP 0x80000000
#define ELF_CLASS   ELFCLASS32

#ifdef TARGET_WORDS_BIGENDIAN
#define ELF_DATA        ELFDATA2MSB
#else
#define ELF_DATA        ELFDATA2LSB
#endif
#define ELF_ARCH    EM_MIPS

#define USE_ELF_CORE_DUMP
#define ELF_EXEC_PAGESIZE        4096

#endif /* _TARGET_ARCH_ELF_H_ */
