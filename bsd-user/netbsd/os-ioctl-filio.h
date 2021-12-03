#ifndef _IOCTL_FILIO_H_
#define _IOCTL_FILIO_H_

/* XXX needs to be fixed for NetBSD dependencies */

/* see sys/filio.h */
#define TARGET_FIOCLEX      TARGET_IO('f', 1)
#define TARGET_FIONCLEX     TARGET_IO('f', 2)
#define TARGET_FIONREAD     TARGET_IOR('f', 127, int)
#define TARGET_FIONBIO      TARGET_IOW('f', 126, int)
#define TARGET_FIOASYNC     TARGET_IOW('f', 125, int)
#define TARGET_FIOSETOWN    TARGET_IOW('f', 124, int)
#define TARGET_FIOGETOWN    TARGET_IOR('f', 123, int)
#define TARGET_FIODTYPE     TARGET_IOR('f', 122, int)
#define TARGET_FIOGETLBA    TARGET_IOR('f', 121, int)

struct target_fiodgname_arg {
    int32_t     len;
    abi_ulong   buf;
};

#define TARGET_FIODGNAME    TARGET_IOW('f', 120,    \
                struct target_fiodgname_arg)
#define TARGET_FIONWRITE    TARGET_IOR('f', 119, int)
#define TARGET_FIONSPACE    TARGET_IOR('f', 118, int)
#define TARGET_FIOSEEKDATA  TARGET_IOWR('f', 97, off_t)
#define TARGET_FIOSEEKHOLE  TARGET_IOWR('f', 98, off_t)

#endif /* !_IOCTL_FILIO_H_ */
