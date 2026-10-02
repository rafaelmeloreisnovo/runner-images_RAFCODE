<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# Threat model and legacy quarantine boundary

## Objective

Preserve the ability to recognize unsafe or obsolete behaviors **without turning this repository into an offensive toolkit**.

Historical/security terms such as sniffer, port mirroring, spoofing, flood, ping-nuke-style denial of service, wireless cracking, fake-phone tooling, botnets and backdoors may appear in documentation only as threat categories or controlled defensive test labels.

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

The generic event/ring/address-map layer is designed around:

- no dynamic allocation;
- caller-owned buffers;
- explicit capacities;
- integer-overflow checks for address ranges;
- no network stack;
- no filesystem parser;
- no shell/command interpreter;
- no privileged instruction except architecture-specific memory-order barriers;
- compile-only CI as the first gate.

These properties reduce attack surface, but they are **design properties, not a claim of formal verification or NIST/ISO certification**.
