/*
 *  ELF loading code
 *
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

#include "qemu/osdep.h"

#include "qemu.h"
#include "qemu/error-report.h"
#include "disas/disas.h"
#include "target_os_elf.h"
#include "target_os_stack.h"
#include "target_os_thread.h"

abi_ulong target_stksiz;
abi_ulong target_stkbas;

static inline void memcpy_fromfs(void * to, const void * from, unsigned long n)
{
        memcpy(to, from, n);
}

static int load_aout_interp(void * exptr, int interp_fd);

#ifdef BSWAP_NEEDED
static void bswap_ehdr(struct elfhdr *ehdr)
{
    bswap16s(&ehdr->e_type);            /* Object file type */
    bswap16s(&ehdr->e_machine);         /* Architecture */
    bswap32s(&ehdr->e_version);         /* Object file version */
    bswaptls(&ehdr->e_entry);           /* Entry point virtual address */
    bswaptls(&ehdr->e_phoff);           /* Program header table file offset */
    bswaptls(&ehdr->e_shoff);           /* Section header table file offset */
    bswap32s(&ehdr->e_flags);           /* Processor-specific flags */
    bswap16s(&ehdr->e_ehsize);          /* ELF header size in bytes */
    bswap16s(&ehdr->e_phentsize);       /* Program header table entry size */
    bswap16s(&ehdr->e_phnum);           /* Program header table entry count */
    bswap16s(&ehdr->e_shentsize);       /* Section header table entry size */
    bswap16s(&ehdr->e_shnum);           /* Section header table entry count */
    bswap16s(&ehdr->e_shstrndx);        /* Section header string table index */
}

static void bswap_phdr(struct elf_phdr *phdr, int phnum)
{
    int i;

    for (i = 0; i < phnum; i++, phdr++) {
        bswap32s(&phdr->p_type);        /* Segment type */
        bswaptls(&phdr->p_offset);      /* Segment file offset */
        bswaptls(&phdr->p_vaddr);       /* Segment virtual address */
        bswaptls(&phdr->p_paddr);       /* Segment physical address */
        bswaptls(&phdr->p_filesz);      /* Segment size in file */
        bswaptls(&phdr->p_memsz);       /* Segment size in memory */
        bswap32s(&phdr->p_flags);       /* Segment flags */
        bswaptls(&phdr->p_align);       /* Segment alignment */
    }
}

static void bswap_shdr(struct elf_shdr *shdr, int shnum)
{
    int i;

    for (i = 0; i < shnum; i++, shdr++) {
        bswap32s(&shdr->sh_name);
        bswap32s(&shdr->sh_type);
        bswaptls(&shdr->sh_flags);
        bswaptls(&shdr->sh_addr);
        bswaptls(&shdr->sh_offset);
        bswaptls(&shdr->sh_size);
        bswap32s(&shdr->sh_link);
        bswap32s(&shdr->sh_info);
        bswaptls(&shdr->sh_addralign);
        bswaptls(&shdr->sh_entsize);
    }
}

static void bswap_sym(struct elf_sym *sym)
{
    bswap32s(&sym->st_name);
    bswaptls(&sym->st_value);
    bswaptls(&sym->st_size);
    bswap16s(&sym->st_shndx);
}

#else /* ! BSWAP_NEEDED */

static void bswap_ehdr(struct elfhdr *ehdr) { }
static void bswap_phdr(struct elf_phdr *phdr, int phnum) { }
static void bswap_shdr(struct elf_shdr *shdr, int shnum) { }
static void bswap_sym(struct elf_sym *sym) { }

#endif /* ! BSWAP_NEEDED */

/*
 * 'copy_elf_strings()' copies argument/envelope strings from user
 * memory to free pages in kernel mem. These are in a format ready
 * to be put directly into the top of new user memory.
 *
 */
