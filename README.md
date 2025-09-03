# ALEchain – Assembler, Linker & Emulator Toolchain

**ALEchain** is a complete educational toolchain for a custom instruction set architecture (ISA), providing:
- **Assembler** – converts assembly source into object files.
- **Linker** – merges object files, resolves symbols, produces executables.
- **Emulator** – runs the executable on a virtual CPU with memory, registers, and interrupts.

The project is built in C++ using **Flex** (lexer) and **Bison** (parser) for the Assembler, and includes a simple Makefile-based build system with tests.

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Assembler](#assembler)
    - [Supported Instructions](#supported-instructions)
    - [Assembler Workflow](#assembler-workflow)
3. [Linker](#linker)
    - [Supported Features](#supported-features)
    - [Linker Workflow](#linker-workflow)
4. [Emulator](#emulator)
    - [Supported Features](#emulator-supported-features)
    - [Emulator Workflow](#emulator-workflow)
5. [Build Instructions](#build-instructions)
6. [Required tools](#required-tools)

---

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

## Assembler

### Supported Instructions
The assembler parser (using **Flex** + **Bison**) supports the following instructions:


### Supported Instructions

| Instruction | Supported Format            | Example                  | Description                                     |
|--|-----------------------------|---------------------------|------------------------------------------------|
| `halt` | `halt`                      | `halt`                    | Stop program execution                          |
| `int` | `int`                       | `int`                     | Software interrupt                              |
| `iret` | `iret`                      | `iret`                    | Return from interrupt                           |
| `call` | `call SYMBOL`               | `call func`               | Function call                                   |
|  | `call INTEGER`              | `call 0x4000`             | Function call with immediate address            |
| `ret` | `ret`                       | `ret`                     | Return from function                            |
| `jmp` | `jmp SYMBOL`                | `jmp label`               | Unconditional jump                              |
|  | `jmp INTEGER`               | `jmp 0x1000`              | Jump to immediate address                       |
|  | `jmp SYMBOL ± INTEGER`      | `jmp label+4`             | Jump with displacement                          |
| `beq` | `beq rX, rY, SYMBOL`        | `beq r1, r2, label`        | Branch if equal                                 |
|  | `beq rX, rY, INTEGER`       | `beq r1, r2, 0x2000`       | Branch if equal to immediate address            |
| `bne` | `bne rX, rY, SYMBOL`        | `bne r1, r2, label`        | Branch if not equal                             |
|  | `bne rX, rY, INTEGER`       | `bne r1, r2, 0x2000`       | Branch if not equal to immediate address        |
| `bgt` | `bgt rX, rY, SYMBOL`        | `bgt r1, r2, label`        | Branch if greater                               |
|  | `bgt rX, rY, INTEGER`       | `bgt r1, r2, 0x2000`       | Branch if greater to immediate address          |
| `push` | `push rX`                   | `push r1`                  | Push single register                            |
|  | `push {rX, rY}`             | `push {r1, r2}`            | Push multiple registers                         |
| `pop` | `pop rX`                    | `pop r1`                   | Pop register from stack                         |
| `xchg` | `xchg rX, rY`               | `xchg r1, r2`              | Exchange registers                              |
| `add` | `add rX, rY`                | `add r1, r2`               | Add two registers                               |
| `sub` | `sub rX, rY`                | `sub r1, r2`               | Subtract registers                              |
| `mul` | `mul rX, rY`                | `mul r1, r2`               | Multiply registers                              |
| `div` | `div rX, rY`                | `div r1, r2`               | Divide registers                                |
| `not` | `not rX`                    | `not r1`                   | Bitwise NOT                                     |
| `and` | `and rX, rY`                | `and r1, r2`               | Bitwise AND                                     |
| `or` | `or rX, rY`                 | `or r1, r2`                | Bitwise OR                                      |
| `xor` | `xor rX, rY`                | `xor r1, r2`               | Bitwise XOR                                     |
| `shl` | `shl rX, rY`                | `shl r1, r2`               | Shift left                                      |
| `shr` | `shr rX, rY`                | `shr r1, r2`               | Shift right                                     |
| `ld` | `ld $SYMBOL, rX`            | `ld $0x1000, r1`           | Load from absolute address                      |
|  | `ld SYMBOL, rX`             | `ld var, r1`               | Load from symbol address                        |
|  | `ld [rX], rY`               | `ld [r1], r2`              | Load from address in register                   |
| `st` | `st rX, SYMBOL`             | `st r1, var`               | Store register to symbol                        |
|  | `st rX, [rY]`               | `st r1, [r2]`              | Store register to address in register           |
| `csrrd` | `csrrd %status, rX`         | `csrrd %status, r1`        | Read control/status register into register      |
| `csrwr` | `csrwr rX, %cause`          | `csrwr r1, %cause`         | Write register value into control/status reg    |

### Directives Supported

| Directive     | Format / Usage                     | Example               | Description                                          |
|----------------|------------------------------------|-----------------------|------------------------------------------------------|
| `.global`      | `.global SYMBOL[, SYMBOL...]`      | `.global main`        | Marks at least 1 symbol as global for linker         |
| `.extern`      | `.extern SYMBOL[, SYMBOL...]`      | `.extern ext_func`    | Marks at least 1 symbol as defined in another module |
| `.section`     | `.section NAME`                    | `.section text`       | Starts a new section (`.text`, `.data`, etc.)        |
| `.word`        | `.word VALUE[, VALUE...]`          | `.word 10, 20, 30`    | Stores one or more 32-bit words                      |
| `.skip`        | `.skip N`                          | `.skip 16`            | Reserves N bytes initialized to 0                    |
| `.ascii`       | `.ascii "string"`                  | `.ascii "Hello"`      | Stores ASCII string without null terminator          |
| `.equ`         | `.equ SYMBOL, VALUE`               | `.equ SIZE, 256`      | Defines constant symbol with value                   |
| `.end`         | `.end`                             | `.end`                | Marks end of source file                             |
| `.type`        | `.type SYMBOL, FUNC\|DATA\|NOTYPE` | `.type main, FUNC`    | Assigns symbol type for linker                       |
| `.weak`        | `.weak SYMBOL1, SYMBOL2`           | `.weak optional_func` | Marks symbol as weakly defined                       |

---

### Assembler Workflow

1. **Lexical Analysis** (`lexer.l`):
    - Converts source code into tokens (instructions, registers, literals, symbols).
2. **Parsing** (`parser.y`):
    - Validates instruction syntax, directives, labels, symbols.
    - Builds internal representation of sections & symbol tables.
3. **Object File Generation**:
    - Produces `.o` file with:
        - Sections
        - Symbol table
        - Relocation info (for linker)
4. **Literals & Patching**:
    - Forward references handled with **literal pools** & patched after parsing.

**Output:** Relocatable `.o` file with text/data sections and symbol table.

---

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

## Emulator
A tiny 32-bit CPU emulator with a clean Bus / Device architecture, a memory-mapped terminal, and a timer that raises interrupts. It loads a simple hex image, runs until HALT, and prints the final CPU state.

---

### Quick start
#### Build
```bash
g++ -std=c++17 -O2 -Wall -Wextra \
  src/emulator_driver.cpp \
  -Isrc -o emulator
```
Or use you existing `Makefile`

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
  - For non-HALT instructions, `PC += 4` before execution effects that jump/branch.
  - On device IRQs, emulator checks masks and `STATUS` bit 2; if allowed, pushes `PC` and `STATUS`, sets `STATUS.bit2`, and jumps to `HANDLER`.

---

### Memory map & MMIO
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

### Opcodes
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

### CALL
| Modifier                       | Value | Semantics                          |
| ------------------------------ | :---: | ---------------------------------- |
| `LINK_TO_REGS_PLUS_IMM`        |  0x0  | `push pc; pc = rA + rB + D`        |
| `LINK_TO_MEM_AT_REGS_PLUS_IMM` |  0x1  | `push pc; pc = mem32[rA + rB + D]` |

### JUMP
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

### ALU
| Modifier | Value | Semantics                  |
| -------- | :---: | -------------------------- |
| `ADD`    |  0x0  | `rA = rB + rC` (if `A!=0`) |
| `SUB`    |  0x1  | `rA = rB - rC` (if `A!=0`) |
| `MUL`    |  0x2  | `rA = rB * rC` (if `A!=0`) |
| `DIV`    |  0x3  | `rA = rB / rC` (if `A!=0`) |

### LOGIC
| Modifier | Value | Semantics                  |
| -------- | :---: |----------------------------|
| `NOT`    |  0x0  | `rA = ~rB` (if `A!=0`)     |
| `AND`    |  0x1  | `rA = rB & rC` (if `A!=0`) |
| `OR`     |  0x2  | `rA = rB \| rC` (if`A!=0`) |
| `XOR`    |  0x3  | `rA = rB ^ rC` (if `A!=0`) |

### SHIFT
| Modifier | Value | Semantics                             |
| -------- | :---: | ------------------------------------- |
| `SHL`    |  0x0  | `rA = rB << rC` (logical) (if `A!=0`) |
| `SHR`    |  0x1  | `rA = rB >> rC` (logical) (if `A!=0`) |

### STORE
| Modifier                          | Value | Semantics                        |
| --------------------------------- | :---: | -------------------------------- |
| `TO_ADDR_REGS_PLUS_IMM`           |  0x0  | `mem32[rA + rB + D] = rC`        |
| `TO_ADDR_AT_MEM_OF_REGS_PLUS_IMM` |  0x2  | `mem32[mem32[rA + rB + D]] = rC` |
| `PREINC_AND_STORE`                |  0x1  | `rA += D; mem32[rA] = rC`        |

### LOAD
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

## Build Instructions

Project uses a simple **Makefile**:

```bash
# Build everything (assembler, linker, emulator)
make all

# Clean build artifacts
make clean
```

---

## Required tools
- g++ (C++17)  
- flex, bison  
- make