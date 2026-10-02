/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_legacy_core.h"

static int test_ring(void)
{
    struct raf_event storage[2];
    struct raf_ring ring;
    struct raf_event in;
    struct raf_event out;

    in.kind = RAF_EVENT_IRQ;
    in.source = 7U;
    in.target = 1U;
    in.code = 3U;
    in.value = 0x55ULL;

    if (raf_ring_init(&ring, storage, 2U) != RAF_OK) {
        return 1;
    }
    if (raf_ring_push(&ring, &in) != RAF_OK) {
        return 2;
    }
    if (raf_ring_pop(&ring, &out) != RAF_OK) {
        return 3;
    }
    if ((out.kind != in.kind) ||
        (out.source != in.source) ||
        (out.target != in.target) ||
        (out.code != in.code) ||
        (out.value != in.value)) {
        return 4;
    }
    if ((storage[0].kind != RAF_EVENT_NONE) ||
        (storage[0].source != 0U) ||
        (storage[0].target != 0U) ||
        (storage[0].code != 0U) ||
        (storage[0].value != 0U)) {
        return 5;
    }
    if (raf_ring_pop(&ring, &out) != RAF_ERR_EMPTY) {
        return 6;
    }

    in.kind = RAF_EVENT_NONE;
    if (raf_ring_push(&ring, &in) != RAF_ERR_RANGE) {
        return 7;
    }
    in.kind = RAF_EVENT_IRQ;

    ring.tail = ring.capacity;
    if (raf_ring_push(&ring, &in) != RAF_ERR_STATE) {
        return 8;
    }

    return 0;
}

static int test_translate(void)
{
    struct raf_addr_map map;
    raf_u64 translated = 0U;

    map.legacy_base = 0x3F8ULL;
    map.modern_base = 0x10000000ULL;
    map.span = 8U;

    if (raf_addr_translate(&map, 0x3FCULL, &translated) != RAF_OK) {
        return 10;
    }
    if (translated != 0x10000004ULL) {
        return 11;
    }
    if (raf_addr_translate(&map, 0x400ULL, &translated) != RAF_ERR_RANGE) {
        return 12;
    }

    map.legacy_base = ~(raf_u64)0 - 1U;
    map.modern_base = 0U;
    map.span = 4U;
    if (raf_addr_map_validate(&map) != RAF_ERR_OVERFLOW) {
        return 13;
    }

    map.legacy_base = 0U;
    map.modern_base = ~(raf_u64)0 - 1U;
    map.span = 4U;
    if (raf_addr_map_validate(&map) != RAF_ERR_OVERFLOW) {
        return 14;
    }

    return 0;
}

int main(void)
{
    int result = test_ring();
    if (result != 0) {
        return result;
    }
    return test_translate();
}
