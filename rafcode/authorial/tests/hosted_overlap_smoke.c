/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_overlap_core.h"

static int test_overlap_order(void)
{
    struct raf_overlap_cell cells_a[4];
    struct raf_overlap_cell cells_b[4];
    struct raf_overlap_field a;
    struct raf_overlap_field b;
    struct raf_overlap_event e1;
    struct raf_overlap_event e2;
    struct raf_overlap_cell r1;
    struct raf_overlap_cell r2;
    raf_u32 l1 = 0U;
    raf_u32 l2 = 0U;

    e1.kind = RAF_OVERLAP_IRQ;
    e1.source = 7U;
    e1.target = 1U;
    e1.code = 3U;
    e1.relation = 0xA5A55A5AU;
    e1.value = 0x55ULL;

    e2.kind = RAF_OVERLAP_DMA;
    e2.source = 2U;
    e2.target = 9U;
    e2.code = 4U;
    e2.relation = 0x13572468U;
    e2.value = 0xAA55ULL;

    if (raf_overlap_field_init(&a, cells_a, 4U) != RAF_OVERLAP_OK) return 1;
    if (raf_overlap_field_init(&b, cells_b, 4U) != RAF_OVERLAP_OK) return 2;

    if (raf_overlap_absorb(&a, &e1, &l1) != RAF_OVERLAP_OK) return 3;
    if (raf_overlap_absorb(&a, &e2, 0) != RAF_OVERLAP_OK) return 4;

    if (raf_overlap_absorb(&b, &e2, 0) != RAF_OVERLAP_OK) return 5;
    if (raf_overlap_absorb(&b, &e1, &l2) != RAF_OVERLAP_OK) return 6;

    if (l1 != l2) return 7;
    if ((a.sum_fold != b.sum_fold) || (a.xor_fold != b.xor_fold)) return 8;

    if (raf_overlap_read(&a, l1, &r1) != RAF_OVERLAP_OK) return 9;
    if (raf_overlap_read(&a, l1, &r2) != RAF_OVERLAP_OK) return 10;

    if ((r1.sum_fold != r2.sum_fold) ||
        (r1.xor_fold != r2.xor_fold) ||
        (r1.touches != r2.touches)) return 11;

    if ((r1.kind_mask & (1U << (RAF_OVERLAP_IRQ - 1U))) == 0U) return 12;

    return 0;
}

static int test_bounds(void)
{
    struct raf_overlap_cell cells[4];
    struct raf_overlap_field field;
    struct raf_overlap_event event;

    if (raf_overlap_field_init(&field, cells, 3U) != RAF_OVERLAP_ERR_RANGE) return 20;
    if (raf_overlap_field_init(&field, cells, 4U) != RAF_OVERLAP_OK) return 21;

    event.kind = 0U;
    event.source = 0U;
    event.target = 0U;
    event.code = 0U;
    event.relation = 0U;
    event.value = 0U;

    if (raf_overlap_absorb(&field, &event, 0) != RAF_OVERLAP_ERR_RANGE) return 22;
    return 0;
}

int main(void)
{
    int rc = test_overlap_order();
    if (rc != 0) return rc;
    return test_bounds();
}
