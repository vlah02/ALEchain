#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include "../inc/emulator.hpp"

constexpr uint32_t PC_START = 0x40000000;
constexpr uint32_t SP_REG = 14;
constexpr uint32_t PC_REG = 15;
constexpr uint32_t STATUS = 0;
constexpr uint32_t HANDLER = 1;
constexpr uint32_t CAUSE = 2;

constexpr uint32_t TERM_OUT = 0xFFFFFF00;
constexpr uint32_t TERM_IN  = 0xFFFFFF04;
constexpr uint32_t TIM_CFG  = 0xFFFFFF10;

#define REG(x) ((x) == 0 ? 0 : regs[x])

uint32_t Emulator::get_timer_period_ms(uint32_t cfg) {
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

Emulator::Emulator() {
    mem.resize(1ULL << 32, 0);
    memset(regs, 0, sizeof(regs));
    memset(csr, 0, sizeof(csr));
    regs[PC_REG] = PC_START;
    setup_terminal();

    timer_cfg_value = 0;
    timer_last = std::chrono::steady_clock::now();
}

Emulator::~Emulator() {
    restore_terminal();
}

void Emulator::setup_terminal() {
    if (terminal_initialized) return;
    tcgetattr(STDIN_FILENO, &orig_term);
    struct termios new_term = orig_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    terminal_initialized = true;
}

void Emulator::restore_terminal() {
    if (terminal_initialized)
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
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
}

void Emulator::poll_terminal_input() {
    int c = getchar();
    if (c != EOF) {
        term_in_value = (uint8_t)c;
        csr[CAUSE] = 3;
    }
}

uint32_t Emulator::load32(uint32_t address) {
    if (address == TERM_IN) {
        uint32_t value = term_in_value;
        term_in_value = 0;
        return value;
    }
    if (address == TIM_CFG) {
        return timer_cfg_value;
    }
    uint32_t ret = 0;
    for (int i = 0; i < 4; ++i) {
        ret |= (mem[address + i] << (i * 8));
    }
    return ret;
}

void Emulator::store32(uint32_t address, uint32_t value) {
    if (address == TERM_OUT) {
        std::cout << (char)(value & 0xFF) << std::flush;
        return;
    }
    if (address == TIM_CFG) {
        timer_cfg_value = value;
        timer_last = std::chrono::steady_clock::now();
        return;
    }
    mem[address + 0] = value & 0xFF;
    mem[address + 1] = (value >> 8) & 0xFF;
    mem[address + 2] = (value >> 16) & 0xFF;
    mem[address + 3] = (value >> 24) & 0xFF;
}

bool Emulator::execute_instruction() {
    poll_terminal_input();

    if (timer_cfg_value <= 7) {
        auto now = std::chrono::steady_clock::now();
        uint32_t period_ms = get_timer_period_ms(timer_cfg_value);
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - timer_last).count();
        if (elapsed >= period_ms) {
            csr[CAUSE] = 2;
            timer_last = now;
        }
    }

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

    if ((csr[CAUSE] == 2 || csr[CAUSE] == 3) && (csr[STATUS] & (1 << 2)) == 0) {
        if (csr[CAUSE] == 2 && (csr[STATUS] & 1) != 0) return false;
        if (csr[CAUSE] == 3 && (csr[STATUS] & (1 << 1)) != 0) return false;

        regs[SP_REG] -= 4;
        store32(regs[SP_REG], regs[PC_REG]);
        regs[SP_REG] -= 4;
        store32(regs[SP_REG], csr[STATUS]);
        csr[STATUS] |= (1 << 2);
        regs[PC_REG] = csr[HANDLER];
    }

    return false;
}

void Emulator::run() {
    while (true) {
        if (execute_instruction())
            break;
    }
    std::cout << "\nEmulated processor executed halt instruction\n";
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
