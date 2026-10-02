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
    RAF_ERR_FULL = -2,
    RAF_ERR_EMPTY = -3,
    RAF_ERR_RANGE = -4,
    RAF_ERR_OVERFLOW = -5
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

struct raf_event {
    raf_u32 kind;
    raf_u32 source;
    raf_u32 target;
    raf_u32 code;
    raf_u32 flags;
    raf_u64 value;
};

struct raf_ring {
    struct raf_event *cells;
    raf_u32 capacity;
    raf_u32 head;
    raf_u32 tail;
    raf_u32 count;
};

struct raf_addr_map {
    raf_u64 legacy_base;
    raf_u64 modern_base;
    raf_u64 span;
    raf_u32 flags;
};

raf_i32 raf_ring_init(struct raf_ring *ring, struct raf_event *cells, raf_u32 capacity);
raf_i32 raf_ring_push(struct raf_ring *ring, const struct raf_event *event);
raf_i32 raf_ring_pop(struct raf_ring *ring, struct raf_event *event_out);
raf_i32 raf_addr_translate(const struct raf_addr_map *map, raf_u64 legacy_addr, raf_u64 *modern_addr_out);

#endif
