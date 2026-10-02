<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE authorial clean-room method

## Purpose

This file defines how the RAFCODE core is corrected without turning existing software, third-party repositories, standards implementations, or historical utilities into code templates.

The work separates three things that must not collapse into one another:

`knowledge inventory != implementation expression != evidence`

## Internal-source use

Existing Rafael repositories may be read to recover **project invariants**, vocabulary, failed paths, semantic relationships and design intent. They are not copied mechanically into the new core.

Observed project concepts include:

- freestanding / low-level execution;
- fixed and bounded state;
- bit-level composition;
- relation between CRC-like folds, BITRAF-like state and routing/observation;
- cross-domain mapping between hardware, memory, bus, firmware and runtime;
- preference for replacing linear waiting paths with direct composition where ordering is not an external requirement.

The new implementation must not copy from those sources:

- function bodies;
- conditional trees;
- file skeletons;
- loop structure;
- symbol layout;
- constant sets whose only justification is another implementation;
- comments or explanatory prose as code comments;
- data structures merely because a previous implementation used them.

## External-source use

External specifications may define facts required for interoperability, such as field width, electrical meaning, opcode, packet layout or a published polynomial. They provide obligations, not implementation architecture.

Third-party source code is classified as `QUARANTINE` for clean-room implementation purposes. It can establish provenance, compatibility context or a negative constraint, but it is not an implementation template.

## Derivation procedure

For each capability:

1. Write the physical/protocol/semantic obligation without code.
2. Identify the minimum state that must exist for that obligation.
3. Identify which relations commute and therefore do not require queue order.
4. Identify which relations genuinely require external ordering.
5. Define a finite representation using project-native terminology.
6. Implement from that representation without an implementation source open as a template.
7. Test invariants rather than similarity to another implementation.
8. Record provenance and execution evidence.

## Non-linearity rule

The generic core must not introduce FIFO merely because conventional software would queue the work.

If two contributions can be represented as an order-independent composition, they enter the bounded overlap field directly.

Only a compatibility adapter may impose ordering, and only when an external contract requires it.

## CRC relation rule

CRC is not authentication and is not promoted to a security verdict.

A CRC-derived value may be used as one relation coordinate among other coordinates. In RAFCODE terminology:

`CRC_TOKEN -> RELATION_LAYER`

not

`CRC_TOKEN -> TRUST`

The current overlap core therefore accepts a generic `relation` word. It does not calculate or verify a specific CRC in the core.

## Novelty rule

Original expression is not the same as proven novelty.

Until a separate prior-art review is performed:

`AUTHORIAL_EXPRESSION = OBSERVED`

`NOVELTY = TOKEN_VAZIO`

No README, paper or claim may convert the second state to `PASS` merely because the implementation was written independently.

## Evidence rule

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`

Compilation proves compilation. Hosted tests prove only their tested software invariants. Cross-compilation does not prove real hardware behavior. Electrical, timing, security and novelty claims require their own evidence lanes.
