#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include "../inc/emulator.hpp"

constexpr uint32_t PC_START = 0x40000000;
constexpr uint32_t SP_REG = 14;
constexpr uint32_t PC_REG = 15;

constexpr uint32_t STATUS = 0;
constexpr uint32_t HANDLER = 1;
constexpr uint32_t CAUSE = 2;

#define REG(x) ((x) == 0 ? 0 : regs[x])

Emulator::Emulator() {
    mem.resize(1ULL << 32, 0);
    memset(regs, 0, sizeof(regs));
    memset(csr, 0, sizeof(csr));
    regs[PC_REG] = PC_START;
}

void Emulator::load_memory(const std::string& hex_filename) {
    std::ifstream in(hex_filename);
    if (!in) {
        std::cerr << "Cannot open " << hex_filename << "\n";
        exit(1);
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        uint32_t addr;
        char colon;
        ss >> std::hex >> addr >> colon;
        for (int i = 0; i < 8; ++i) {
            int byte;
            ss >> std::hex >> byte;
            mem[addr + i] = (uint8_t)byte;
        }
    }
    uint32_t base = PC_START;
    std::cout << "First 16 bytes at " << std::hex << base << ":\n";
    for (int i = 0; i < 16; ++i) {
        printf("%02x ", mem[base + i]);
    }
    std::cout << std::endl;
}

uint32_t Emulator::load32(uint32_t address) {
    uint32_t ret = 0;
    for (int i = 0; i < 4; ++i) {
        ret |= (mem[address + i] << (i * 8));
    }
    return ret;
}

void Emulator::store32(uint32_t address, uint32_t value) {
    mem[address + 0] = value & 0xFF;
    mem[address + 1] = (value >> 8) & 0xFF;
    mem[address + 2] = (value >> 16) & 0xFF;
    mem[address + 3] = (value >> 24) & 0xFF;
}

void Emulator::decode(uint32_t pc, uint8_t& op, uint8_t& mod, uint8_t& ra, uint8_t& rb, uint8_t& rc, int16_t& displacement) {
    op  = (mem[pc + 0] & 0xF0) >> 4;
    mod = (mem[pc + 0] & 0x0F);
    ra  = (mem[pc + 1] & 0xF0) >> 4;
    rb  = (mem[pc + 1] & 0x0F);
    rc  = (mem[pc + 2] & 0xF0) >> 4;
    displacement = ((mem[pc + 2] & 0x0F) << 8) | mem[pc + 3];
    if (displacement & 0x800)
        displacement |= 0xF000;
}

