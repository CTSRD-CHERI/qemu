
#ifndef _TARGET_ARCH_H_
#define _TARGET_ARCH_H_

#define TARGET_UT_MAX       36
#define TARGET_UTH_NOCHANGE  -1

void bsd_sparc64_save_window(CPUSPARCState *env);
void bsd_sparc64_restore_window(CPUSPARCState *env);
void bsd_sparc64_flush_windows(CPUSPARCState *env);

#define target_cpu_set_tls(env, newtls)		\
do {						\
	(env)->gregs[7] = (newtls);		\
} while(0)

#endif /* ! _TARGET_ARCH_H_ */
