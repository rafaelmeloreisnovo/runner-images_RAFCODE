/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_legacy_core.h"

static int test_overlap(void)
{
    struct raf_overlap_cell cells_a[4];
    struct raf_overlap_cell cells_b[4];
    struct raf_overlap_field field_a;
    struct raf_overlap_field field_b;
    struct raf_overlap_cell read_a;
    struct raf_overlap_cell read_again;
    struct raf_event e1;
    struct raf_event e2;
    raf_u32 locus_a = 0U;
    raf_u32 locus_b = 0U;

    e1.kind = RAF_EVENT_IRQ;
    e1.source = 7U;
    e1.target = 1U;
    e1.code = 3U;
    e1.relation = 0xA5A55A5AU;
    e1.value = 0x55ULL;

    e2.kind = RAF_EVENT_DMA;
    e2.source = 2U;
    e2.target = 9U;
    e2.code = 4U;
    e2.relation = 0x13572468U;
    e2.value = 0xAA55ULL;

    if (raf_overlap_init(&field_a, cells_a, 4U) != RAF_OK) {
        return 1;
    }
    if (raf_overlap_init(&field_b, cells_b, 4U) != RAF_OK) {
        return 2;
    }
    if (raf_overlap_init(&field_b, cells_b, 3U) != RAF_ERR_RANGE) {
        return 3;
    }
    if (raf_overlap_init(&field_b, cells_b, 4U) != RAF_OK) {
        return 4;
    }

    if (raf_overlap_absorb(&field_a, &e1, &locus_a) != RAF_OK) {
        return 5;
    }
    if (raf_overlap_absorb(&field_a, &e2, 0) != RAF_OK) {
        return 6;
    }

    if (raf_overlap_absorb(&field_b, &e2, 0) != RAF_OK) {
        return 7;
    }
    if (raf_overlap_absorb(&field_b, &e1, &locus_b) != RAF_OK) {
        return 8;
    }

    if (locus_a != locus_b) {
        return 9;
    }

    /* Aggregate overlap is independent of arrival order. */
    if ((field_a.sum_fold != field_b.sum_fold) ||
        (field_a.xor_fold != field_b.xor_fold)) {
        return 10;
    }

    if (raf_overlap_read(&field_a, locus_a, &read_a) != RAF_OK) {
        return 11;
    }
    if (raf_overlap_read(&field_a, locus_a, &read_again) != RAF_OK) {
        return 12;
    }

    /* Read does not consume state: this is not queue semantics. */
    if ((read_a.sum_fold != read_again.sum_fold) ||
        (read_a.xor_fold != read_again.xor_fold) ||
        (read_a.touches != read_again.touches)) {
        return 13;
    }

    if ((read_a.kind_mask & (1U << (RAF_EVENT_IRQ - 1U))) == 0U) {
        return 14;
    }

    e1.kind = RAF_EVENT_NONE;
    if (raf_overlap_absorb(&field_a, &e1, 0) != RAF_ERR_RANGE) {
        return 15;
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
        return 20;
    }
    if (translated != 0x10000004ULL) {
        return 21;
    }
    if (raf_addr_translate(&map, 0x400ULL, &translated) != RAF_ERR_RANGE) {
        return 22;
    }

    map.legacy_base = ~(raf_u64)0 - 1U;
    map.modern_base = 0U;
    map.span = 4U;
    if (raf_addr_map_validate(&map) != RAF_ERR_OVERFLOW) {
        return 23;
    }

    map.legacy_base = 0U;
    map.modern_base = ~(raf_u64)0 - 1U;
    map.span = 4U;
    if (raf_addr_map_validate(&map) != RAF_ERR_OVERFLOW) {
        return 24;
    }

    return 0;
}

int main(void)
{
    int result = test_overlap();
    if (result != 0) {
        return result;
    }
    return test_translate();
}
