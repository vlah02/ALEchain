#pragma once
#include <string>
#include "emulator_memory.hpp"
#include "emulator_bus.hpp"
#include "emulator_termios.hpp"

class Emulator {
public:
    Emulator();
    ~Emulator() = default;

    void load_memory(const std::string& hex_filename) const;
    void run();

private:
    uint32_t load32(uint32_t address) const {
        return bus.read32(address);
    }
    void store32(uint32_t address, uint32_t value) const {
        bus.write32(address, value);
    }
    bool execute_instruction();

    uint32_t regs[16]{};
    uint32_t csr[3]{};

    Memory mem;
    Bus bus;

    HostTerminal termios;
};