static abi_ulong copy_elf_strings(int argc,char ** argv, void **page,
                                  abi_ulong p)
{
    char *tmp, *tmp1, *pag = NULL;
    int len, offset = 0;

    if (!p) {
        return 0;       /* bullet-proofing */
    }
    while (argc-- > 0) {
        tmp = argv[argc];
        if (!tmp) {
            fprintf(stderr, "VFS: argc is wrong");
            exit(-1);
        }
        tmp1 = tmp;
        while (*tmp++);
        len = tmp - tmp1;
        if (p < len) {  /* this shouldn't happen - 128kB */
                return 0;
        }
        while (len) {
            --p; --tmp; --len;
            if (--offset < 0) {
                offset = p % TARGET_PAGE_SIZE;
                pag = (char *)page[p/TARGET_PAGE_SIZE];
                if (!pag) {
                    pag = g_try_malloc0(TARGET_PAGE_SIZE);
                    page[p/TARGET_PAGE_SIZE] = pag;
                    if (!pag)
                        return 0;
                }
            }
            if (len == 0 || offset == 0) {
                *(pag + offset) = *tmp;
            }
            else {
              int bytes_to_copy = (len > offset) ? offset : len;
              tmp -= bytes_to_copy;
              p -= bytes_to_copy;
              offset -= bytes_to_copy;
              len -= bytes_to_copy;
              memcpy_fromfs(pag + offset, tmp, bytes_to_copy + 1);
            }
        }
    }
    return p;
}

