# ALEchain – Assembler, Linker & Emulator Toolchain

**ALEchain** is a complete educational toolchain for a custom instruction set architecture (ISA), providing:
- **Assembler** – converts assembly source into object files.
- **Linker** – merges object files, resolves symbols, produces executables.
- **Emulator** – runs the executable on a virtual CPU with memory, registers, and interrupts.

The project is built in C++ using **Flex** (lexer) and **Bison** (parser) for the Assembler, and includes a simple Makefile-based build system with tests.

---
<br>

## Table of Contents
1. [Project Overview](#project-overview)
2. [Assembler](#assembler)
    - [Build & Run](#build--run)
    - [Architecture Overview](#architecture-overview)
    - [Expressions & Literals](#expressions--literals)
    - [Supported Instructions](#supported-instructions)
    - [Directives](#directives)
    - [Relocations](#relocations)
    - [Output File Format (textual)](#output-file-format-textual)
    - [Example](#example)
    - [Current Limitations / Notes](#current-limitations--notes)
    - [Extending](#extending)
3. [Linker](#linker)
    - [Supported Features](#supported-features)
    - [Linker Workflow](#linker-workflow)
4. [Emulator](#emulator)
    - [Quick Start](#quick-start)
    - [High-Level Design](#high-level-design)
    - [Components](#components)
    - [CPU Model](#cpu-model)
    - [Memory Map & MMIO](#memory-map--mmio)
    - [Instruction Set (Overview)](#instruction-set-overview)
    - [Interrupts](#interrupts)
    - [Hex Image Format](#hex-image-format)
5. [Build Instructions](#build-instructions)
6. [Required Tools](#required-tools)


---
<br>

## Project Overview

**ALEchain** simulates the typical low-level program pipeline:

Assembly source (.s)  
↓  
Assembler → Object file (.o)  
↓  
Linker → Linked binary (.hex)  
↓  
Emulator → Execution on virtual CPU

- **Assembler**: Parses assembly files, resolves symbols, creates relocatable object files.
- **Linker**: Merges multiple object files, handles relocations, produces final memory image.
- **Emulator**: Loads and executes the program on a virtual CPU, supporting I/O and timers.

---
<br>

## Assembler

A small, relocatable assembler built with **Flex** + **Bison**.  
It parses a custom ISA, builds sections, symbols, and relocations, and writes a simple text `.o` format with three blocks:

- `.sections` — raw bytes of each section
- `.symbols` — symbol table with bindings, types, and values
- `.relocations` — relocation records (currently `R_ABS32`)

The assembler is organized around a single `Assembler` driver that owns the four core subsystems:

- `Sections`  — owns `Section` objects and emission order
- `Symbols`   — symbol table (defs/occurrences, globals, weaks, types)
- `Relocations` — relocation list
- `Pool`      — literal/symbol pool, `.equ` handling, and patching during `flush()`

`parser.y` references these through `Assembler::instance()`.

---

### Build & Run

    # Build (uses flex+bison)
    make

    # Assemble a single file
    ./assembler input.s

    # or with explicit output:
    ./assembler -o out.o input.s

The output file is a human-readable object file with `.sections`, `.symbols`, and `.relocations` blocks.

---

### Architecture Overview

1. **Lexing** (`lexer.l`)
    - Tokenizes mnemonics, registers (`%r0..%r15`, `%sp`, `%pc`), CSRs (`%status`, `%handler`, `%cause`),
      integers (dec/hex/bin/oct), strings, and symbols.

2. **Parsing** (`parser.y`)
    - Validates instruction/directive forms and emits bytes into the current `Section`.
    - For immediates and symbol references:
        - Plain integers are emitted directly.
        - `SYMBOL[±INTEGER]` are sent to the **Pool**.

3. **Pooling & Patching** (`Pool`)
    - `Pool::add_literal` parses `SYMBOL ± imm` and decides:
        - If the base is an `.equ`, enqueue an **immediate** value (optionally patching the instruction in place).
        - Otherwise enqueue a **symbol** (will add a relocation).
    - `Pool::flush(Sections, Symbols, Relocations)`:
        - Resolves pending `.equ` expressions (`A ± B`).
        - For each pooled item:
            - Emits the 32-bit word into the owning section.
            - Patches the referring instruction’s displacement field (pool slot is PC-relative).
            - If the item is a symbol, adds a relocation (`R_ABS32`) at the pool word offset.

4. **Object Emission**
    - `Sections::out(std::ostream&)` prints all section bytes.
    - `Symbols::out(std::ostream&)` prints symbol table rows (binding, weak/strong, type, value/section).
    - `Relocations::out(std::ostream&)` prints relocation records.

---

### Expressions & Literals

- **Integers**: decimal (`123`), hex (`0x7B`), octal (`0o173`), binary (`0b1111011`), negative numbers (`-1`).
- **Symbols**: `name`, and additive expressions `name+N` / `name-N` where `N` is an integer.
- **.equ**:
    - Immediate: `.equ SIZE, 256`
    - Deferred: `.equ DIFF, end - start` or `.equ SUM, A + B`
    - Deferred `.equ` are resolved at `Pool::flush()` time. If a referenced symbol is undefined at flush, an error is reported.

---

### Supported Instructions

All mnemonics are lowercase in the source. Registers are `%r0..%r15`, plus aliases `%sp` (r14) and `%pc` (r15).  
CSRs supported: `%status`, `%handler`, `%cause`.

> Notes:
> - Some shift/extended `add` forms are tokenized but not yet emitted (placeholders in grammar).
> - Branch/jump targets accept both absolute immediates and `SYMBOL±IMM`. The pool handles relocation and patching.

| Instruction | Form(s)                               | Example                          | Description                                  |
|-------------|----------------------------------------|-----------------------------------|----------------------------------------------|
| `halt`      | `halt`                                 | `halt`                            | Stop program                                 |
| `int`       | `int`                                  | `int`                             | Software interrupt                           |
| `iret`      | `iret`                                 | `iret`                            | Return from interrupt                        |
| `call`      | `call SYMBOL` \| `call INTEGER`        | `call func` \| `call 0x4000`      | Call                                         |
| `ret`       | `ret`                                  | `ret`                             | Return                                       |
| `jmp`       | `jmp SYMBOL` \| `jmp INTEGER` \| `jmp SYMBOL ± INTEGER` | `jmp label+4`                  | Unconditional jump                           |
| `beq`       | `beq rX, rY, SYMBOL/INTEGER`           | `beq %r1, %r2, target`            | Branch if equal                              |
| `bne`       | `bne rX, rY, SYMBOL/INTEGER`           | `bne %r1, %r2, 0x2000`            | Branch if not equal                          |
| `bgt`       | `bgt rX, rY, SYMBOL/INTEGER`           | `bgt %r1, %r2, label`             | Branch if greater                            |
| `push`      | `push rX` \| `push {rX, rY, ...}`      | `push %r1` \| `push { %r1, %r2 }` | Push register(s)                             |
| `pop`       | `pop rX`                               | `pop %r1`                         | Pop register                                 |
| `xchg`      | `xchg rX, rY`                          | `xchg %r1, %r2`                   | Exchange registers                           |
| `add`       | `add rX, rY`                           | `add %r1, %r2`                    | Add registers                                |
| `sub`       | `sub rX, rY`                           | `sub %r1, %r2`                    | Subtract registers                           |
| `mul`       | `mul rX, rY`                           | `mul %r1, %r2`                    | Multiply registers                           |
| `div`       | `div rX, rY`                           | `div %r1, %r2`                    | Divide registers                             |
| `not`       | `not rX`                               | `not %r1`                         | Bitwise NOT                                  |
| `and`       | `and rX, rY`                           | `and %r1, %r2`                    | Bitwise AND                                  |
| `or`        | `or rX, rY`                            | `or %r1, %r2`                     | Bitwise OR                                   |
| `xor`       | `xor rX, rY`                           | `xor %r1, %r2`                    | Bitwise XOR                                  |
| `shl`       | `shl rX, rY`                           | `shl %r1, %r2`                    | Shift left                                   |
| `shr`       | `shr rX, rY`                           | `shr %r1, %r2`                    | Shift right                                  |
| `ld`        | `ld $INTEGER, rX`                      | `ld $0x1000, %r1`                 | Load absolute (immediate address)            |
|             | `ld $SYMBOL, rX`                       | `ld $var, %r1`                    | Load absolute (symbol, reloc if needed)      |
|             | `ld INTEGER, rX`                       | `ld 4, %r1`                       | Load via stack scratch sequence (emits pool) |
|             | `ld SYMBOL, rX`                        | `ld var, %r1`                     | Load via stack scratch + pool/reloc          |
|             | `ld [rY], rX`                          | `ld [%r2], %r1`                   | Load from register address                   |
|             | `ld [rY + INTEGER], rX`                | `ld [%r2 + 8], %r1`               | Load with displacement                       |
|             | `ld [rY + SYMBOL], rX`                 | `ld [%r2 + var], %r1`             | Load with symbolic displacement (occurrence) |
|             | `ld [rY + rZ], rX`                     | `ld [%r2 + %r3], %r1`             | Load indexed                                 |
| `st`        | `st rX, INTEGER`                       | `st %r1, 0x2000`                  | Store to absolute address (immediate)        |
|             | `st rX, SYMBOL`                        | `st %r1, var`                     | Store to absolute address (symbol/reloc)     |
|             | `st rX, rY`                            | `st %r1, %r2`                      | Store to address in register                 |
|             | `st rX, [rY]`                          | `st %r1, [%r2]`                   | Store via [base]                             |
|             | `st rX, [rY + INTEGER]`                | `st %r1, [%r2 + 4]`               | Store via [base + disp]                      |
|             | `st rX, [rY + SYMBOL]`                 | `st %r1, [%r2 + var]`             | Store via [base + symbol] (pool/reloc)       |
|             | `st rX, [rY + rZ]`                      | `st %r1, [%r2 + %r3]`             | Store via [base + index]                     |
| `csrrd`     | `csrrd %status|%handler|%cause, rX`    | `csrrd %status, %r1`              | CSR read                                     |
| `csrwr`     | `csrwr rX, %status|%handler|%cause`    | `csrwr %r1, %cause`               | CSR write                                    |

---

### Directives

| Directive  | Format / Usage                                | Example                       | Description                                                       |
|------------|-----------------------------------------------|-------------------------------|-------------------------------------------------------------------|
| `.global`  | `.global SYMBOL[, SYMBOL...]`                 | `.global main`                | Mark symbol(s) as global (binding = global)                       |
| `.extern`  | `.extern SYMBOL[, SYMBOL...]`                 | `.extern ext_fn`              | **Currently tokenized as `.global` and treated the same**         |
| `.section` | `.section NAME`                               | `.section txt`                | Switch/create section                                             |
| `.word`    | `.word VALUE \| SYMBOL \| SYMBOL±IMM, ...`    | `.word 10, var, label+4`      | Emit 32-bit words; symbols may create pool entries/relocations    |
| `.skip`    | `.skip N`                                     | `.skip 16`                    | Reserve N zero bytes                                              |
| `.ascii`   | `.ascii "string"`                             | `.ascii "Hello"`              | Emit raw bytes (no NUL)                                           |
| `.equ`     | `.equ NAME, IMM` \| `.equ D, A − B` \| `.equ S, A + B` | `.equ SZ, 256`      | Define absolute symbols; deferred forms resolved at flush         |
| `.type`    | `.type SYMBOL, FUNC|DATA|NOTYPE`              | `.type main, FUNC`            | Symbol type metadata                                              |
| `.weak`    | `.weak SYMBOL[, SYMBOL...]`                   | `.weak optional_handler`      | Mark symbol(s) as weak                                            |
| `.end`     | `.end`                                        | `.end`                        | End of source                                                     |

> **Notes**:  
> - `.word SYMBOL±IMM`: if `SYMBOL` is a known `.equ`, the immediate value is emitted; otherwise a pool entry + relocation is created.
> - `.weak`: validated at flush — it must be either defined or referenced in the module.

---

### Relocations

- Type: `R_ABS32`
- Created for any pooled **symbol** (non-`.equ`) when its 32-bit word is emitted into a section.
- Offset points at the pool word (not the instruction).
- Addend is the parsed `± IMM` part.

Example output:

    .relocations
    txt 64 R_ABS32 func 0

---

### Output File Format (textual)

    .sections
    .txt
    00000000 00000000 00000000 00000000
    ...

    .symbols
    main defined strong FUNC txt 0 0
    var  local   strong NOTYPE data 4 0
    SIZE defined strong NOTYPE ABS 256 0
    ...

    .relocations
    txt 64 R_ABS32 func 0

---

## Example

    .section txt
    .global main
    .type main, FUNC

    main:
        ld  $msg, %r1
        call puts
        halt

    .section data
    msg:
        .ascii "Hello"
        .word 0

Assembling:

    ./assembler -o hello.o hello.s

You’ll see `.sections` with the encoded instructions and data, `.symbols` with `main`, `msg`, etc., and `.relocations` with a record for `puts`.

---
<br>

## Linker

### Supported Features
- Linking multiple `.o` files into one final **executable image**.
- Handling **global** and **weak** symbols.
- Support for **relocations**:
    - Resolving symbol addresses
    - Patching forward references
- Section placement with `-place=.section@0xADDR`.
- Produces either:
    - **Hex output** for emulator
    - **Relocatable output** for further linking

---

### Linker Workflow

1. **Load Sections & Symbols**:
    - Reads `.o` files, extracts section data, symbol tables, relocations.
2. **Merge Sections**:
    - Combines sections of same name.
    - Resolves base addresses (explicit or sequential).
3. **Resolve Symbols**:
    - Ensures no multiple definitions unless weak.
    - Applies relocations for all symbol references.
4. **Generate Output**:
    - `-hex` → Final memory image for emulator.
    - `-relocatable` → Relocatable object for further linking.

---
<br>

## Emulator
A tiny 32-bit CPU emulator with a clean Bus / Device architecture, a memory-mapped terminal, and a timer that raises interrupts. It loads a simple hex image, runs until HALT, and prints the final CPU state.

---

### Quick start
#### Build
```bash
g++ -o emulator src/emulator_driver.cpp
```
Or use your existing `Makefile`

#### Run
```bash
./emulator path/to/program.hex
```
Your host terminal is switched to raw, non-blocking mode while the emulator runs (restored on exit).

---

### High-level design
```
+------------------+       +--------------------------+
|   Emulator       |       |   Memory (paged)         |
|  - regs[16]      |       |  - 4 KiB pages           |
|  - csr[3]        |<----->|  - load8/32, store8/32   |
|  - Bus           |       +--------------------------+
|  - HostTerminal  |
+--------^---------+
         |
         v
+--------------------------------------------------------+
| Bus                                                    |
| - Owns vector<unique_ptr<Device>> (sorted by base)     |
| - map() checks overlap            read32/write32/tick  |
+--------------------------------------------------------+
   |                       |
   | MMIO                  | MMIO
   v                       v
+----------------+     +----------------+
| TerminalDevice |     | TimerDevice    |
| base=0xFFFFFF00|     | base=0xFFFFFF10|
| size=8         |     | size=4         |
| TX/RX regs     |     | CFG reg        |
| raises CAUSE=3 |     | raises CAUSE=2 |
+----------------+     +----------------+
```

### Components
- **Memory** — paged backing store with `load8/32` & `store8/32`.
- **Bus** — routes 32-bit reads/writes to RAM or mapped **Devices**.
  - Rejects overlapping devices, keeps them sorted by base.
  - Calls `Device::tick()` each step (`bus.tick()`).
- **Device** (abstract) — base class with `read32`, `write32`, `tick`, `contains`.
- **TerminalDevice** — simple UART-like MMIO:
  - `TX` (offset `0x0`) write-only: writes one byte to host `stdout`.
  - `RX` (offset `0x4`) read-only: returns last byte read from host (clears on read).
  - Polls host each `tick()`; if a byte was read, raises interrupt (`CAUSE=3`).
- **TimerDevice** — periodic timer:
  - Single 32-bit CFG register at base `0xFFFFFF10`:
    - `CFG & 0x7` selects the period (see table below).
    - On write: stores `CFG` and resets timer reference.
  - Each `tick()` checks elapsed time and, if period elapsed, raises interrupt (`CAUSE=2`).
- **HostTerminal** — RAII guard that puts your terminal into raw, non-blocking mode.
Constructed once in `Emulator`, restored on destruction.

---

### CPU model
- **32-bit** registers; **16 GPRs** (`r0..r15`):
  - `r0` is **hard-wired to 0** after every instruction.
  - `r14` is **SP** (stack pointer).
  - `r15` is **PC** (program counter).
- **CSRs** (indices used by instructions):
  - `csr[0]` = **STATUS**
    - bit 0: **Timer IRQ mask** (1 = masked)
    - bit 1: **Terminal IRQ mask** (1 = masked)
    - bit 2: **In-ISR flag** (set when hardware IRQ vectors)
  - `csr[1]` = **HANDLER** (interrupt vector)
  - `csr[2]` = **CAUSE**
    - `2` = timer, `3` = terminal, `4` = software INT
- **Fetch/Decode/Execute**:
  - Each step: `bus.tick()` → fetch 4 bytes at `PC` → decode → execute.
  - For non-HALT instructions, `PC += 4` *before* execution effects that jump/branch.
  - On device IRQs, emulator checks masks and `STATUS` bit 2; if allowed, pushes `PC` and `STATUS`, sets `STATUS.bit2`, and jumps to `HANDLER`.

---

### Memory map & MMIO
> All addresses not claimed by a mapped device are served by RAM.

**Devices**:

  | Device         | Base         | Size | Off | Access | Meaning                                    |
  |----------------| ------------ | ---- | --- | ------ | ------------------------------------------ |
  | TerminalDevice | `0xFFFFFF00` | 8    | 0x0 | W32    | TX: lower 8 bits written to host stdout    |
  |                |              |      | 0x4 | R32    | RX: last byte from host (clears on read)   |
  | TimerDevice    | `0xFFFFFF10` | 4    | 0x0 | R/W32  | Timer config (period select in low 3 bits) |

**Timer periods** (`CFG & 0x7`):

| Value | Period (ms) |
| ----: | ----------: |
|     0 |         500 |
|     1 |        1000 |
|     2 |        1500 |
|     3 |        2000 |
|     4 |        5000 |
|     5 |       10000 |
|     6 |       30000 |
|     7 |       60000 |

---

### Instruction set (overview)
The top nibble of byte 0 is the **opcode**; the low nibble is a **modifier** (for grouped instructions). Register fields `A/B/C` and 12-bit signed displacement `D` are in the remaining bytes.  
<br>

#### Opcodes
| Mnemonic group | Opcode | Description                                        |
| -------------- | :----: | -------------------------------------------------- |
| `HALT`         |   0x0  | Stop execution                                     |
| `INT`          |   0x1  | Software interrupt (CAUSE ← 4; push; jump HANDLER) |
| `CALL`         |   0x2  | Call subroutine (see `CALL_MOD`)                   |
| `JUMP`         |   0x3  | Branch/Jump (see `JUMP_MOD`)                       |
| `XCHG`         |   0x4  | Atomic swap `rB` ↔ `rC`                            |
| `ALU`          |   0x5  | Add/Sub/Mul/Div (see `ALU_MOD`)                    |
| `LOGIC`        |   0x6  | Not/And/Or/Xor (see `LOGIC_MOD`)                   |
| `SHIFT`        |   0x7  | Logical shifts (see `SHIFT_MOD`)                   |
| `STORE`        |   0x8  | Store to memory (see `STORE_MOD`)                  |
| `LOAD`         |   0x9  | Loads & CSR ops (see `LOAD_MOD`)                   |
<br>

#### CALL
| Modifier                       | Value | Semantics                          |
| ------------------------------ | :---: | ---------------------------------- |
| `LINK_TO_REGS_PLUS_IMM`        |  0x0  | `push pc; pc = rA + rB + D`        |
| `LINK_TO_MEM_AT_REGS_PLUS_IMM` |  0x1  | `push pc; pc = mem32[rA + rB + D]` |
<br>

#### JUMP
| Modifier           | Value | Semantics                                   |
| ------------------ | :---: | ------------------------------------------- |
| `ABS`              |  0x0  | `pc = rA + D`                               |
| `BR_EQ`            |  0x1  | `if (rB == rC) pc = rA + D`                 |
| `BR_NE`            |  0x2  | `if (rB != rC) pc = rA + D`                 |
| `BR_GT_SIGNED`     |  0x3  | `if ((int)rB > (int)rC) pc = rA + D`        |
| `IND`              |  0x8  | `pc = mem32[rA + D]`                        |
| `BR_EQ_IND`        |  0x9  | `if (rB == rC) pc = mem32[rA + D]`          |
| `BR_NE_IND`        |  0xA  | `if (rB != rC) pc = mem32[rA + D]`          |
| `BR_GT_IND_SIGNED` |  0xB  | `if ((int)rB > (int)rC) pc = mem32[rA + D]` |
<br>

#### ALU
| Modifier | Value | Semantics                  |
| -------- | :---: | -------------------------- |
| `ADD`    |  0x0  | `rA = rB + rC` (if `A!=0`) |
| `SUB`    |  0x1  | `rA = rB - rC` (if `A!=0`) |
| `MUL`    |  0x2  | `rA = rB * rC` (if `A!=0`) |
| `DIV`    |  0x3  | `rA = rB / rC` (if `A!=0`) |
<br>

#### LOGIC
| Modifier | Value | Semantics                  |
| -------- | :---: |----------------------------|
| `NOT`    |  0x0  | `rA = ~rB` (if `A!=0`)     |
| `AND`    |  0x1  | `rA = rB & rC` (if `A!=0`) |
| `OR`     |  0x2  | `rA = rB \| rC` (if`A!=0`) |
| `XOR`    |  0x3  | `rA = rB ^ rC` (if `A!=0`) |
<br>

#### SHIFT
| Modifier | Value | Semantics                             |
| -------- | :---: | ------------------------------------- |
| `SHL`    |  0x0  | `rA = rB << rC` (logical) (if `A!=0`) |
| `SHR`    |  0x1  | `rA = rB >> rC` (logical) (if `A!=0`) |
<br>

#### STORE
| Modifier                          | Value | Semantics                        |
| --------------------------------- | :---: | -------------------------------- |
| `TO_ADDR_REGS_PLUS_IMM`           |  0x0  | `mem32[rA + rB + D] = rC`        |
| `TO_ADDR_AT_MEM_OF_REGS_PLUS_IMM` |  0x2  | `mem32[mem32[rA + rB + D]] = rC` |
| `PREINC_AND_STORE`                |  0x1  | `rA += D; mem32[rA] = rC`        |
<br>

#### LOAD
| Modifier                         | Value | Semantics                     |
| -------------------------------- | :---: |-------------------------------|
| `CSR_READ`                       |  0x0  | `rA = csr[B]`                 |
| `ADD_IMMEDIATE`                  |  0x1  | `rA = rB + D`                 |
| `LOAD_FROM_ADDR`                 |  0x2  | `rA = mem32[rB + rC + D]`     |
| `LOAD_FROM_REG_AND_POSTINC`      |  0x3  | `rA = mem32[rB]; rB += D`     |
| `CSR_WRITE_FROM_REG`             |  0x4  | `csr[A] = rB`                 |
| `CSR_OR_IMMEDIATE`               |  0x5  | `csr[A] = csr[B] \| D`        |
| `CSR_WRITE_FROM_ADDR`            |  0x6  | `csr[A] = mem32[rB + rC + D]` |
| `CSR_WRITE_FROM_REG_AND_POSTINC` |  0x7  | `csr[A] = mem32[rB]; rB += D` |
> Note: `r0` is forced to 0 after each instruction; writes to `r0` are ignored via that rule.

---

### Interrupts
- **Hardware IRQs**:
  - Timer → `CAUSE=2`
  - Terminal (on host keystroke) → `CAUSE=3`  
    If `(STATUS.bit2 == 0)` (not already in ISR) and the IRQ is not masked (`STATUS.bit0` masks timer, `STATUS.bit1` masks terminal), the emulator:
  1. pushes `PC`, pushes `STATUS`,
  2. sets `STATUS.bit2` (in-ISR),
  3. jumps to `HANDLER`.
- **Software INT (opcode 0x1)**:
  - Sets `CAUSE=4`, pushes `PC` & `STATUS`, **clears** `STATUS.bit0`, and jumps to `HANDLER`.

---

### Hex image format
The loader expects a simple hex dump: each line begins with a hex address followed by a colon and **eight** byte values (hex), e.g.:
```
40000000: 13 00 00 00  00 00 00 00
40000008: AA BB CC DD  11 22 33 44
```
Bytes are written in memory at the given address (little-endian is handled by how your code packs/reads words).

---
<br>

## Build Instructions

Project uses a simple **Makefile**:

```bash
# Build everything (assembler, linker, emulator)
make all

# Clean build artifacts
make clean
```

---
<br>

## Required tools
- g++ (C++17)  
- flex, bison  
- make