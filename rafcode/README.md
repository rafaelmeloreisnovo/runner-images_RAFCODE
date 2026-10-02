<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE Freestanding Legacy Core

This directory is an **original clean-room layer** for studying and re-expressing legacy computing concepts as small, auditable, freestanding building blocks without rewriting or rebranding the upstream `actions/runner-images` code.

## Scope

The model connects these strata:

`materials/chemistry -> semiconductor devices -> digital logic -> buses -> CPU/memory/I-O -> firmware/boot -> storage -> command runtime -> toolchain -> networking -> observability/security`

Examples covered by the taxonomy include diodes, inductors/coils, capacitors, transformers, transistors (base/collector/emitter), TTL, flip-flops, EPROM/ROM/RAM, IRQ, DMA, ISA, PCI/PCIe, IDE/PATA/SATA, USB, serial/parallel ports, BIOS/boot, MBR, FAT/FAT32, NTFS/ext-family concepts, FIFO/LIFO/queues as legacy reference points, page faults/latency, IPX/SPX and legacy physical/network topologies.

## Design invariants

1. **Freestanding core**: no libc, heap, filesystem, process model, or OS API is required by the generic C module.
2. **Bounded state**: every executable state space has an explicit finite capacity; no hidden dynamic queue is created.
3. **No canonical FIFO**: legacy queues may be modeled by adapters, but the core itself composes events by direct bounded overlap rather than enqueue/dequeue semantics.
4. **Typed relation**: legacy address/port/IRQ/DMA concepts are normalized into explicit events and address maps instead of scattered special cases.
5. **CRC is not trust**: a CRC may enter an event as a compact relation token, but the core never treats equality as authentication, provenance, or integrity proof.
6. **Architecture adapters**: assembly is isolated under `freestanding/asm/<arch>/`; machine-specific barriers do not leak into generic logic.
7. **No proprietary firmware payloads**: BIOS/ROM/EPROM are modeled as interfaces and metadata unless redistribution rights are independently established.
8. **Defensive security boundary**: offensive names from historical tooling are represented only as threat classes, detection fixtures, or prohibited capabilities.
9. **Provenance before claim**: `SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`.
10. **Test state is explicit**: `IMPLEMENTED_UNTESTED != PASS` and missing evidence remains `TOKEN_VAZIO`.

## Authorial primitive

The core now uses a **bounded overlap field**. Each normalized event contributes directly to one deterministic locus. A locus stores two complementary folds plus cardinality/domain metadata:

- `sum_fold` — multiplicity-sensitive composition modulo the machine word;
- `xor_fold` — bitwise overlap view;
- `relation_fold` — compact relation-layer accumulation;
- `touches` — number of contributions;
- `kind_mask` — domains that touched the locus.

There is no `push`, `pop`, `head`, `tail`, or hidden waiting list. Reading a locus does not consume it. The global sum/xor pair is commutative with respect to event arrival order, so the aggregate expresses composition rather than FIFO sequence.

This primitive is **not claimed as cryptographic, collision-resistant, or novel prior art**. Its current status is an original project expression whose novelty remains `UNVERIFIED` until a separate prior-art review.

## Why this structure

Legacy systems often couple electrical reality, register layout, firmware convention, command syntax, and OS behavior in one place. RAFCODE instead keeps the semantic seams explicit:

- **event** — normalized relation between source, target, domain, code, relation token and value;
- **overlap field** — fixed-memory composition without queue semantics;
- **address map** — explicit legacy-to-modern remapping with overflow checks;
- **architecture barrier** — minimal assembler surface for ordering;
- **taxonomy** — preservation/emulation/replacement decisions kept separate from executable code.

See `AUTHORIAL_METHOD.md`, `SEMANTIC_SEAMS.md`, `PROVENANCE.md` and `THREAT_MODEL.md` before extending the module.

## Build intent

The first gate remains intentionally narrow: compile generic C plus tiny assembly adapters to object files with `-ffreestanding -fno-builtin` and warnings-as-errors, then run a hosted logic smoke test. That proves only the tested software invariants and toolchain viability; it does not prove hardware, electrical, timing, security, or external-standard conformance.

## Copyright and third-party material

The repository root carries the upstream MIT license. New original RAFCODE files include per-file SPDX metadata. Existing third-party notices must be preserved. Third-party implementations may be used as provenance/inventory evidence, but they are not implementation templates for this clean-room core.
