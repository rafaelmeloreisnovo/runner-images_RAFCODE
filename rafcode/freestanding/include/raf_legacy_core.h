/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#ifndef RAF_LEGACY_CORE_H
#define RAF_LEGACY_CORE_H

typedef unsigned char raf_u8;
typedef unsigned short raf_u16;
typedef unsigned int raf_u32;
typedef unsigned long long raf_u64;
typedef signed int raf_i32;

_Static_assert(sizeof(raf_u8) == 1, "raf_u8 must be 8-bit");
_Static_assert(sizeof(raf_u16) == 2, "raf_u16 must be 16-bit");
_Static_assert(sizeof(raf_u32) == 4, "raf_u32 must be 32-bit");
_Static_assert(sizeof(raf_u64) == 8, "raf_u64 must be 64-bit");

enum raf_status {
    RAF_OK = 0,
    RAF_ERR_ARGUMENT = -1,
    RAF_ERR_RANGE = -2,
    RAF_ERR_OVERFLOW = -3,
    RAF_ERR_STATE = -4
};

enum raf_event_kind {
    RAF_EVENT_NONE = 0,
    RAF_EVENT_IRQ = 1,
    RAF_EVENT_DMA = 2,
    RAF_EVENT_PORT = 3,
    RAF_EVENT_MEMORY = 4,
    RAF_EVENT_SERIAL = 5,
    RAF_EVENT_BUS = 6,
    RAF_EVENT_CONTROL = 7
};

#define RAF_EVENT_KIND_MAX RAF_EVENT_CONTROL

/*
 * relation is deliberately not an integrity verdict.  An adapter may place a
 * CRC or another compact relation token here; the core only folds it into the
 * overlap state.  Equality never means "trusted" or "verified".
 */
struct raf_event {
    raf_u32 kind;
    raf_u32 source;
    raf_u32 target;
    raf_u32 code;
    raf_u32 relation;
    raf_u64 value;
};

/*
 * An overlap cell is a bounded multiset summary, not a FIFO slot.  sum_fold
 * preserves multiplicity modulo 2^64, xor_fold preserves bitwise overlap,
 * touches records cardinality, and kind_mask records participating domains.
 */
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

struct raf_addr_map {
    raf_u64 legacy_base;
    raf_u64 modern_base;
    raf_u64 span;
};

raf_i32 raf_event_validate(const struct raf_event *event);
raf_i32 raf_overlap_validate(const struct raf_overlap_field *field);
raf_i32 raf_overlap_init(struct raf_overlap_field *field,
                         struct raf_overlap_cell *cells,
                         raf_u32 capacity);
raf_i32 raf_overlap_absorb(struct raf_overlap_field *field,
                           const struct raf_event *event,
                           raf_u32 *locus_out);
raf_i32 raf_overlap_read(const struct raf_overlap_field *field,
                         raf_u32 locus,
                         struct raf_overlap_cell *cell_out);
raf_i32 raf_addr_map_validate(const struct raf_addr_map *map);
raf_i32 raf_addr_translate(const struct raf_addr_map *map,
                           raf_u64 legacy_addr,
                           raf_u64 *modern_addr_out);

#endif
