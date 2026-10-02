/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_legacy_core.h"

static void raf_event_zero(struct raf_event *event)
{
    event->kind = 0U;
    event->source = 0U;
    event->target = 0U;
    event->code = 0U;
    event->value = 0U;
}

raf_i32 raf_event_validate(const struct raf_event *event)
{
    if (event == (const struct raf_event *)0) {
        return RAF_ERR_ARGUMENT;
    }

    if ((event->kind == RAF_EVENT_NONE) ||
        (event->kind > RAF_EVENT_KIND_MAX)) {
        return RAF_ERR_RANGE;
    }

    return RAF_OK;
}

raf_i32 raf_ring_validate(const struct raf_ring *ring)
{
    raf_u32 expected_tail;
    raf_u32 to_end;

    if ((ring == (const struct raf_ring *)0) ||
        (ring->cells == (struct raf_event *)0) ||
        (ring->capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    if ((ring->head >= ring->capacity) ||
        (ring->tail >= ring->capacity) ||
        (ring->count > ring->capacity)) {
        return RAF_ERR_STATE;
    }

    to_end = ring->capacity - ring->head;
    if (ring->count >= to_end) {
        expected_tail = ring->count - to_end;
    } else {
        expected_tail = ring->head + ring->count;
    }

    if (ring->tail != expected_tail) {
        return RAF_ERR_STATE;
    }

    return RAF_OK;
}

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
    raf_i32 status = raf_ring_validate(ring);

    if (status != RAF_OK) {
        return status;
    }

    status = raf_event_validate(event);
    if (status != RAF_OK) {
        return status;
    }

    if (ring->count == ring->capacity) {
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
    raf_i32 status = raf_ring_validate(ring);

    if (status != RAF_OK) {
        return status;
    }

    if (event_out == (struct raf_event *)0) {
        return RAF_ERR_ARGUMENT;
    }

    if (ring->count == 0U) {
        return RAF_ERR_EMPTY;
    }

    if (event_out == &ring->cells[ring->head]) {
        return RAF_ERR_ARGUMENT;
    }

    *event_out = ring->cells[ring->head];
    raf_event_zero(&ring->cells[ring->head]);
    ring->head += 1U;
    if (ring->head == ring->capacity) {
        ring->head = 0U;
    }
    ring->count -= 1U;
    return RAF_OK;
}

raf_i32 raf_addr_map_validate(const struct raf_addr_map *map)
{
    raf_u64 last_offset;
    raf_u64 max_u64 = ~(raf_u64)0;

    if ((map == (const struct raf_addr_map *)0) ||
        (map->span == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    last_offset = map->span - 1U;
    if ((map->legacy_base > (max_u64 - last_offset)) ||
        (map->modern_base > (max_u64 - last_offset))) {
        return RAF_ERR_OVERFLOW;
    }

    return RAF_OK;
}

raf_i32 raf_addr_translate(const struct raf_addr_map *map,
                           raf_u64 legacy_addr,
                           raf_u64 *modern_addr_out)
{
    raf_i32 status = raf_addr_map_validate(map);
    raf_u64 offset;

    if (status != RAF_OK) {
        return status;
    }

    if (modern_addr_out == (raf_u64 *)0) {
        return RAF_ERR_ARGUMENT;
    }

    if (legacy_addr < map->legacy_base) {
        return RAF_ERR_RANGE;
    }

    offset = legacy_addr - map->legacy_base;
    if (offset >= map->span) {
        return RAF_ERR_RANGE;
    }

    *modern_addr_out = map->modern_base + offset;
    return RAF_OK;
}
