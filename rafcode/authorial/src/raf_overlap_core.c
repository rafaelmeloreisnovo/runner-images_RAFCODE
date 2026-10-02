/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_overlap_core.h"

#define RAF_OVERLAP_SEED 0x524146434F444532ULL /* "RAFCODE2" */

static raf_u32 raf_overlap_shift(raf_u32 x)
{
    return (x & 63U) | 1U;
}

static raf_u64 raf_overlap_rotl64(raf_u64 x, raf_u32 shift)
{
    return (x << shift) | (x >> (64U - shift));
}

static void raf_overlap_zero(struct raf_overlap_cell *cell)
{
    cell->sum_fold = 0U;
    cell->xor_fold = 0U;
    cell->relation_fold = 0U;
    cell->touches = 0U;
    cell->kind_mask = 0U;
    cell->last_generation = 0U;
}

static raf_i32 raf_overlap_event_check(const struct raf_overlap_event *event)
{
    if (event == (const struct raf_overlap_event *)0) {
        return RAF_OVERLAP_ERR_ARGUMENT;
    }
    if ((event->kind < RAF_OVERLAP_IRQ) || (event->kind > RAF_OVERLAP_CONTROL)) {
        return RAF_OVERLAP_ERR_RANGE;
    }
    return RAF_OVERLAP_OK;
}

static raf_i32 raf_overlap_field_check(const struct raf_overlap_field *field)
{
    if ((field == (const struct raf_overlap_field *)0) ||
        (field->cells == (struct raf_overlap_cell *)0) ||
        (field->capacity == 0U)) {
        return RAF_OVERLAP_ERR_ARGUMENT;
    }
    if ((field->capacity & (field->capacity - 1U)) != 0U) {
        return RAF_OVERLAP_ERR_RANGE;
    }
    return RAF_OVERLAP_OK;
}

static raf_u64 raf_overlap_contribution(const struct raf_overlap_event *event)
{
    raf_u64 endpoints;
    raf_u64 relation_code;
    raf_u64 folded;
    raf_u32 a;
    raf_u32 b;

    endpoints = ((raf_u64)event->source << 32U) | (raf_u64)event->target;
    relation_code = ((raf_u64)event->relation << 32U) | (raf_u64)event->code;
    a = raf_overlap_shift(event->source ^ event->target ^ event->relation ^ event->kind);
    b = raf_overlap_shift(event->code ^ event->relation ^ (event->kind << 3U));

    folded = event->value ^ ((raf_u64)event->kind << 56U);
    folded ^= raf_overlap_rotl64(endpoints, a);
    folded ^= raf_overlap_rotl64(relation_code, b);
    folded = raf_overlap_rotl64(folded + endpoints + RAF_OVERLAP_SEED, b) ^ relation_code;
    return folded;
}

raf_i32 raf_overlap_field_init(struct raf_overlap_field *field,
                               struct raf_overlap_cell *cells,
                               raf_u32 capacity)
{
    raf_u32 i;

    if ((field == (struct raf_overlap_field *)0) ||
        (cells == (struct raf_overlap_cell *)0) ||
        (capacity == 0U)) {
        return RAF_OVERLAP_ERR_ARGUMENT;
    }
    if ((capacity & (capacity - 1U)) != 0U) {
        return RAF_OVERLAP_ERR_RANGE;
    }

    field->cells = cells;
    field->capacity = capacity;
    field->generation = 0U;
    field->sum_fold = 0U;
    field->xor_fold = 0U;

    i = 0U;
    while (i < capacity) {
        raf_overlap_zero(&cells[i]);
        i += 1U;
    }
    return RAF_OVERLAP_OK;
}

raf_i32 raf_overlap_absorb(struct raf_overlap_field *field,
                           const struct raf_overlap_event *event,
                           raf_u32 *locus_out)
{
    raf_i32 status;
    raf_u64 contribution;
    raf_u32 locus;
    struct raf_overlap_cell *cell;

    status = raf_overlap_field_check(field);
    if (status != RAF_OVERLAP_OK) {
        return status;
    }
    status = raf_overlap_event_check(event);
    if (status != RAF_OVERLAP_OK) {
        return status;
    }
    if (field->generation == ~(raf_u32)0) {
        return RAF_OVERLAP_ERR_OVERFLOW;
    }

    contribution = raf_overlap_contribution(event);
    locus = (raf_u32)(contribution ^ (contribution >> 32U) ^ (raf_u64)event->relation) &
            (field->capacity - 1U);
    cell = &field->cells[locus];

    if (cell->touches == ~(raf_u32)0) {
        return RAF_OVERLAP_ERR_OVERFLOW;
    }

    cell->sum_fold += contribution;
    cell->xor_fold ^= contribution;
    cell->relation_fold += event->relation ^ event->code;
    cell->touches += 1U;
    cell->kind_mask |= 1U << (event->kind - 1U);

    field->generation += 1U;
    cell->last_generation = field->generation;
    field->sum_fold += contribution;
    field->xor_fold ^= contribution;

    if (locus_out != (raf_u32 *)0) {
        *locus_out = locus;
    }
    return RAF_OVERLAP_OK;
}

raf_i32 raf_overlap_read(const struct raf_overlap_field *field,
                         raf_u32 locus,
                         struct raf_overlap_cell *out)
{
    raf_i32 status = raf_overlap_field_check(field);

    if (status != RAF_OVERLAP_OK) {
        return status;
    }
    if (out == (struct raf_overlap_cell *)0) {
        return RAF_OVERLAP_ERR_ARGUMENT;
    }
    if (locus >= field->capacity) {
        return RAF_OVERLAP_ERR_RANGE;
    }

    *out = field->cells[locus];
    return RAF_OVERLAP_OK;
}
