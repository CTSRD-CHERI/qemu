/*
 * Note: Adding QEMU_ALWAYS_INLINE to probe_access() allows the conditions that
 * depend on access_type to be constant-folded for the probe_read/probe_write
 * helpers. Prior to this optimization about 16% of the total time booting a
 * purecap CheriBSD kernel was spent inside probe_access() and of those 16% a
 * large percentage was spent executing `switch(access_type).`
 */
void *probe_access(CPUArchState *env, target_ulong addr, int size,
                   MMUAccessType access_type, int mmu_idx, uintptr_t ra)
{
    return probe_access_inlined(env, addr, size, access_type, mmu_idx, ra);
}

void *probe_write(CPUArchState *env, target_ulong addr, int size, int mmu_idx,
                  uintptr_t retaddr)
{
    return probe_access_inlined(env, addr, size, MMU_DATA_STORE, mmu_idx,
                                retaddr);
}

void *probe_read(CPUArchState *env, target_ulong addr, int size, int mmu_idx,
                 uintptr_t retaddr)
{
    return probe_access_inlined(env, addr, size, MMU_DATA_LOAD, mmu_idx,
                                retaddr);
}

#ifdef TARGET_CHERI
void *probe_cap_write(CPUArchState *env, target_ulong addr, int size,
                      int mmu_idx, uintptr_t retaddr)
{
    return probe_access_inlined(env, addr, size, MMU_DATA_CAP_STORE, mmu_idx,
                                retaddr);
}

void *probe_ver_write(CPUArchState *env, target_ulong addr, int size,
                      int mmu_idx, uintptr_t retaddr)
{
    return probe_access_inlined(env, addr, size, MMU_VERSION_STORE, mmu_idx,
                                retaddr);
}
#endif
