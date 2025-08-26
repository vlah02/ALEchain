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

### Emulator Supported Features
- 32-bit virtual CPU with:
    - 16 general-purpose registers (R0–R15)
    - Program Counter (PC)
    - Stack Pointer (SP)
    - Status/Handler/Cause registers
- Memory-mapped I/O:
    - **Terminal output** at `0xFFFFFF00`
    - **Terminal input** at `0xFFFFFF04`
- Timer interrupts with configurable intervals.
- Instruction-by-instruction execution & CPU state printing.

---

### Emulator Workflow

1. **Load Hex File**:
    - Reads linked `.hex` memory image.
2. **Initialize CPU State**:
    - Zeroes registers, sets PC, SP.
3. **Execute Instructions**:
    - Fetch → Decode → Execute cycle.
    - Handles arithmetic, memory ops, branches, interrupts.
4. **Output**:
    - Terminal I/O simulation.
    - Final CPU register dump on `halt`.

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