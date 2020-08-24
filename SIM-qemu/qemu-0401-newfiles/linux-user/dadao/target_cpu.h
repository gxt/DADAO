#ifndef DADAO_TARGET_CPU_H
#define DADAO_TARGET_CPU_H

static inline void cpu_clone_regs(CPUDADAOState *env, target_ulong newsp)
{
    if (newsp) {
        env->__REG_PC = newsp;
    }

    env->__REG_PC = 0;
}

static inline void cpu_set_tls(CPUDADAOState *env, target_ulong newtls)
{
    env->__REG_PC = newtls;
}

static inline abi_ulong get_sp_from_cpustate(CPUDADAOState *env)
{
   return env->__REG_PC;
}

#endif
