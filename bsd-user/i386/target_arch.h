
#ifndef _TARGET_ARCH_H_
#define _TARGET_ARCH_H_

/* target_arch_cpu.c */
void bsd_i386_write_dt(void *ptr, unsigned long addr, unsigned long limit,
                int flags);
void bsd_i386_set_idt(int n, unsigned int dpl);
void bsd_i386_set_idt_base(uint64_t base);

#define target_cpu_set_tls(env, newtls)

#endif /* ! _TARGET_ARCH_H_ */