static void setup_arg_pages(struct bsd_binprm *bprm, struct image_info *info,
                            abi_ulong *stackp, abi_ulong *stringp)
{
    abi_ulong stack_base, size;
    abi_long addr;

    /* Create enough stack to hold everything.  If we don't use
     * it for args, we'll use it for something else...
     */
    size = target_dflssiz;
    stack_base = TARGET_USRSTACK - size;
    addr = target_mmap(stack_base , size + qemu_host_page_size,
            PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (addr == -1) {
        perror("stk mmap");
        exit(-1);
    }
    /* we reserve one extra page at the top of the stack as guard */
    target_mprotect(addr + size, qemu_host_page_size, PROT_NONE);

    target_stksiz = size;
    target_stkbas = addr;

    if (setup_initial_stack(bprm, stackp, stringp) != 0) {
        perror("stk setup");
        exit(-1);
    }
}

static void set_brk(abi_ulong start, abi_ulong end)
{
        /* page-align the start and end addresses... */
        start = HOST_PAGE_ALIGN(start);
        end = HOST_PAGE_ALIGN(end);
        if (end <= start)
                return;
        if(target_mmap(start, end - start,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0) == -1) {
            perror("cannot mmap brk");
            exit(-1);
        }
}


/* We need to explicitly zero any fractional pages after the data
   section (i.e. bss).  This would contain the junk from the file that
   should not be in memory. */
static void padzero(abi_ulong elf_bss, abi_ulong last_bss)
{
        abi_ulong nbyte;

        if (elf_bss >= last_bss)
                return;

        /* XXX: this is really a hack : if the real host page size is
           smaller than the target page size, some pages after the end
           of the file may not be mapped. A better fix would be to
           patch target_mmap(), but it is more complicated as the file
           size must be known */
        if (qemu_real_host_page_size < qemu_host_page_size) {
            abi_ulong end_addr, end_addr1;
            end_addr1 = REAL_HOST_PAGE_ALIGN(elf_bss);
            end_addr = HOST_PAGE_ALIGN(elf_bss);
            if (end_addr1 < end_addr) {
                mmap((void *)g2h(end_addr1), end_addr - end_addr1,
                     PROT_READ|PROT_WRITE|PROT_EXEC,
                     MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
            }
        }

        nbyte = elf_bss & (qemu_host_page_size-1);
        if (nbyte) {
            nbyte = qemu_host_page_size - nbyte;
            do {
                /* FIXME - what to do if put_user() fails? */
                put_user_u8(0, elf_bss);
                elf_bss++;
            } while (--nbyte);
        }
}

static abi_ulong load_elf_interp(struct elfhdr * interp_elf_ex,
                                 int interpreter_fd,
                                 abi_ulong *interp_load_addr)
{
        struct elf_phdr *elf_phdata  =  NULL;
        struct elf_phdr *eppnt;
        abi_ulong load_addr = 0;
        int load_addr_set = 0;
        int retval;
        abi_ulong last_bss, elf_bss;
        abi_ulong error;
        int i;

        elf_bss = 0;
        last_bss = 0;
        error = 0;

        bswap_ehdr(interp_elf_ex);
        /* First of all, some simple consistency checks */
        if ((interp_elf_ex->e_type != ET_EXEC &&
             interp_elf_ex->e_type != ET_DYN) ||
           !elf_check_arch(interp_elf_ex->e_machine)) {
                return ~((abi_ulong)0UL);
        }


        /* Now read in all of the header information */

        if (sizeof(struct elf_phdr) * interp_elf_ex->e_phnum > TARGET_PAGE_SIZE)
            return ~(abi_ulong)0UL;

        elf_phdata =  (struct elf_phdr *)
                malloc(sizeof(struct elf_phdr) * interp_elf_ex->e_phnum);

        if (!elf_phdata)
          return ~((abi_ulong)0UL);

        /*
         * If the size of this structure has changed, then punt, since
         * we will be doing the wrong thing.
         */
        if (interp_elf_ex->e_phentsize != sizeof(struct elf_phdr)) {
            free(elf_phdata);
            return ~((abi_ulong)0UL);
        }

        retval = lseek(interpreter_fd, interp_elf_ex->e_phoff, SEEK_SET);
        if(retval >= 0) {
            retval = read(interpreter_fd,
                           (char *) elf_phdata,
                           sizeof(struct elf_phdr) * interp_elf_ex->e_phnum);
        }
        if (retval < 0) {
                perror("load_elf_interp");
                exit(-1);
                free (elf_phdata);
                return retval;
        }
        bswap_phdr(elf_phdata, interp_elf_ex->e_phnum);

        if (interp_elf_ex->e_type == ET_DYN) {
            /* in order to avoid hardcoding the interpreter load
               address in qemu, we allocate a big enough memory zone */
            error = target_mmap(0, INTERP_MAP_SIZE,
                                PROT_NONE, MAP_PRIVATE | MAP_ANON,
                                -1, 0);
            if (error == -1) {
                perror("mmap");
                exit(-1);
            }
            load_addr = error;
            load_addr_set = 1;
        }

        eppnt = elf_phdata;
        for (i = 0; i < interp_elf_ex->e_phnum; i++, eppnt++)
            if (eppnt->p_type == PT_LOAD) {
                int elf_type = MAP_PRIVATE | MAP_DENYWRITE;
                int elf_prot = 0;
                abi_ulong vaddr = 0;
                abi_ulong k;

                if (eppnt->p_flags & PF_R) {
                    elf_prot =  PROT_READ;
                }
                if (eppnt->p_flags & PF_W) {
                    elf_prot |= PROT_WRITE;
                }
                if (eppnt->p_flags & PF_X) {
                    elf_prot |= PROT_EXEC;
                }
                if (interp_elf_ex->e_type == ET_EXEC || load_addr_set) {
                    elf_type |= MAP_FIXED;
                    vaddr = eppnt->p_vaddr;
                }
                error = target_mmap(load_addr+TARGET_ELF_PAGESTART(vaddr),
                        eppnt->p_filesz + TARGET_ELF_PAGEOFFSET(eppnt->p_vaddr),
                        elf_prot, elf_type, interpreter_fd, eppnt->p_offset -
                        TARGET_ELF_PAGEOFFSET(eppnt->p_vaddr));
                if (error == -1) {
                    /* Real error */
                    close(interpreter_fd);
                    free(elf_phdata);
                    return ~((abi_ulong)0UL);
                }
                if (!load_addr_set && interp_elf_ex->e_type == ET_DYN) {
                    load_addr = error;
                    load_addr_set = 1;
                }
                /*
                 * Find the end of the file  mapping for this phdr, and keep
                 * track of the largest address we see for this.
                 */
                k = load_addr + eppnt->p_vaddr + eppnt->p_filesz;
                if (k > elf_bss) {
                    elf_bss = k;
                }
                /*
                 * Do the same thing for the memory mapping - between
                 * elf_bss and last_bss is the bss section.
                 */
                k = load_addr + eppnt->p_memsz + eppnt->p_vaddr;
                if (k > last_bss) {
                    last_bss = k;
                }
            }
        /* Now use mmap to map the library into memory. */

        close(interpreter_fd);

        /*
         * Now fill out the bss section.  First pad the last page up
         * to the page boundary, and then perform a mmap to make sure
         * that there are zeromapped pages up to and including the last
         * bss page.
         */
        padzero(elf_bss, last_bss);
        /* What we have mapped so far */
        elf_bss = TARGET_ELF_PAGESTART(elf_bss + qemu_host_page_size - 1);

        /* Map the last of the bss segment */
        if (last_bss > elf_bss) {
            target_mmap(elf_bss, last_bss-elf_bss,
                        PROT_READ|PROT_WRITE|PROT_EXEC,
                        MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);
        }
        free(elf_phdata);

        *interp_load_addr = load_addr;
        return ((abi_ulong) interp_elf_ex->e_entry) + load_addr;
}

static int symfind(const void *s0, const void *s1)
{
    target_ulong addr = *(target_ulong *)s0;
    struct elf_sym *sym = (struct elf_sym *)s1;
    int result = 0;
    if (addr < sym->st_value) {
        result = -1;
    } else if (addr >= sym->st_value + sym->st_size) {
        result = 1;
    }
    return result;
}

static const char *lookup_symbolxx(struct syminfo *s, target_ulong orig_addr)
{
#if ELF_CLASS == ELFCLASS32
    struct elf_sym *syms = s->disas_symtab.elf32;
#else
    struct elf_sym *syms = s->disas_symtab.elf64;
#endif

    // binary search
    struct elf_sym *sym;

    sym = bsearch(&orig_addr, syms, s->disas_num_syms, sizeof(*syms), symfind);
    if (sym != NULL) {
        return s->disas_strtab + sym->st_name;
    }

    return "";
}

/* FIXME: This should use elf_ops.h  */
static int symcmp(const void *s0, const void *s1)
{
    struct elf_sym *sym0 = (struct elf_sym *)s0;
    struct elf_sym *sym1 = (struct elf_sym *)s1;
    return (sym0->st_value < sym1->st_value)
        ? -1
        : ((sym0->st_value > sym1->st_value) ? 1 : 0);
}

/* Best attempt to load symbols from this ELF object. */
static void load_symbols(struct elfhdr *hdr, int fd)
{
    unsigned int i, nsyms;
    struct elf_shdr sechdr, symtab, strtab;
    char *strings;
    struct syminfo *s;
    struct elf_sym *syms, *new_syms;

    lseek(fd, hdr->e_shoff, SEEK_SET);
    for (i = 0; i < hdr->e_shnum; i++) {
        if (read(fd, &sechdr, sizeof(sechdr)) != sizeof(sechdr))
            return;
        bswap_shdr(&sechdr, 1);
        if (sechdr.sh_type == SHT_SYMTAB) {
            symtab = sechdr;
            lseek(fd, hdr->e_shoff
                  + sizeof(sechdr) * sechdr.sh_link, SEEK_SET);
            if (read(fd, &strtab, sizeof(strtab))
                != sizeof(strtab))
                return;
            bswap_shdr(&strtab, 1);
            goto found;
        }
    }
    return; /* Shouldn't happen... */

 found:
    /* Now know where the strtab and symtab are.  Snarf them. */
    s = malloc(sizeof(*s));
    syms = malloc(symtab.sh_size);
    if (!syms) {
        free(s);
        return;
    }
    s->disas_strtab = strings = malloc(strtab.sh_size);
    if (!s->disas_strtab) {
        free(s);
        free(syms);
        return;
    }

    lseek(fd, symtab.sh_offset, SEEK_SET);
    if (read(fd, syms, symtab.sh_size) != symtab.sh_size) {
        free(s);
        free(syms);
        free(strings);
        return;
    }

    nsyms = symtab.sh_size / sizeof(struct elf_sym);

    i = 0;
    while (i < nsyms) {
        bswap_sym(syms + i);
        // Throw away entries which we do not need.
        if (syms[i].st_shndx == SHN_UNDEF ||
                syms[i].st_shndx >= SHN_LORESERVE ||
                ELF_ST_TYPE(syms[i].st_info) != STT_FUNC) {
            nsyms--;
            if (i < nsyms) {
                syms[i] = syms[nsyms];
            }
            continue;
        }
#if defined(TARGET_ARM) || defined (TARGET_MIPS)
        /* The bottom address bit marks a Thumb or MIPS16 symbol.  */
        syms[i].st_value &= ~(target_ulong)1;
#endif
        i++;
    }

     /* Attempt to free the storage associated with the local symbols
        that we threw away.  Whether or not this has any effect on the
        memory allocation depends on the malloc implementation and how
        many symbols we managed to discard. */
    new_syms = realloc(syms, nsyms * sizeof(*syms));
    if (new_syms == NULL) {
        free(s);
        free(syms);
        free(strings);
        return;
    }
    syms = new_syms;

    qsort(syms, nsyms, sizeof(*syms), symcmp);

    lseek(fd, strtab.sh_offset, SEEK_SET);
    if (read(fd, strings, strtab.sh_size) != strtab.sh_size) {
        free(s);
        free(syms);
        free(strings);
        return;
    }
    s->disas_num_syms = nsyms;
#if ELF_CLASS == ELFCLASS32
    s->disas_symtab.elf32 = syms;
    s->lookup_symbol = (lookup_symbol_t)lookup_symbolxx;
#else
    s->disas_symtab.elf64 = syms;
    s->lookup_symbol = (lookup_symbol_t)lookup_symbolxx;
#endif
    s->next = syminfos;
    syminfos = s;
}

/* Check the elf header and see if this a target elf binary. */
int is_target_elf_binary(int fd)
{
    uint8_t buf[128];
    struct elfhdr elf_ex;

    if (lseek(fd, 0L, SEEK_SET) < 0) {
        return 0;
    }
    if (read(fd, buf, sizeof(buf)) < 0) {
        return 0;
    }

    elf_ex = *((struct elfhdr *)buf);
    bswap_ehdr(&elf_ex);

    if ((elf_ex.e_type != ET_EXEC && elf_ex.e_type != ET_DYN) ||
        (!elf_check_arch(elf_ex.e_machine))) {
        return 0;
    } else {
        return 1;
    }
}

int load_elf_binary(struct bsd_binprm *bprm, struct target_pt_regs *regs,
                    struct image_info *info)
{
    struct elfhdr elf_ex;
    struct elfhdr interp_elf_ex;
    struct exec interp_ex;
    int interpreter_fd = -1; /* avoid warning */
    abi_ulong load_addr, load_bias;
    int load_addr_set = 0;
    unsigned int interpreter_type = INTERPRETER_NONE;
    unsigned char ibcs2_interpreter;
    int i;
    struct elf_phdr * elf_ppnt;
    struct elf_phdr *elf_phdata;
    abi_ulong elf_bss, k, elf_brk;
    int retval;
    char * elf_interpreter;
    abi_ulong elf_entry, interp_load_addr = 0;
    abi_ulong start_code, end_code, start_data, end_data;
    abi_ulong reloc_func_desc = 0;
#ifdef LOW_ELF_STACK
    abi_ulong elf_stack;
#endif
    char passed_fileno[6];

    ibcs2_interpreter = 0;
    load_addr = 0;
    load_bias = 0;
    elf_ex = *((struct elfhdr *) bprm->buf);          /* exec-header */
    bswap_ehdr(&elf_ex);

    /* First of all, some simple consistency checks */
    if ((elf_ex.e_type != ET_EXEC && elf_ex.e_type != ET_DYN) ||
                                (! elf_check_arch(elf_ex.e_machine))) {
            return -ENOEXEC;
    }

#ifndef __FreeBSD__
    bprm->p = copy_elf_strings(1, &bprm->filename, bprm->page, bprm->p);
    bprm->p = copy_elf_strings(bprm->envc,bprm->envp,bprm->page,bprm->p);
    bprm->p = copy_elf_strings(bprm->argc,bprm->argv,bprm->page,bprm->p);
    if (!bprm->p) {
        retval = -E2BIG;
    }
#endif /* ! __FreeBSD__ */

    /* Now read in all of the header information */
    elf_phdata = (struct elf_phdr *)malloc(elf_ex.e_phentsize*elf_ex.e_phnum);
    if (elf_phdata == NULL) {
        return -ENOMEM;
    }

    retval = lseek(bprm->fd, elf_ex.e_phoff, SEEK_SET);
    if(retval > 0) {
        retval = read(bprm->fd, (char *) elf_phdata,
                                elf_ex.e_phentsize * elf_ex.e_phnum);
    }

    if (retval < 0) {
        perror("load_elf_binary");
        exit(-1);
        free (elf_phdata);
        return -errno;
    }

    bswap_phdr(elf_phdata, elf_ex.e_phnum);
    elf_ppnt = elf_phdata;

    elf_bss = 0;
    elf_brk = 0;


#ifdef LOW_ELF_STACK
    elf_stack = ~((abi_ulong)0UL);
#endif
    elf_interpreter = NULL;
    start_code = ~((abi_ulong)0UL);
    end_code = 0;
    start_data = 0;
    end_data = 0;
    interp_ex.a_info = 0;

    for (i = 0; i < elf_ex.e_phnum; i++) {
        if (elf_ppnt->p_type == PT_INTERP) {
            if ( elf_interpreter != NULL )
            {
                free (elf_phdata);
                free(elf_interpreter);
                close(bprm->fd);
                return -EINVAL;
            }

            /* This is the program interpreter used for
             * shared libraries - for now assume that this
             * is an a.out format binary
             */

            elf_interpreter = (char *)malloc(elf_ppnt->p_filesz);

            if (elf_interpreter == NULL) {
                free (elf_phdata);
                close(bprm->fd);
                return -ENOMEM;
            }

            retval = lseek(bprm->fd, elf_ppnt->p_offset, SEEK_SET);
            if(retval >= 0) {
                retval = read(bprm->fd, elf_interpreter, elf_ppnt->p_filesz);
            }
            if(retval < 0) {
                perror("load_elf_binary2");
                exit(-1);
            }

            /* If the program interpreter is one of these two,
               then assume an iBCS2 image. Otherwise assume
               a native linux image. */

            /* JRP - Need to add X86 lib dir stuff here... */

            if (strcmp(elf_interpreter, "/usr/lib/libc.so.1") == 0 ||
                    strcmp(elf_interpreter, "/usr/lib/ld-elf.so.1") == 0) {
                ibcs2_interpreter = 1;
            }

#if 0
            printf("Using ELF interpreter %s\n", path(elf_interpreter));
#endif
            if (retval >= 0) {
                retval = open(path(elf_interpreter), O_RDONLY);
                if(retval >= 0) {
                    interpreter_fd = retval;
                }
                else {
                    perror(elf_interpreter);
                    exit(-1);
                    /* retval = -errno; */
                }
            }

            if (retval >= 0) {
                retval = lseek(interpreter_fd, 0, SEEK_SET);
                if(retval >= 0) {
                    retval = read(interpreter_fd,bprm->buf,128);
                }
            }
            if (retval >= 0) {
                interp_ex = *((struct exec *) bprm->buf); /* aout exec-header */
                interp_elf_ex = *((struct elfhdr *) bprm->buf); /* elf exec-header */
            }
            if (retval < 0) {
                perror("load_elf_binary3");
                exit(-1);
                free (elf_phdata);
                free(elf_interpreter);
                close(bprm->fd);
                return retval;
            }
        }
        elf_ppnt++;
    }

    /* Some simple consistency checks for the interpreter */
    if (elf_interpreter){
        interpreter_type = INTERPRETER_ELF | INTERPRETER_AOUT;

        /* Now figure out which format our binary is */
        if ((N_MAGIC(interp_ex) != OMAGIC) && (N_MAGIC(interp_ex) != ZMAGIC) &&
                (N_MAGIC(interp_ex) != QMAGIC)) {
          interpreter_type = INTERPRETER_ELF;
        }

        if (interp_elf_ex.e_ident[0] != 0x7f ||
                strncmp((char *)&interp_elf_ex.e_ident[1], "ELF",3) != 0) {
            interpreter_type &= ~INTERPRETER_ELF;
        }

        if (!interpreter_type) {
            free(elf_interpreter);
            free(elf_phdata);
            close(bprm->fd);
            return -ELIBBAD;
        }
    }

    /* OK, we are done with that, now set up the arg stuff,
       and then start this sucker up */

    {
        char * passed_p;

        if (interpreter_type == INTERPRETER_AOUT) {
            snprintf(passed_fileno, sizeof(passed_fileno), "%d", bprm->fd);
            passed_p = passed_fileno;

            if (elf_interpreter) {
                bprm->p = copy_elf_strings(1,&passed_p,bprm->page,bprm->p);
                bprm->argc++;
            }
        }
        if (!bprm->p) {
            free(elf_interpreter);
            free (elf_phdata);
            close(bprm->fd);
            return -E2BIG;
        }
    }

    /* OK, This is the point of no return */
    info->end_data = 0;
    info->end_code = 0;
    info->start_mmap = (abi_ulong)ELF_START_MMAP;
    info->mmap = 0;
    elf_entry = (abi_ulong) elf_ex.e_entry;

    /*
     * In case where user has not explicitly set the guest_base, we
     * probe here that should we set it automatically.
     */
    if (!have_guest_base) {
        /*
         * Go through ELF program header table and find out whether
         * any of the segments drop below our current mmap_min_addr and
         * in that case set guest_base to corresponding address.
         */
        for (i = 0, elf_ppnt = elf_phdata; i < elf_ex.e_phnum;
            i++, elf_ppnt++) {
            if (elf_ppnt->p_type != PT_LOAD)
                continue;
            if (HOST_PAGE_ALIGN(elf_ppnt->p_vaddr) < mmap_min_addr) {
                guest_base = HOST_PAGE_ALIGN(mmap_min_addr);
                break;
            }
        }
    }

    /* Do this so that we can load the interpreter, if need be.  We will
       change some of these later */
    info->rss = 0;
    setup_arg_pages(bprm, info, &bprm->p, &bprm->stringp);
    info->start_stack = bprm->p;

    /* Now we do a little grungy work by mmaping the ELF image into
     * the correct location in memory.  At this point, we assume that
     * the image should be loaded at fixed address, not at a variable
     * address.
     */

    for (i = 0, elf_ppnt = elf_phdata; i < elf_ex.e_phnum; i++, elf_ppnt++) {
        int elf_prot = 0;
        int elf_flags = 0;
        abi_ulong error;

        if (elf_ppnt->p_type != PT_LOAD)
            continue;

        if (elf_ppnt->p_flags & PF_R) elf_prot |= PROT_READ;
        if (elf_ppnt->p_flags & PF_W) elf_prot |= PROT_WRITE;
        if (elf_ppnt->p_flags & PF_X) elf_prot |= PROT_EXEC;
        elf_flags = MAP_PRIVATE | MAP_DENYWRITE;
        if (elf_ex.e_type == ET_EXEC || load_addr_set) {
            elf_flags |= MAP_FIXED;
        } else if (elf_ex.e_type == ET_DYN) {
            /* Try and get dynamic programs out of the way of the default mmap
               base, as well as whatever program they might try to exec.  This
               is because the brk will follow the loader, and is not movable. */
            /* NOTE: for qemu, we do a big mmap to get enough space
               without hardcoding any address */
            error = target_mmap(0, ET_DYN_MAP_SIZE,
                                PROT_NONE, MAP_PRIVATE | MAP_ANON,
                                -1, 0);
            if (error == -1) {
                perror("mmap");
                exit(-1);
            }
            load_bias = TARGET_ELF_PAGESTART(error - elf_ppnt->p_vaddr);
        }

        error = target_mmap(TARGET_ELF_PAGESTART(load_bias + elf_ppnt->p_vaddr),
                            (elf_ppnt->p_filesz +
                             TARGET_ELF_PAGEOFFSET(elf_ppnt->p_vaddr)),
                            elf_prot,
                            (MAP_FIXED | MAP_PRIVATE | MAP_DENYWRITE),
                            bprm->fd,
                            (elf_ppnt->p_offset -
                             TARGET_ELF_PAGEOFFSET(elf_ppnt->p_vaddr)));
        if (error == -1) {
            perror("mmap");
            exit(-1);
        }

#ifdef LOW_ELF_STACK
        if (TARGET_ELF_PAGESTART(elf_ppnt->p_vaddr) < elf_stack)
            elf_stack = TARGET_ELF_PAGESTART(elf_ppnt->p_vaddr);
#endif

        if (!load_addr_set) {
            load_addr_set = 1;
            load_addr = elf_ppnt->p_vaddr - elf_ppnt->p_offset;
            if (elf_ex.e_type == ET_DYN) {
                load_bias += error -
                    TARGET_ELF_PAGESTART(load_bias + elf_ppnt->p_vaddr);
                load_addr += load_bias;
                reloc_func_desc = load_bias;
            }
        }
        k = elf_ppnt->p_vaddr;
        if (k < start_code)
            start_code = k;
        if (start_data < k)
            start_data = k;
        k = elf_ppnt->p_vaddr + elf_ppnt->p_filesz;
        if (k > elf_bss)
            elf_bss = k;
        if ((elf_ppnt->p_flags & PF_X) && end_code <  k)
            end_code = k;
        if (end_data < k)
            end_data = k;
        k = elf_ppnt->p_vaddr + elf_ppnt->p_memsz;
        if (k > elf_brk) elf_brk = k;
    }

    elf_entry += load_bias;
    elf_bss += load_bias;
    elf_brk += load_bias;
    start_code += load_bias;
    end_code += load_bias;
    start_data += load_bias;
    end_data += load_bias;

    if (elf_interpreter) {
        if (interpreter_type & 1) {
            elf_entry = load_aout_interp(&interp_ex, interpreter_fd);
        }
        else if (interpreter_type & 2) {
            elf_entry = load_elf_interp(&interp_elf_ex, interpreter_fd,
                                            &interp_load_addr);
        }
        reloc_func_desc = interp_load_addr;

        close(interpreter_fd);
        free(elf_interpreter);

        if (elf_entry == ~((abi_ulong)0UL)) {
            printf("Unable to load interpreter\n");
            free(elf_phdata);
            exit(-1);
            return 0;
        }
    }

    free(elf_phdata);

    if (qemu_log_enabled())
        load_symbols(&elf_ex, bprm->fd);

    if (interpreter_type != INTERPRETER_AOUT) close(bprm->fd);
    info->personality = (ibcs2_interpreter ? PER_SVR4 : PER_LINUX);

#ifdef LOW_ELF_STACK
    info->start_stack = bprm->p = elf_stack - 4;
#endif
    bprm->p = target_create_elf_tables(bprm->p,
                    bprm->argc,
                    bprm->envc,
                    bprm->stringp,
                    &elf_ex,
                    load_addr, load_bias,
                    interp_load_addr,
                    (interpreter_type == INTERPRETER_AOUT ? 0 : 1),
                    info);
    info->load_addr = reloc_func_desc;
    info->start_brk = info->brk = elf_brk;
    info->end_code = end_code;
    info->start_code = start_code;
    info->start_data = start_data;
    info->end_data = end_data;
    info->start_stack = bprm->p;
    info->load_bias = load_bias;

    /*
     * Calling set_brk effectively mmaps the pages that we need for the bss
     * and break sections.
     */
    set_brk(elf_bss, elf_brk);

    padzero(elf_bss, elf_brk);

#if 0
    printf("(start_brk) 0x" TARGET_FMT_lx "\n" , info->start_brk);
    printf("(end_code) 0x" TARGET_FMT_lx "\n" , info->end_code);
    printf("(start_code) 0x" TARGET_FMT_lx "\n" , info->start_code);
    printf("(start_data) 0x" TARGET_FMT_lx "\n" , info->start_data);
    printf("(end_data) 0x" TARGET_FMT_lx "\n" , info->end_data);
    printf("(start_stack) 0x" TARGET_FMT_lx "\n" , info->start_stack);
    printf("(brk) 0x" TARGET_FMT_lx "\n" , info->brk);
#endif

    if ( info->personality == PER_SVR4 )
    {
            /* Why this, you ask???  Well SVr4 maps page 0 as read-only,
               and some applications "depend" upon this behavior.
               Since we do not have the power to recompile these, we
               emulate the SVr4 behavior.  Sigh.  */
            mapped_addr = target_mmap(0, qemu_host_page_size, PROT_READ |
                    PROT_EXEC, MAP_FIXED | MAP_PRIVATE, -1, 0);
	    if (mapped_addr == -1) {
		    return -1;
	    }
    }

    info->entry = elf_entry;

#ifdef USE_ELF_CORE_DUMP
    /* not yet */
    /* bprm->core_dump = &elf_core_dump; */
    bprm->core_dump = NULL;
#endif

    return 0;
}

static int load_aout_interp(void * exptr, int interp_fd)
{
    printf("a.out interpreter not yet supported\n");
    return(0);
}

void do_init_thread(struct target_pt_regs *regs, struct image_info *infop)
{
    target_thread_init(regs, infop);
}
