/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_legacy_core.h"

raf_i32 raf_ring_init(struct raf_ring *ring, struct raf_event *cells, raf_u32 capacity)
{
    if ((ring == (struct raf_ring *)0) ||
        (cells == (struct raf_event *)0) ||
        (capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    ring->cells = cells;
    ring->capacity = capacity;
    ring->head = 0U;
    ring->tail = 0U;
    ring->count = 0U;
    return RAF_OK;
}

raf_i32 raf_ring_push(struct raf_ring *ring, const struct raf_event *event)
{
    if ((ring == (struct raf_ring *)0) ||
        (event == (const struct raf_event *)0) ||
        (ring->cells == (struct raf_event *)0) ||
        (ring->capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    if (ring->count >= ring->capacity) {
        return RAF_ERR_FULL;
    }

    ring->cells[ring->tail] = *event;
    ring->tail += 1U;
    if (ring->tail == ring->capacity) {
        ring->tail = 0U;
    }
    ring->count += 1U;
    return RAF_OK;
}

raf_i32 raf_ring_pop(struct raf_ring *ring, struct raf_event *event_out)
{
    if ((ring == (struct raf_ring *)0) ||
        (event_out == (struct raf_event *)0) ||
        (ring->cells == (struct raf_event *)0) ||
        (ring->capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    if (ring->count == 0U) {
        return RAF_ERR_EMPTY;
    }

    *event_out = ring->cells[ring->head];
    ring->head += 1U;
    if (ring->head == ring->capacity) {
        ring->head = 0U;
    }
    ring->count -= 1U;
    return RAF_OK;
}

raf_i32 raf_addr_translate(const struct raf_addr_map *map,
                           raf_u64 legacy_addr,
                           raf_u64 *modern_addr_out)
{
    raf_u64 offset;
    raf_u64 max_u64 = ~(raf_u64)0;

    if ((map == (const struct raf_addr_map *)0) ||
        (modern_addr_out == (raf_u64 *)0) ||
        (map->span == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    if (legacy_addr < map->legacy_base) {
        return RAF_ERR_RANGE;
    }

    offset = legacy_addr - map->legacy_base;
    if (offset >= map->span) {
        return RAF_ERR_RANGE;
    }

    if (map->modern_base > (max_u64 - offset)) {
        return RAF_ERR_OVERFLOW;
    }

    *modern_addr_out = map->modern_base + offset;
    return RAF_OK;
}
