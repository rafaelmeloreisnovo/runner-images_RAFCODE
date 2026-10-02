<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE Freestanding Legacy Core

This directory is an **original clean-room layer** for studying and re-expressing legacy computing concepts as small, auditable, freestanding building blocks without rewriting or rebranding the upstream `actions/runner-images` code.

## Scope

The model connects these strata:

`materials/chemistry -> semiconductor devices -> digital logic -> buses -> CPU/memory/I-O -> firmware/boot -> storage -> command runtime -> toolchain -> networking -> observability/security`

Examples covered by the taxonomy include diodes, inductors/coils, capacitors, transformers, transistors (base/collector/emitter), TTL, flip-flops, EPROM/ROM/RAM, IRQ, DMA, ISA, PCI/PCIe, IDE/PATA/SATA, USB, serial/parallel ports, BIOS/boot, MBR, FAT/FAT32, NTFS/ext-family concepts, FIFO/LIFO/queues, page faults/latency, IPX/SPX and legacy physical/network topologies.

## Design invariants

1. **Freestanding core**: no libc, heap, filesystem, process model, or OS API is required by the core C module.
2. **Bounded state**: queues and maps have explicit capacities; overflow is returned as state, never hidden.
3. **Typed translation**: legacy address/port/IRQ/DMA concepts are normalized into explicit events and address maps instead of scattered special cases.
4. **Architecture adapters**: assembly is isolated under `freestanding/asm/<arch>/`; machine-specific barriers do not leak into generic logic.
5. **No proprietary firmware payloads**: BIOS/ROM/EPROM are modeled as interfaces and metadata unless redistribution rights are independently established.
6. **Defensive security boundary**: offensive names from historical tooling are represented only as threat classes, detection fixtures, or prohibited capabilities. No cracking, flooding, spoofing, packet injection, botnet, backdoor, or destructive feature belongs in this module.
7. **Provenance before claim**: `SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`.
8. **Test state is explicit**: `IMPLEMENTED_UNTESTED != PASS` and missing evidence remains `TOKEN_VAZIO`.

## Why this structure

Legacy systems often couple electrical reality, register layout, firmware convention, command syntax, and OS behavior in one place. This layer separates them into stable primitives:

- **event** — one normalized unit for IRQ, DMA, port, memory, serial, bus and control transitions;
- **bounded ring** — FIFO transport without allocation;
- **address map** — explicit legacy-to-modern remapping with overflow checks;
- **architecture barrier** — the smallest assembler surface needed to make ordering visible;
- **taxonomy** — preservation/emulation/replacement decisions kept separate from executable code.

This makes it possible to preserve knowledge of DOS-era and PC-era mechanisms without requiring obsolete binaries or unsafe behavior.

## Build intent

The first gate is intentionally narrow: compile generic C plus tiny assembly adapters to object files with `-ffreestanding -fno-builtin` and warnings-as-errors. That proves **syntactic/toolchain viability only**; it does not prove hardware correctness, timing correctness, electrical correctness, or conformance to any external standard.

## Copyright and third-party material

The repository root currently carries the upstream MIT license. New original RAFCODE files include per-file SPDX metadata. Existing third-party notices must be preserved. If third-party code is later incorporated, its own copyright holder(s) and compatible license expression must be recorded rather than replaced.

See `PROVENANCE.md` and `THREAT_MODEL.md` before extending this module.
