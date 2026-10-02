/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 */

#include "raf_legacy_core.h"

#define RAF_FOLD_SEED 0x524146434F444531ULL /* "RAFCODE1" */

static raf_u64 raf_rotl64(raf_u64 x, raf_u32 shift)
{
    return (x << shift) | (x >> (64U - shift));
}

static raf_u32 raf_shift_from_u32(raf_u32 x)
{
    return (x & 63U) | 1U;
}

static void raf_overlap_cell_zero(struct raf_overlap_cell *cell)
{
    cell->sum_fold = 0U;
    cell->xor_fold = 0U;
    cell->relation_fold = 0U;
    cell->touches = 0U;
    cell->kind_mask = 0U;
    cell->last_generation = 0U;
}

static raf_u64 raf_event_fold(const struct raf_event *event)
{
    raf_u64 endpoints;
    raf_u64 relation_code;
    raf_u64 domain_value;
    raf_u64 folded;
    raf_u32 s0;
    raf_u32 s1;

    endpoints = ((raf_u64)event->source << 32U) | (raf_u64)event->target;
    relation_code = ((raf_u64)event->relation << 32U) | (raf_u64)event->code;
    domain_value = ((raf_u64)event->kind << 56U) ^ event->value;

    s0 = raf_shift_from_u32(event->source ^ event->target ^ event->relation ^ event->kind);
    s1 = raf_shift_from_u32(event->code ^ event->relation ^ (event->kind << 3U));

    folded = domain_value ^ raf_rotl64(endpoints, s0) ^ raf_rotl64(relation_code, s1);
    folded = raf_rotl64(folded + endpoints + RAF_FOLD_SEED, s1) ^ relation_code;
    return folded;
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

raf_i32 raf_overlap_validate(const struct raf_overlap_field *field)
{
    if ((field == (const struct raf_overlap_field *)0) ||
        (field->cells == (struct raf_overlap_cell *)0) ||
        (field->capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    /* Power-of-two capacity makes locus selection a bounded mask operation. */
    if ((field->capacity & (field->capacity - 1U)) != 0U) {
        return RAF_ERR_RANGE;
    }

    return RAF_OK;
}

raf_i32 raf_overlap_init(struct raf_overlap_field *field,
                         struct raf_overlap_cell *cells,
                         raf_u32 capacity)
{
    raf_u32 i;

    if ((field == (struct raf_overlap_field *)0) ||
        (cells == (struct raf_overlap_cell *)0) ||
        (capacity == 0U)) {
        return RAF_ERR_ARGUMENT;
    }

    if ((capacity & (capacity - 1U)) != 0U) {
        return RAF_ERR_RANGE;
    }

    field->cells = cells;
    field->capacity = capacity;
    field->generation = 0U;
    field->sum_fold = 0U;
    field->xor_fold = 0U;

    i = 0U;
    while (i < capacity) {
        raf_overlap_cell_zero(&cells[i]);
        i += 1U;
    }

    return RAF_OK;
}

raf_i32 raf_overlap_absorb(struct raf_overlap_field *field,
                           const struct raf_event *event,
                           raf_u32 *locus_out)
{
    raf_i32 status;
    raf_u64 contribution;
    raf_u64 relation_lane;
    raf_u32 locus;
    raf_u32 shift;
    struct raf_overlap_cell *cell;

    status = raf_overlap_validate(field);
    if (status != RAF_OK) {
        return status;
    }

    status = raf_event_validate(event);
    if (status != RAF_OK) {
        return status;
    }

    if (field->generation == ~(raf_u32)0) {
        return RAF_ERR_OVERFLOW;
    }

    contribution = raf_event_fold(event);
    locus = (raf_u32)(contribution ^ (contribution >> 32U) ^ (raf_u64)event->relation) &
            (field->capacity - 1U);
    cell = &field->cells[locus];

    if (cell->touches == ~(raf_u32)0) {
        return RAF_ERR_OVERFLOW;
    }

    shift = raf_shift_from_u32(event->kind ^ event->code ^ event->relation);
    relation_lane = ((raf_u64)event->relation << 32U) | (raf_u64)event->code;
    contribution ^= raf_rotl64(event->value ^ relation_lane, shift);

    /*
     * No enqueue/dequeue exists here.  Each event contributes directly to a
     * bounded locus.  The sum/xor pair is commutative with respect to arrival
     * order, so the overlap state represents composition rather than a queue.
     */
    cell->sum_fold += contribution;
    cell->xor_fold ^= contribution;
    cell->relation_fold += event->relation ^ event->code;
    cell->touches += 1U;
    cell->kind_mask |= (1U << (event->kind - 1U));

    field->generation += 1U;
    cell->last_generation = field->generation;
    field->sum_fold += contribution;
    field->xor_fold ^= contribution;

    if (locus_out != (raf_u32 *)0) {
        *locus_out = locus;
    }

    return RAF_OK;
}

raf_i32 raf_overlap_read(const struct raf_overlap_field *field,
                         raf_u32 locus,
                         struct raf_overlap_cell *cell_out)
{
    raf_i32 status = raf_overlap_validate(field);

    if (status != RAF_OK) {
        return status;
    }

    if (cell_out == (struct raf_overlap_cell *)0) {
        return RAF_ERR_ARGUMENT;
    }

    if (locus >= field->capacity) {
        return RAF_ERR_RANGE;
    }

    *cell_out = field->cells[locus];
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
