#pragma once
#include <cstdint>
#include <vector>
#include <string>

class Emulator {
public:
    Emulator();
    void load_memory(const std::string& hex_filename);
    uint32_t load32(uint32_t address);
    void run();

    // Utility: Dump register state
    void print_state();

private:
    void execute_instruction();
    void store32(uint32_t address, uint32_t value);
    void decode(uint32_t instr, uint8_t& OC, uint8_t& MOD, uint8_t& regA, uint8_t& regB, uint8_t& regC, int16_t& disp);

    // Processor state
    uint32_t regs[16];  // r0-r15 (r14 = sp, r15 = pc)
    uint32_t csr[3];    // status, handler, cause

    // 4 GiB memory as a vector of bytes (Level A: do NOT emulate peripherals/term/timer)
    std::vector<uint8_t> mem;
};
