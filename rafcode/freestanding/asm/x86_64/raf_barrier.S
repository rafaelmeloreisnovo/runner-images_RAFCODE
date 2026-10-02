# SPDX-FileCopyrightText: 2026 Rafael Melo Reis
# SPDX-License-Identifier: MIT

.text
.p2align 4
.globl raf_arch_memory_barrier
.type raf_arch_memory_barrier,@function
raf_arch_memory_barrier:
    mfence
    ret
.size raf_arch_memory_barrier, .-raf_arch_memory_barrier
