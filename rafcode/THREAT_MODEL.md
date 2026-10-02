<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# Threat model and legacy quarantine boundary

## Objective

Preserve the ability to recognize unsafe or obsolete behaviors **without turning this repository into an offensive toolkit**, while reducing hidden state, implicit trust and residual execution paths.

Historical/security terms such as sniffer, port mirroring, spoofing, flood, ping-nuke-style denial of service, wireless cracking, fake-phone tooling, botnets and backdoors may appear in documentation only as threat categories or controlled defensive test labels.

## Tail / shadow definition

A **tail/shadow** is any state or capability that survives outside the currently documented contract, including:

- residual data after logical removal;
- mutable dependency references;
- unused flags or reserved fields with undefined semantics;
- fallback/default execution paths;
- stale credentials or inherited permissions;
- inconsistent queue/index state;
- address windows whose declared span wraps the address space;
- legacy binaries, firmware or scripts whose provenance is unknown;
- workflow inputs that cross a trust boundary without validation.

The design rule is: if a state is not required and specified, remove it; if it must exist, validate it before use; if it cannot be validated, fail closed with `TOKEN_VAZIO` at the evidence layer.

## Allowed forms

- fixed synthetic packet/event fixtures;
- parser robustness tests against local byte arrays;
- static detection signatures that do not contain deployable payloads;
- configuration checks for accidental promiscuous capture, unexpected listeners, unsigned firmware, hidden persistence or unsafe defaults;
- rate/queue saturation tests performed entirely on in-memory synthetic data;
- SBOM, provenance and dependency review;
- negative tests proving prohibited capabilities are absent.

## Excluded capabilities

This module must not implement:

- credential capture or extraction;
- packet injection or identity impersonation;
- traffic flooding against hosts/networks;
- wireless key cracking;
- persistence/backdoor installation;
- botnet command/control;
- destructive disk/firmware operations;
- evasion or stealth mechanisms intended to defeat monitoring.

## Legacy execution rule

Old binaries, ROMs, firmware images and utilities are **not trusted merely because they are historically known**. Their default state is:

`UNVERIFIED -> QUARANTINE -> identify hash/source/license -> inspect -> bounded test -> evidence -> decision`

Unknown provenance remains `TOKEN_VAZIO`.

## Security properties of the freestanding core

The generic event/ring/address-map layer now enforces:

- no dynamic allocation;
- caller-owned buffers;
- explicit capacities;
- queue invariants revalidated before each push/pop;
- invalid/empty event kinds rejected before enqueue;
- popped slots cleared to remove residual event state;
- complete address-map span overflow checks on both legacy and target spaces;
- no undefined flag/reserved fields in the public event/map structures;
- no network stack;
- no filesystem parser;
- no shell/command interpreter;
- no privileged instruction except architecture-specific memory-order barriers;
- assembly objects explicitly mark the GNU stack non-executable;
- CI dependency pinned to an immutable full commit SHA;
- checkout credentials not persisted after source retrieval.

The ring is intentionally **not claimed thread-safe or lock-free**. Callers must provide serialization appropriate to their execution context. A memory barrier alone is not synchronization.

## CI / workflow trust boundary

Repository workflows are part of the attack surface. High-value conditions include:

- mutable `uses:` tags or branches;
- direct interpolation of event/pull-request content into executable shell or script bodies;
- long-lived credentials or tokens with more privilege than the step requires;
- force pushes and automatic approval/merge paths;
- mutable runner labels where deterministic execution is required;
- privileged workflow triggers that can consume untrusted code or data.

These conditions must be reviewed separately from the freestanding core because `SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`.

## 0-day boundary

An undisclosed 0-day cannot be proven absent from source inspection. The actionable defensive task is to minimize generic exploitability and compare exposed conditions against **publicly disclosed vulnerability classes and current platform guidance**. Therefore `ZERO_DAY_ABSENCE = TOKEN_VAZIO`; only concrete tested properties may become `PASS`.

These properties reduce attack surface, but they are **design/test properties, not a claim of formal verification or NIST/ISO certification**.
