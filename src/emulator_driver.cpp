#include <cstdint>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <memory>
#include "../inc/emulator_driver.hpp"
#include "../inc/emulator_terminal.hpp"
#include "../inc/emulator_timer.hpp"
#include "../inc/emulator_isa.hpp"

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
    bus.map(std::move(timer_dev));

    auto terminal_dev = std::make_unique<TerminalDevice>(
        TERM_CFG,
        [this]{ csr[CAUSE] = 3; },
        HostTerminal::read_char_nonblock,
        [](uint8_t ch){ std::cout << static_cast<char>(ch) << std::flush; }
    );
    bus.map(std::move(terminal_dev));
}

void Emulator::load_memory(const std::string& hex_filename) const {
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
    bus.tick();

    uint32_t pc = regs[PC_REG];

    uint32_t inst = load32(pc);
    uint8_t b0 =  inst        & 0xFF;
    uint8_t b1 = (inst >> 8)  & 0xFF;
    uint8_t b2 = (inst >> 16) & 0xFF;
    uint8_t b3 = (inst >> 24) & 0xFF;

    uint8_t op =   (b0 >> 4) & 0x0F;
    uint8_t mod =         b0 & 0x0F;
    uint8_t regA = (b1 >> 4) & 0x0F;
    uint8_t regB =        b1 & 0x0F;
    uint8_t regC = (b2 >> 4) & 0x0F;
    int16_t disp;

    uint16_t udisp = static_cast<uint16_t>(((b2 & 0x0F) << 8) | b3);
    if (udisp & 0x0800) {
        disp = static_cast<int16_t>(udisp | 0xF000);
    } else {
        disp = static_cast<int16_t>(udisp);
    }

    auto opcode = static_cast<OPCODE>(op);
    if (opcode != OPCODE::HALT)
        regs[PC_REG] += 4;
    regs[0] = 0;

    switch (opcode) {
    case OPCODE::HALT:
        return true;

    case OPCODE::INT:
        regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
        regs[SP_REG] -= 4; store32(regs[SP_REG], csr[STATUS]);
        csr[CAUSE] = 4;
        csr[STATUS] &= ~1U;
        regs[PC_REG] = csr[HANDLER];
        break;

    case OPCODE::CALL: {
        switch (static_cast<CALL_MOD>(mod)) {
        case CALL_MOD::LINK_TO_REGS_PLUS_IMM:
            regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = REG(regA) + REG(regB) + disp;
            break;
        case CALL_MOD::LINK_TO_MEM_AT_REGS_PLUS_IMM:
            regs[SP_REG] -= 4; store32(regs[SP_REG], regs[PC_REG]);
            regs[PC_REG] = load32(REG(regA) + REG(regB) + disp);
            break;
        }
    } break;

    case OPCODE::JUMP: {
        switch (static_cast<JUMP_MOD>(mod)) {
        case JUMP_MOD::ABS:
            regs[PC_REG] = REG(regA) + disp;
            break;
        case JUMP_MOD::BR_EQ:
            if (REG(regB) == REG(regC)) regs[PC_REG] = REG(regA) + disp;
            break;
        case JUMP_MOD::BR_NE:
            if (REG(regB) != REG(regC)) regs[PC_REG] = REG(regA) + disp;
            break;
        case JUMP_MOD::BR_GT_SIGNED:
            if (static_cast<int32_t>(REG(regB)) > static_cast<int32_t>(REG(regC))) regs[PC_REG] = REG(regA) + disp;
            break;
        case JUMP_MOD::IND:
            regs[PC_REG] = load32(REG(regA) + disp);
            break;
        case JUMP_MOD::BR_EQ_IND:
            if (REG(regB) == REG(regC)) regs[PC_REG] = load32(REG(regA) + disp);
            break;
        case JUMP_MOD::BR_NE_IND:
            if (REG(regB) != REG(regC)) regs[PC_REG] = load32(REG(regA) + disp);
            break;
        case JUMP_MOD::BR_GT_IND_SIGNED:
            if (static_cast<int32_t>(REG(regB)) > static_cast<int32_t>(REG(regC))) regs[PC_REG] = load32(REG(regA) + disp);
            break;
        }
    } break;

    case OPCODE::XCHG: {
        uint32_t tmp = REG(regB);
        if (regB != 0) regs[regB] = REG(regC);
        if (regC != 0) regs[regC] = tmp;
    } break;

    case OPCODE::ALU: {
        switch (static_cast<ALU_MOD>(mod)) {
        case ALU_MOD::ADD: if (regA) regs[regA] = REG(regB) + REG(regC); break;
        case ALU_MOD::SUB: if (regA) regs[regA] = REG(regB) - REG(regC); break;
        case ALU_MOD::MUL: if (regA) regs[regA] = REG(regB) * REG(regC); break;
        case ALU_MOD::DIV: if (regA) regs[regA] = REG(regB) / REG(regC); break;
        }
    } break;

    case OPCODE::LOGIC: {
        switch (static_cast<LOGIC_MOD>(mod)) {
        case LOGIC_MOD::NOT: if (regA) regs[regA] = ~REG(regB); break;
        case LOGIC_MOD::AND: if (regA) regs[regA] = REG(regB) & REG(regC); break;
        case LOGIC_MOD::OR:  if (regA) regs[regA] = REG(regB) | REG(regC); break;
        case LOGIC_MOD::XOR: if (regA) regs[regA] = REG(regB) ^ REG(regC); break;
        }
    } break;

    case OPCODE::SHIFT: {
        switch (static_cast<SHIFT_MOD>(mod)) {
        case SHIFT_MOD::SHL: if (regA) regs[regA] = REG(regB) << REG(regC); break;
        case SHIFT_MOD::SHR: if (regA) regs[regA] = REG(regB) >> REG(regC); break;
        }
    } break;

    case OPCODE::STORE: {
        switch (static_cast<STORE_MOD>(mod)) {
        case STORE_MOD::TO_ADDR_REGS_PLUS_IMM:
            store32(REG(regA) + REG(regB) + disp, REG(regC));
            break;
        case STORE_MOD::TO_ADDR_AT_MEM_OF_REGS_PLUS_IMM:
            store32(load32(REG(regA) + REG(regB) + disp), REG(regC));
            break;
        case STORE_MOD::PREINC_AND_STORE:
            if (regA != 0) regs[regA] = regs[regA] + disp;
            store32(regs[regA], REG(regC));
            break;
        }
    } break;

    case OPCODE::LOAD: {
        switch (static_cast<LOAD_MOD>(mod)) {
        case LOAD_MOD::CSR_READ:
            if (regA) regs[regA] = csr[regB];
            break;
        case LOAD_MOD::ADD_IMMEDIATE:
            if (regA) regs[regA] = REG(regB) + disp;
            break;
        case LOAD_MOD::LOAD_FROM_ADDR:
            if (regA) regs[regA] = load32(REG(regB) + REG(regC) + disp);
            break;
        case LOAD_MOD::LOAD_FROM_REG_AND_POSTINC:
            if (regA) regs[regA] = load32(REG(regB));
            if (regB) regs[regB] = regs[regB] + disp;
            break;
        case LOAD_MOD::CSR_WRITE_FROM_REG:
            csr[regA] = REG(regB);
            break;
        case LOAD_MOD::CSR_OR_IMMEDIATE:
            csr[regA] = csr[regB] | disp;
            break;
        case LOAD_MOD::CSR_WRITE_FROM_ADDR:
            csr[regA] = load32(REG(regB) + REG(regC) + disp);
            break;
        case LOAD_MOD::CSR_WRITE_FROM_REG_AND_POSTINC:
            csr[regA] = load32(REG(regB));
            if (regB) regs[regB] = regs[regB] + disp;
            break;
        }
    } break;

    default:
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
