<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE architecture — from matter to machine

## 1. Physical layer

This layer describes **what constrains the bit physically**, not merely logically.

- **materials/chemistry** — conductor, semiconductor, dielectric, doping, oxidation, diffusion, junction and thermal behavior;
- **diode** — directional junction behavior and rectification/clamping models;
- **transistor** — switching/amplification primitive; BJT terminology includes base, collector and emitter;
- **capacitor** — charge storage, filtering, decoupling and timing;
- **inductor/coil** — magnetic energy storage, filtering and switching behavior;
- **transformer** — magnetic coupling and isolation/voltage transformation.

The freestanding software layer does **not** pretend these analog quantities are software variables. Electrical models require separate evidence and measurement.

## 2. Logic layer

Physical switching becomes discrete state:

`transistor network -> gate -> latch -> flip-flop -> register -> counter/state machine -> memory/control`

TTL is treated as a historical logic-family constraint. Flip-flops are represented conceptually as state-holding elements; queues are not called “flip-flops” because a queue is a software/data-structure abstraction built from many state elements.

EPROM/ROM/RAM are separated by mutability and persistence semantics. SLC/MLC are flash-cell storage-density categories and belong in storage-media modeling, not CPU RAM semantics.

## 3. Machine interconnect

Historical names are normalized into four primitives:

1. **address space** — where a target is selected;
2. **transport** — how payload moves;
3. **arbitration/ordering** — who may move it and when;
4. **notification** — how completion or urgency is signaled.

Examples:

- ISA / PCI / PCIe -> interconnect + configuration + address windows;
- PCIe lanes -> serialized point-to-point transport width;
- IDE/PATA / ATA / SATA -> storage command and transport generations;
- USB -> host-controlled serial bus with endpoints/transactions;
- DB9/DB25 serial and parallel/LPT -> legacy external communication interfaces;
- IRQ -> asynchronous notification identity;
- DMA -> transfer ownership/path that reduces CPU-managed copying;
- FIFO/queue -> ordered buffering primitive;
- LIFO -> stack discipline, intentionally distinct from FIFO.

Northbridge/southbridge are retained as **historical topology labels**. Modern systems often integrate functions once split across those chips into the CPU package/SoC and a platform controller; therefore the taxonomy records function rather than assuming one fixed chipset layout.

## 4. Firmware and boot

The model separates:

`reset vector -> firmware policy -> device discovery -> boot target selection -> boot record/loader -> operating environment`

BIOS/ROM emulation in this project means **interface/state emulation**, not redistribution of proprietary ROM images. MBR is modeled as a boot/storage structure, not as a general filesystem.

## 5. Storage and memory

- FAT/FAT32, NTFS and ext-family filesystems are adapters above block storage.
- page faults belong to virtual-memory policy and are not available in a truly freestanding environment unless a higher layer supplies address translation/paging.
- defrag/chkdsk/fdisk/disk-management utilities belong to OS/tooling strata, not the core.
- latency must be measured per layer; one latency number must not mix electrical propagation, bus transaction, storage access and scheduler delay.

## 6. Command/runtime preservation

Legacy names such as `AUTOEXEC.BAT`, `CONFIG.SYS`, `IO.SYS`, `MSDOS.SYS`, `IBMDOS`, `COMMAND.COM`, `DEBUG.COM`, `QBASIC`, `DOSKEY`, `TREE`, `ATTRIB`, `CHOICE`, `NET USE`, `NBTSTAT`, `NSLOOKUP` and similar tools are recorded as **behavioral reference points**.

The authorial replacement strategy is not to clone their source or command text. Instead:

- identify the primitive behavior;
- define an original typed interface;
- implement only the required behavior;
- attach provenance and tests;
- leave unsupported behavior explicit.

## 7. Networking preservation

IPX/SPX, BNC/coax Ethernet, ring/token-ring-era concepts and serial AT-command control can be represented as protocol/topology state machines. The generic core only transports normalized events; protocol-specific parsers belong in isolated adapters.

## 8. Authorial primitive: normalized event fabric

RAFCODE uses a small event cell instead of one structure per historical bus:

`{kind, source, target, code, flags, value}`

The same cell can describe a simulated IRQ, DMA completion, port access, memory event, serial symbol, bus event or control transition. A bounded ring carries cells without heap allocation. Address maps translate legacy windows to a selected modern/simulated region.

This is deliberately **not** a cycle-accurate hardware emulator. It is an auditable substrate from which specific emulators/adapters can be built and tested independently.

## 9. Toolchain

Assembler, compiler, linker, preprocessor/precompiler and debugger roles remain separate:

`source -> preprocess -> compile/assemble -> object -> link -> image -> load -> execute -> observe`

A compiler accepting a file is not execution evidence. A linker producing an ELF/PE image is not hardware evidence. Runtime debugger output is evidence only for the executed target/configuration.

## 10. State policy for legacy material

Every legacy item should eventually receive one state:

- `PRESERVE_DOC` — keep knowledge/reference only;
- `MODEL` — create a clean-room state model;
- `EMULATE` — implement a bounded compatibility adapter;
- `REPLACE` — replace behavior with a simpler original primitive;
- `QUARANTINE` — retain only as non-executable evidence/reference;
- `REMOVE_CANDIDATE` — redundant/obsolete for the target, pending proof;
- `TOKEN_VAZIO` — authority/evidence not yet sufficient.

No item becomes `PASS` solely because it is familiar or historically common.
