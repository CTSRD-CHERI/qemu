/*
 *  Load BSD executables.
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

#define TARGET_NGROUPS 32

/* ??? This should really be somewhere else.  */
abi_long memcpy_to_target(abi_ulong dest, const void *src,
                          unsigned long len)
{
    void *host_ptr;

    host_ptr = lock_user(VERIFY_WRITE, dest, len, 0);
    if (!host_ptr)
        return -TARGET_EFAULT;
    memcpy(host_ptr, src, len);
    unlock_user(host_ptr, dest, 1);
    return 0;
}

static int count(char ** vec)
{
    int         i;

    for (i = 0; *vec; i++) {
        vec++;
    }

    return(i);
}

static int prepare_binprm(struct bsd_binprm *bprm)
{
    struct stat         st;
    int mode;
    int retval;

    if(fstat(bprm->fd, &st) < 0) {
        return(-errno);
    }

    mode = st.st_mode;
    if(!S_ISREG(mode)) {        /* Must be regular file */
        return(-EACCES);
    }
    if(!(mode & 0111)) {        /* Must have at least one execute bit set */
        return(-EACCES);
    }

    bprm->e_uid = geteuid();
    bprm->e_gid = getegid();

    /* Set-uid? */
    if(mode & S_ISUID) {
        bprm->e_uid = st.st_uid;
    }

    /* Set-gid? */
    /*
     * If setgid is set but no group execute bit then this
     * is a candidate for mandatory locking, not a setgid
     * executable.
     */
    if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP)) {
        bprm->e_gid = st.st_gid;
    }

    memset(bprm->buf, 0, sizeof(bprm->buf));
    retval = lseek(bprm->fd, 0L, SEEK_SET);
    if(retval >= 0) {
        retval = read(bprm->fd, bprm->buf, 128);
    }
    if(retval < 0) {
        perror("prepare_binprm");
        exit(-1);
        /* return(-errno); */
    }
    else {
        return(retval);
    }
}

/* Construct the envp and argv tables on the target stack.  */
abi_ulong loader_build_argptr(int envc, int argc, abi_ulong sp,
                              abi_ulong stringp, int push_ptr)
{
    int n = sizeof(abi_ulong);
    abi_ulong envp;
    abi_ulong argv;

    sp -= (envc + 1) * n;
    envp = sp;
    sp -= (argc + 1) * n;
    argv = sp;
    if (push_ptr) {
        /* FIXME - handle put_user() failures */
        sp -= n;
        put_user_ual(envp, sp);
        sp -= n;
        put_user_ual(argv, sp);
    }
    sp -= n;
    /* FIXME - handle put_user() failures */
    put_user_ual(argc, sp);

    while (argc-- > 0) {
        /* FIXME - handle put_user() failures */
        put_user_ual(stringp, argv);
        argv += n;
        stringp += target_strlen(stringp) + 1;
    }
    /* FIXME - handle put_user() failures */
    put_user_ual(0, argv);
    while (envc-- > 0) {
        /* FIXME - handle put_user() failures */
        put_user_ual(stringp, envp);
        envp += n;
        stringp += target_strlen(stringp) + 1;
    }
    /* FIXME - handle put_user() failures */
    put_user_ual(0, envp);

    return sp;
}

static int is_there(const char *candidate)
{
    struct stat fin;

    /* XXX work around access(2) false positives for superuser */
    if (access(candidate, X_OK) == 0 && stat(candidate, &fin) == 0 &&
            S_ISREG(fin.st_mode) && (getuid() != 0 ||
                (fin.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)) {
        return 1;
    }

    return 0;
}

static int find_in_path(char *path, const char *filename, char *retpath,
        size_t rpsize)
{
    const char *d;
    int found;

    if (strchr(filename, '/') != NULL) {
        if (is_there(filename)) {
                if (!realpath(filename, retpath)) {
                    return -1;
                }
                return 0;
        } else {
            return -1;
        }
    }

    found = 0;
    while ((d = strsep(&path, ":")) != NULL) {
        if (*d == '\0') {
            d = ".";
        }
        if (snprintf(retpath, rpsize, "%s/%s", d, filename) >= (int)rpsize) {
            continue;
        }
        if (is_there((const char *)retpath)) {
            found = 1;
            break;
        }
    }
    return found;
}

int loader_exec(const char * filename, char ** argv, char ** envp,
             struct target_pt_regs *regs, struct image_info *infop,
             struct bsd_binprm *bprm)
{
    char *p, *path = NULL, fullpath[PATH_MAX];
    const char *execname = NULL;
    int retval, i, found;

    bprm->p = TARGET_PAGE_SIZE * MAX_ARG_PAGES; /* -sizeof(unsigned int); */
    for (i=0 ; i<MAX_ARG_PAGES ; i++)       /* clear page-table */
            bprm->page[i] = NULL;

    /* Find target executable in path, if not already an absolute path. */
    p = getenv("PATH");
    if (p != NULL) {
        path = g_strdup(p);
        if (path == NULL) {
            fprintf(stderr, "Out of memory\n");
            return -1;
        }
        execname = realpath(filename, NULL);
        if (execname == NULL) {
            execname = g_strdup(filename);
        }
        found = find_in_path(path, execname, fullpath, sizeof(fullpath));
        /* Absolute path specified but not found? */
        if (found == -1) {
            return -1;
        }
        if (found) {
            retval = open(fullpath, O_RDONLY);
            bprm->fullpath = g_strdup(fullpath);
        } else {
            retval = open(execname, O_RDONLY);
            bprm->fullpath = g_strdup(execname);
        }
        if (execname) {
            g_free((void *)execname);
        }
        g_free(path);
    } else {
        retval = open(filename, O_RDONLY);
        bprm->fullpath = NULL;
    }
    if (retval < 0) {
        return retval;
    }

    bprm->fd = retval;
    bprm->filename = (char *)filename;
    bprm->argc = count(argv);
    bprm->argv = argv;
    bprm->envc = count(envp);
    bprm->envp = envp;

    retval = prepare_binprm(bprm);

    if(retval>=0) {
        if (bprm->buf[0] == 0x7f
                && bprm->buf[1] == 'E'
                && bprm->buf[2] == 'L'
                && bprm->buf[3] == 'F') {
            retval = load_elf_binary(bprm, regs, infop);
        } else {
            fprintf(stderr, "Unknown binary format\n");
            return -1;
        }
    }

    if(retval>=0) {
        /* success.  Initialize important registers */
        do_init_thread(regs, infop);
        return retval;
    }

    /* Something went wrong, return the inode and free the argument pages*/
    for (i=0 ; i<MAX_ARG_PAGES ; i++) {
        g_free(bprm->page[i]);
    }
    return(retval);
}
