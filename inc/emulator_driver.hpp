#pragma once
#include <string>
#include <termios.h>
#include <chrono>
#include "emulator_memory.hpp"
#include "emulator_bus.hpp"
#include "emulator_terminal.hpp"

class Emulator {
public:
    Emulator();
    ~Emulator() = default;

    void load_memory(const std::string& hex_filename);
    void run();
    void print_state();

private:
    bool execute_instruction();
    uint32_t load32(uint32_t address);
    void store32(uint32_t address, uint32_t value);
    void poll_terminal_input();

    uint32_t term_in_value = 0;
    uint32_t regs[16]{};
    uint32_t csr[3]{};

    Memory mem;
    Bus bus;

    uint32_t timer_cfg_value = 0;
    std::chrono::steady_clock::time_point timer_last;

    HostTerminal term_guard;

    uint32_t get_timer_period_ms(uint32_t cfg) {
        switch (cfg & 0x7) {
        case 0: return 500;
        case 1: return 1000;
        case 2: return 1500;
        case 3: return 2000;
        case 4: return 5000;
        case 5: return 10000;
        case 6: return 30000;
        case 7: return 60000;
        default: return 500;
        }
    }
};
