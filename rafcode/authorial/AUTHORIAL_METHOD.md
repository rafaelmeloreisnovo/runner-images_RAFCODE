<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE authorial clean-room method

`knowledge inventory != implementation expression != evidence`

## Rule

Existing Rafael repositories are read to recover project invariants, failed paths, vocabulary and semantic relations. They are not implementation templates for this core.

The new core does not copy function bodies, condition trees, file skeletons, loop structures, symbol layouts or constants whose only justification is another implementation.

External specifications may define interoperability facts, but they do not define this implementation architecture.

## Derivation

For each capability:

1. state the physical/protocol/semantic obligation without code;
2. identify the minimum finite state;
3. separate relations that commute from relations that genuinely require external ordering;
4. define a project-native low-level representation;
5. implement from that representation;
6. test invariants rather than similarity to prior code;
7. record provenance and execution evidence.

## Non-linearity

The generic core does not introduce FIFO merely because conventional software would queue the work. If contributions can compose without order, they enter the overlap field directly. Ordered compatibility belongs in an adapter only when an external contract requires sequence.

## CRC relation

`CRC_TOKEN -> RELATION_LAYER`

not

`CRC_TOKEN -> TRUST`

The core accepts a generic `relation` word. It neither computes nor verifies a specific CRC and never promotes relation equality to authentication or provenance.

## Novelty

Independent expression does not prove novelty.

`AUTHORIAL_EXPRESSION = OBSERVED`

`NOVELTY = TOKEN_VAZIO`

until a separate prior-art review is completed.

## Evidence

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`

Compilation proves compilation. Hosted tests prove only their tested software invariants. Cross-compilation is not hardware, electrical, timing or security evidence.
