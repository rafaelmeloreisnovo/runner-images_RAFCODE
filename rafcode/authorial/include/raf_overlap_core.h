/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#ifndef RAF_OVERLAP_CORE_H
#define RAF_OVERLAP_CORE_H

typedef unsigned int raf_u32;
typedef unsigned long long raf_u64;
typedef signed int raf_i32;

_Static_assert(sizeof(raf_u32) == 4, "raf_u32 must be 32-bit");
_Static_assert(sizeof(raf_u64) == 8, "raf_u64 must be 64-bit");

enum raf_overlap_status {
    RAF_OVERLAP_OK = 0,
    RAF_OVERLAP_ERR_ARGUMENT = -1,
    RAF_OVERLAP_ERR_RANGE = -2,
    RAF_OVERLAP_ERR_OVERFLOW = -3
};

enum raf_overlap_kind {
    RAF_OVERLAP_IRQ = 1,
    RAF_OVERLAP_DMA = 2,
    RAF_OVERLAP_PORT = 3,
    RAF_OVERLAP_MEMORY = 4,
    RAF_OVERLAP_SERIAL = 5,
    RAF_OVERLAP_BUS = 6,
    RAF_OVERLAP_CONTROL = 7
};

struct raf_overlap_event {
    raf_u32 kind;
    raf_u32 source;
    raf_u32 target;
    raf_u32 code;
    raf_u32 relation;
    raf_u64 value;
};

struct raf_overlap_cell {
    raf_u64 sum_fold;
    raf_u64 xor_fold;
    raf_u32 relation_fold;
    raf_u32 touches;
    raf_u32 kind_mask;
    raf_u32 last_generation;
};

struct raf_overlap_field {
    struct raf_overlap_cell *cells;
    raf_u32 capacity;
    raf_u32 generation;
    raf_u64 sum_fold;
    raf_u64 xor_fold;
};

raf_i32 raf_overlap_field_init(struct raf_overlap_field *field,
                               struct raf_overlap_cell *cells,
                               raf_u32 capacity);
raf_i32 raf_overlap_absorb(struct raf_overlap_field *field,
                           const struct raf_overlap_event *event,
                           raf_u32 *locus_out);
raf_i32 raf_overlap_read(const struct raf_overlap_field *field,
                         raf_u32 locus,
                         struct raf_overlap_cell *out);

#endif
