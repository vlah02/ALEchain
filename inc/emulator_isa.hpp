#pragma once
#include <cstdint>

enum class OPCODE : uint8_t {
    HALT  = 0x0,
    INT   = 0x1,
    CALL  = 0x2,
    JUMP  = 0x3,
    XCHG  = 0x4,
    ALU   = 0x5,
    LOGIC = 0x6,
    SHIFT = 0x7,
    STORE = 0x8,
    LOAD  = 0x9,
};

// op == CALL
enum class CALL_MOD : uint8_t {
    LINK_TO_REGS_PLUS_IMM        = 0x0, // push pc; pc = gpr[A] + gpr[B] + D
    LINK_TO_MEM_AT_REGS_PLUS_IMM = 0x1, // push pc; pc = mem32[gpr[A] + gpr[B] + D]
};

// op == JUMP
enum class JUMP_MOD : uint8_t {
    ABS            = 0x0, // pc = gpr[A] + D
    BR_EQ          = 0x1, // if (gpr[B] == gpr[C]) pc = gpr[A] + D
    BR_NE          = 0x2, // if (gpr[B] != gpr[C]) pc = gpr[A] + D
    BR_GT_SIGNED   = 0x3, // if ((int)gpr[B] > (int)gpr[C]) pc = gpr[A] + D
    IND            = 0x8, // pc = mem32[gpr[A] + D]
    BR_EQ_IND      = 0x9, // if (gpr[B] == gpr[C]) pc = mem32[gpr[A] + D]
    BR_NE_IND      = 0xA, // if (gpr[B] != gpr[C]) pc = mem32[gpr[A] + D]
    BR_GT_IND_SIGNED = 0xB, // if ((int)gpr[B] > (int)gpr[C]) pc = mem32[gpr[A] + D]
};

// op == ALU
enum class ALU_MOD : uint8_t {
    ADD = 0x0, // gpr[A] = gpr[B] + gpr[C]
    SUB = 0x1, // gpr[A] = gpr[B] - gpr[C]
    MUL = 0x2, // gpr[A] = gpr[B] * gpr[C]
    DIV = 0x3, // gpr[A] = gpr[B] / gpr[C]
};

// op == LOGIC
enum class LOGIC_MOD : uint8_t {
    NOT = 0x0, // gpr[A] = ~gpr[B]
    AND = 0x1, // gpr[A] = gpr[B] & gpr[C]
    OR  = 0x2, // gpr[A] = gpr[B] | gpr[C]
    XOR = 0x3, // gpr[A] = gpr[B] ^ gpr[C]
};

// op == SHIFT
enum class SHIFT_MOD : uint8_t {
    SHL = 0x0, // gpr[A] = gpr[B] << gpr[C] (logical)
    SHR = 0x1, // gpr[A] = gpr[B] >> gpr[C] (logical)
};

// op == STORE
enum class STORE_MOD : uint8_t {
    TO_ADDR_REGS_PLUS_IMM           = 0x0, // mem32[gpr[A] + gpr[B] + D] = gpr[C]
    TO_ADDR_AT_MEM_OF_REGS_PLUS_IMM = 0x2, // mem32[mem32[gpr[A] + gpr[B] + D]] = gpr[C]
    PREINC_AND_STORE                = 0x1, // gpr[A] += D; mem32[gpr[A]] = gpr[C]
};

// op == LOAD
enum class LOAD_MOD : uint8_t {
    CSR_READ                        = 0x0, // gpr[A] = csr[B]
    ADD_IMMEDIATE                   = 0x1, // gpr[A] = gpr[B] + D
    LOAD_FROM_ADDR                  = 0x2, // gpr[A] = mem32[gpr[B] + gpr[C] + D]
    LOAD_FROM_REG_AND_POSTINC       = 0x3, // gpr[A] = mem32[gpr[B]]; gpr[B] += D
    CSR_WRITE_FROM_REG              = 0x4, // csr[A] = gpr[B]
    CSR_OR_IMMEDIATE                = 0x5, // csr[A] = csr[B] | D
    CSR_WRITE_FROM_ADDR             = 0x6, // csr[A] = mem32[gpr[B] + gpr[C] + D]
    CSR_WRITE_FROM_REG_AND_POSTINC  = 0x7, // csr[A] = mem32[gpr[B]]; gpr[B] += D
};
