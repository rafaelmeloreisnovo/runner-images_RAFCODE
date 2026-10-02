<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE architecture — from matter to machine

## 1. Physical layer

This layer records what constrains the bit physically: materials/chemistry, semiconductor junctions, diode behavior, transistor switching/amplification, capacitors, inductors/coils, transformers and thermal/electrical limits. The software model does not promote an electrical quantity to `PASS` without measurement.

## 2. Logic layer

Physical switching becomes discrete state:

`device -> gate -> latch/flip-flop -> register -> state transition -> memory/control`

TTL, EPROM, ROM, RAM and SLC/MLC are retained as distinct historical/physical semantics. A software queue is not treated as a flip-flop or as a fundamental machine primitive.

## 3. Machine interconnect

Historical buses are decomposed into semantic obligations rather than cloned implementations:

- address selection;
- transfer relation;
- ordering/arbitration when the protocol actually requires it;
- completion/notification;
- electrical/timing constraints kept outside the generic software claim.

ISA, PCI/PCIe, IDE/PATA/ATA/SATA, USB, serial DB9/DB25, LPT, IRQ, DMA, chipset topology, slots, lanes and pin metadata are adapters over those obligations. Their historical implementation shapes are not templates for the core.

## 4. Firmware and boot

The knowledge chain remains:

`reset condition -> firmware contract -> discovery -> target selection -> boot structure -> execution environment`

BIOS/ROM/EPROM are modeled as interfaces/metadata unless redistribution authority is established. Proprietary payloads remain outside the clean-room core.

## 5. Storage and memory

FAT/FAT32, NTFS and ext-family systems remain compatibility knowledge above block storage. Page faults belong to a supplied virtual-memory policy. `fdisk`, disk-management, defrag and repair utilities are tooling/reference classes, not core primitives.

Latency is treated as an observable property of each boundary, never as a single number mixing propagation, transfer, storage and scheduling.

## 6. Command/runtime preservation

DOS-era command/runtime names are retained as behavioral reference points only. Replacement procedure:

`required behavior -> semantic obligation -> original low-level contract -> bounded implementation -> execution evidence`

No command source, conditional tree or program skeleton is copied into the clean-room path.

## 7. Networking preservation

IPX/SPX, BNC/coax, token/ring-era topologies and AT-command control are preserved as protocol/topology knowledge. Unsafe historical capabilities remain quarantined. Protocol ordering required for interoperability is implemented in an isolated adapter; it does not force the generic core to become a queue.

## 8. Authorial overlap core

The generic event is:

`{kind, source, target, code, relation, value}`

`relation` may carry a CRC-derived token or another compact relation value, but it is **not an integrity verdict**.

The event is not pushed into a FIFO. It is folded directly into a deterministic, finite locus in a static overlap field. Each locus stores:

`{sum_fold, xor_fold, relation_fold, touches, kind_mask, last_generation}`

The two global folds are updated directly from each contribution. Because sum and XOR composition are commutative, their aggregate is independent of arrival order. `last_generation` is observability metadata only; it does not define the composed value.

This creates an explicit distinction:

`sequence required by external protocol` != `core processing queue`

An adapter may expose an ordered compatibility surface when the outside contract requires it, while the core itself remains a bounded composition field.

### CRC relation rule

A CRC is useful as a compact polynomial relation over bits, but in this architecture:

`CRC_MATCH != TRUST`

and

`CRC_TOKEN -> overlap relation -> routing/observation input`

rather than `CRC_TOKEN -> authenticated/verified`.

The existing RMR material shows a project lineage where rolling CRC state is combined with other state and route signatures; that is retained as **authorial-concept evidence**, not copied as implementation shape.

## 9. Semantic seams

The architecture is governed at the places where disciplines touch:

`materials ↔ device ↔ logic ↔ timing ↔ memory ↔ interconnect ↔ addressing ↔ firmware ↔ storage ↔ runtime ↔ observability/security`

Each seam must state what crosses it, the unit/representation, bounds, authority and evidence. `SEMANTIC_SEAMS.md` is the inventory; no seam becomes executable merely because the adjacent domains are individually understood.

## 10. Toolchain

`source -> preprocess -> compile/assemble -> object -> link -> image -> load -> execute -> observe`

Compiler success is not execution evidence; execution evidence is not electrical evidence; a benchmark is not a security proof.

## 11. State policy

Legacy material is classified as `PRESERVE_DOC`, `MODEL`, `EMULATE`, `REPLACE`, `QUARANTINE`, `REMOVE_CANDIDATE`, or `TOKEN_VAZIO`.

Novelty is separate from correctness. New project code may be original in expression while prior-art status remains `NOVELTY_UNVERIFIED` until separately checked.
