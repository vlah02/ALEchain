#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <memory>
#include "../inc/emulator_driver.hpp"

constexpr uint32_t PC_START = 0x40000000;
constexpr uint32_t SP_REG   = 14;
constexpr uint32_t PC_REG   = 15;

constexpr uint32_t STATUS = 0;
constexpr uint32_t HANDLER = 1;
constexpr uint32_t CAUSE   = 2;

#define REG(x) ((x) == 0 ? 0 : regs[x])

Emulator::Emulator() : bus(mem) {

    std::memset(regs, 0, sizeof(regs));
    std::memset(csr,  0, sizeof(csr));
    regs[PC_REG] = PC_START;

    auto timer_dev = std::make_unique<TimerDevice>(
        TIM_CFG,
        [this]{ csr[CAUSE] = 2; }
    );
    timer = timer_dev.get();
    bus.map(std::move(timer_dev));

    auto terminal_dev = std::make_unique<TerminalDevice>(
        TERM_CFG,
        [this]{ csr[CAUSE] = 3; },
        HostTerminal::read_char_nonblock,
        [](uint8_t ch){ std::cout << (char)ch << std::flush; }
    );
    terminal = terminal_dev.get();
    bus.map(std::move(terminal_dev));

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

bool Emulator::execute_instruction() {
    if (terminal) terminal->tick();
    if (timer) timer->tick();

    uint32_t pc = regs[PC_REG];
    uint8_t op, mod, regA, regB, regC;
    int16_t disp;

    uint32_t inst = load32(pc);
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
