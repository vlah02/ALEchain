#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <termios.h>
#include <chrono>

class Emulator {
public:
    Emulator();
    ~Emulator();

    void load_memory(const std::string& hex_filename);
    void run();
    void print_state();

private:
    bool execute_instruction();
    uint32_t load32(uint32_t address);
    void store32(uint32_t address, uint32_t value);
    void poll_terminal_input();

    uint32_t term_in_value = 0;
    uint32_t regs[16];
    uint32_t csr[3];
    std::vector<uint8_t> mem;

    uint32_t timer_cfg_value = 0;
    std::chrono::steady_clock::time_point timer_last;

    termios orig_term;
    void setup_terminal();
    void restore_terminal();
    bool terminal_initialized = false;

    uint32_t get_timer_period_ms(uint32_t cfg);
};