bool Emulator::execute_instruction() {
    uint32_t pc = regs[PC_REG];
    uint8_t op, mod, regA, regB, regC;
    int16_t disp;

    op  = (mem[pc + 0] & 0xF0) >> 4;
    mod = (mem[pc + 0] & 0x0F);
    regA = (mem[pc + 1] & 0xF0) >> 4;
    regB = (mem[pc + 1] & 0x0F);
    regC = (mem[pc + 2] & 0xF0) >> 4;
    disp = ((mem[pc + 2] & 0x0F) << 8) | mem[pc + 3];
    if (disp & 0x800) disp |= 0xF000;

    printf("PC=0x%08x op=%x mod=%x a=%x b=%x c=%x disp=%d\n", pc, op, mod, regA, regB, regC, disp);

    if (op != 0)
        regs[PC_REG] += 4;
    regs[0] = 0;

    switch (op) {
    case 0x0:
        return true;
    case 0x1:
        regs[SP_REG] -= 4;
        store32(regs[SP_REG], regs[PC_REG]);
        regs[SP_REG] -= 4;
        store32(regs[SP_REG], csr[STATUS]);
        csr[CAUSE] = 4;
        csr[STATUS] &= ~1U;
        regs[PC_REG] = csr[HANDLER];
        break;
    case 0x2:
        switch (mod) {
        case 0x0:
            regs[SP_REG] -= 4;
            store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = (regA == 0 ? 0 : regs[regA]) + (regB == 0 ? 0 : regs[regB]) + disp;
            break;
        case 0x1:
            regs[SP_REG] -= 4;
            store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = load32((regA == 0 ? 0 : regs[regA]) + (regB == 0 ? 0 : regs[regB]) + disp);
            break;
        }
        break;
    case 0x3:
        switch (mod) {
        case 0x0:
            regs[PC_REG] = (regA == 0 ? 0 : regs[regA]) + disp;
            break;
        case 0x1:
            if ((regB == 0 ? 0 : regs[regB]) == (regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = (regA == 0 ? 0 : regs[regA]) + disp;
            break;
        case 0x2:
            if ((regB == 0 ? 0 : regs[regB]) != (regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = (regA == 0 ? 0 : regs[regA]) + disp;
            break;
        case 0x3:
            if ((int32_t)(regB == 0 ? 0 : regs[regB]) > (int32_t)(regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = (regA == 0 ? 0 : regs[regA]) + disp;
            break;
        case 0x8:
            regs[PC_REG] = load32((regA == 0 ? 0 : regs[regA]) + disp);
            break;
        case 0x9:
            if ((regB == 0 ? 0 : regs[regB]) == (regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = load32((regA == 0 ? 0 : regs[regA]) + disp);
            break;
        case 0xA:
            if ((regB == 0 ? 0 : regs[regB]) != (regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = load32((regA == 0 ? 0 : regs[regA]) + disp);
            break;
        case 0xB:
            if ((int32_t)(regB == 0 ? 0 : regs[regB]) > (int32_t)(regC == 0 ? 0 : regs[regC]))
                regs[PC_REG] = load32((regA == 0 ? 0 : regs[regA]) + disp);
            break;
        }
        break;
    case 0x4:
        {
            uint32_t tmp = (regB == 0 ? 0 : regs[regB]);
            if (regB != 0) regs[regB] = (regC == 0 ? 0 : regs[regC]);
            if (regC != 0) regs[regC] = tmp;
        }
        break;
    case 0x5:
        switch (mod) {
        case 0x0: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) + (regC == 0 ? 0 : regs[regC]); break;
        case 0x1: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) - (regC == 0 ? 0 : regs[regC]); break;
        case 0x2: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) * (regC == 0 ? 0 : regs[regC]); break;
        case 0x3: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) / (regC == 0 ? 0 : regs[regC]); break;
        }
        break;
    case 0x6:
        switch (mod) {
        case 0x0: if (regA != 0) regs[regA] = ~((regB == 0 ? 0 : regs[regB])); break;
        case 0x1: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) & (regC == 0 ? 0 : regs[regC]); break;
        case 0x2: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) | (regC == 0 ? 0 : regs[regC]); break;
        case 0x3: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) ^ (regC == 0 ? 0 : regs[regC]); break;
        }
        break;
    case 0x7:
        switch (mod) {
        case 0x0: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) << (regC == 0 ? 0 : regs[regC]); break;
        case 0x1: if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) >> (regC == 0 ? 0 : regs[regC]); break;
        }
        break;
    case 0x8:
        switch (mod) {
        case 0x0:
            store32((regA == 0 ? 0 : regs[regA]) + (regB == 0 ? 0 : regs[regB]) + disp, (regC == 0 ? 0 : regs[regC]));
            break;
        case 0x2:
            store32(load32((regA == 0 ? 0 : regs[regA]) + (regB == 0 ? 0 : regs[regB]) + disp), (regC == 0 ? 0 : regs[regC]));
            break;
        case 0x1:
            if (regA != 0) regs[regA] = (regs[regA]) + disp;
            store32(regs[regA], (regC == 0 ? 0 : regs[regC]));
            break;
        }
        break;
    case 0x9:
        switch (mod) {
        case 0x0:
            if (regA != 0) regs[regA] = csr[regB];
            break;
        case 0x1:
            if (regA != 0) regs[regA] = (regB == 0 ? 0 : regs[regB]) + disp;
            break;
        case 0x2:
            if (regA != 0) regs[regA] = load32((regB == 0 ? 0 : regs[regB]) + (regC == 0 ? 0 : regs[regC]) + disp);
            break;
        case 0x3:
            if (regA != 0) regs[regA] = load32((regB == 0 ? 0 : regs[regB]));
            if (regB != 0) regs[regB] = (regs[regB]) + disp;
            break;
        case 0x4:
            csr[regA] = (regB == 0 ? 0 : regs[regB]);
            break;
        case 0x5:
            csr[regA] = csr[regB] | disp;
            break;
        case 0x6:
            csr[regA] = load32((regB == 0 ? 0 : regs[regB]) + (regC == 0 ? 0 : regs[regC]) + disp);
            break;
        case 0x7:
            csr[regA] = load32((regB == 0 ? 0 : regs[regB]));
            if (regB != 0) regs[regB] = (regs[regB]) + disp;
            break;
        }
        break;
    }
    regs[0] = 0;

    return false;
}

void Emulator::run() {
    while (true) {
        if (execute_instruction())
            break;
    }
    std::cout << "Emulated processor executed halt instruction\n";
    print_state();
}

void Emulator::print_state() {
    std::cout << "Emulated processor state:\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << " r" << std::dec << i << "=" << "0x"
                  << std::setw(8) << std::setfill('0') << std::hex << regs[i];
        if (i % 4 == 3) std::cout << std::endl;
        else std::cout << "   ";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: emulator <input_hex_file>\n";
        return 1;
    }
    Emulator emu;
    emu.load_memory(argv[1]);
    emu.run();
    return 0;
}
