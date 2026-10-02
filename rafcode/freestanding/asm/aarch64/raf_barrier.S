# SPDX-FileCopyrightText: 2026 Rafael Melo Reis
# SPDX-License-Identifier: MIT

.text
.align 2
.global raf_arch_memory_barrier
.type raf_arch_memory_barrier,%function
raf_arch_memory_barrier:
    dmb sy
    ret
.size raf_arch_memory_barrier, .-raf_arch_memory_barrier
.section .note.GNU-stack,"",%progbits
