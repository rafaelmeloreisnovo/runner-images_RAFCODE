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
    in.flags = 0U;
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
    if (raf_ring_pop(&ring, &out) != RAF_ERR_EMPTY) {
        return 5;
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
    map.flags = 0U;

    if (raf_addr_translate(&map, 0x3FCULL, &translated) != RAF_OK) {
        return 10;
    }
    if (translated != 0x10000004ULL) {
        return 11;
    }
    if (raf_addr_translate(&map, 0x400ULL, &translated) != RAF_ERR_RANGE) {
        return 12;
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
