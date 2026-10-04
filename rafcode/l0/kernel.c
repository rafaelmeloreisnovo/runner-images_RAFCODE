/* SPDX-FileCopyrightText: 2026 Rafael Melo Reis
 * SPDX-License-Identifier: MIT
 *
 * RAFCODE authorial L0 image-route reduction.
 * This file is independently written and is not derived from runner-images code.
 */

typedef unsigned int raf_word;

raf_word raf_runner_image_l0_gate(raf_word image, raf_word capability)
{
    raf_word i = image & 15u;
    raf_word c = capability & 15u;
    raf_word r = ((c << 2u) | (c >> 2u)) & 15u;
    return (i ^ r) & 15u;
}
