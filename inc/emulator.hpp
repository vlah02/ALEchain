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

    void print_state();

private:
    bool execute_instruction();
    void store32(uint32_t address, uint32_t value);
    void decode(uint32_t instr, uint8_t& OC, uint8_t& MOD, uint8_t& regA, uint8_t& regB, uint8_t& regC, int16_t& disp);

    uint32_t regs[16];
    uint32_t csr[3];

    std::vector<uint8_t> mem;
};
