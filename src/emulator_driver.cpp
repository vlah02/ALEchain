#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <chrono>
#include "../inc/emulator_driver.hpp"
#include "../inc/emulator_devices.hpp"

constexpr uint32_t PC_START = 0x40000000;
constexpr uint32_t SP_REG   = 14;
constexpr uint32_t PC_REG   = 15;

constexpr uint32_t STATUS = 0;
constexpr uint32_t HANDLER = 1;
constexpr uint32_t CAUSE   = 2;

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

Emulator::Emulator() : bus(mem) {
    std::memset(regs, 0, sizeof(regs));
    std::memset(csr, 0, sizeof(csr));
    regs[PC_REG] = PC_START;
    setup_terminal();

    timer_cfg_value = 0;
    timer_last = std::chrono::steady_clock::now();

    bus.map(make_term_out());
    bus.map(make_term_in(term_in_value));
    bus.map(make_tim_cfg(timer_cfg_value, timer_last));
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
        std::exit(1);
    }
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        uint32_t addr;
        char colon;
        ss >> std::hex >> addr >> colon;
        if (!ss || colon != ':') continue;

        for (int i = 0; i < 8; ++i) {
            unsigned int byte;
            ss >> std::hex >> byte;
            if (!ss) break;
            mem.store8(addr + i, static_cast<uint8_t>(byte));
        }
    }
}

void Emulator::poll_terminal_input() {
    int c = getchar();
    if (c != EOF) {
        term_in_value = static_cast<uint8_t>(c);
        csr[CAUSE] = 3;
    }
}

uint32_t Emulator::load32(uint32_t address) {
    return bus.read32(address);
}

void Emulator::store32(uint32_t address, uint32_t value) {
    bus.write32(address, value);
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

    uint32_t inst = mem.load32(pc);
    uint8_t b0 =  inst        & 0xFF;
    uint8_t b1 = (inst >> 8)  & 0xFF;
    uint8_t b2 = (inst >> 16) & 0xFF;
    uint8_t b3 = (inst >> 24) & 0xFF;

    op   = (b0 >> 4) & 0x0F;
    mod  =  b0       & 0x0F;
    regA = (b1 >> 4) & 0x0F;
    regB =  b1       & 0x0F;
    regC = (b2 >> 4) & 0x0F;

    uint16_t udisp = static_cast<uint16_t>(((b2 & 0x0F) << 8) | b3);
    if (udisp & 0x0800) {
        disp = static_cast<int16_t>(udisp | 0xF000);
    } else {
        disp = static_cast<int16_t>(udisp);
    }

    if (op != 0)
        regs[PC_REG] += 4;
    regs[0] = 0;

    switch (op) {
    case 0x0:
        return true;

    case 0x1:
        regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
        regs[SP_REG] -= 4; store32(regs[SP_REG], csr[STATUS]);
        csr[CAUSE] = 4;
        csr[STATUS] &= ~1U;
        regs[PC_REG] = csr[HANDLER];
        break;

    case 0x2:
        switch (mod) {
        case 0x0:
            regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = REG(regA) + REG(regB) + disp;
            break;
        case 0x1:
            regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = load32(REG(regA) + REG(regB) + disp);
            break;
        }
        break;

    case 0x3:
        switch (mod) {
        case 0x0: regs[PC_REG] = REG(regA) + disp; break;
        case 0x1: if (REG(regB) == REG(regC)) regs[PC_REG] = REG(regA) + disp; break;
        case 0x2: if (REG(regB) != REG(regC)) regs[PC_REG] = REG(regA) + disp; break;
        case 0x3: if ((int32_t)REG(regB) > (int32_t)REG(regC)) regs[PC_REG] = REG(regA) + disp; break;
        case 0x8: regs[PC_REG] = load32(REG(regA) + disp); break;
        case 0x9: if (REG(regB) == REG(regC)) regs[PC_REG] = load32(REG(regA) + disp); break;
        case 0xA: if (REG(regB) != REG(regC)) regs[PC_REG] = load32(REG(regA) + disp); break;
        case 0xB: if ((int32_t)REG(regB) > (int32_t)REG(regC)) regs[PC_REG] = load32(REG(regA) + disp); break;
        }
        break;

    case 0x4: {
        uint32_t tmp = REG(regB);
        if (regB != 0) regs[regB] = REG(regC);
        if (regC != 0) regs[regC] = tmp;
    } break;

    case 0x5:
        switch (mod) {
        case 0x0: if (regA) regs[regA] = REG(regB) + REG(regC); break;
        case 0x1: if (regA) regs[regA] = REG(regB) - REG(regC); break;
        case 0x2: if (regA) regs[regA] = REG(regB) * REG(regC); break;
        case 0x3: if (regA) regs[regA] = REG(regB) / REG(regC); break;
        }
        break;

    case 0x6:
        switch (mod) {
        case 0x0: if (regA) regs[regA] = ~REG(regB); break;
        case 0x1: if (regA) regs[regA] = REG(regB) & REG(regC); break;
        case 0x2: if (regA) regs[regA] = REG(regB) | REG(regC); break;
        case 0x3: if (regA) regs[regA] = REG(regB) ^ REG(regC); break;
        }
        break;

    case 0x7:
        switch (mod) {
        case 0x0: if (regA) regs[regA] = REG(regB) << REG(regC); break;
        case 0x1: if (regA) regs[regA] = REG(regB) >> REG(regC); break;
        }
        break;

    case 0x8:
        switch (mod) {
        case 0x0: store32(REG(regA) + REG(regB) + disp, REG(regC)); break;
        case 0x2: store32(load32(REG(regA) + REG(regB) + disp), REG(regC)); break;
        case 0x1:
            if (regA != 0) regs[regA] = regs[regA] + disp;
            store32(regs[regA], REG(regC));
            break;
        }
        break;

    case 0x9:
        switch (mod) {
        case 0x0: if (regA) regs[regA] = csr[regB]; break;
        case 0x1: if (regA) regs[regA] = REG(regB) + disp; break;
        case 0x2: if (regA) regs[regA] = load32(REG(regB) + REG(regC) + disp); break;
        case 0x3:
            if (regA) regs[regA] = load32(REG(regB));
            if (regB) regs[regB] = regs[regB] + disp;
            break;
        case 0x4: csr[regA] = REG(regB); break;
        case 0x5: csr[regA] = csr[regB] | disp; break;
        case 0x6: csr[regA] = load32(REG(regB) + REG(regC) + disp); break;
        case 0x7:
            csr[regA] = load32(REG(regB));
            if (regB) regs[regB] = regs[regB] + disp;
            break;
        }
        break;
    }

    regs[0] = 0;

    if ((csr[CAUSE] == 2 || csr[CAUSE] == 3) && (csr[STATUS] & (1 << 2)) == 0) {
        if (csr[CAUSE] == 2 && (csr[STATUS] & 1) != 0) return false;
        if (csr[CAUSE] == 3 && (csr[STATUS] & (1 << 1)) != 0) return false;

        regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
        regs[SP_REG] -= 4; store32(regs[SP_REG], csr[STATUS]);
        csr[STATUS] |= (1 << 2);
        regs[PC_REG] = csr[HANDLER];
    }

    return false;
}

void Emulator::run() {
    while (true) {
        if (execute_instruction()) break;
    }
    std::cout << "\nEmulated processor executed halt instruction\n";
    print_state();
}

void Emulator::print_state() {
    std::cout << "Emulated processor state:\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << " r" << std::dec << i << "=" << "0x"
                  << std::setw(8) << std::setfill('0') << std::hex << regs[i];
        if (i % 4 == 3) std::cout << '\n';
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
