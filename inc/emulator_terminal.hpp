#pragma once
#include <cstdint>
#include <functional>
#include <iostream>
#include "emulator_bus.hpp"

constexpr uint32_t TERM_CFG = 0xFFFFFF00;

class TerminalDevice {
public:
    using ReadFn  = std::function<int()>;
    using WriteFn = std::function<void(uint8_t)>;
    using IrqFn   = std::function<void()>;

    TerminalDevice(uint32_t base, IrqFn irq,
                   ReadFn reader,
                   WriteFn writer)
        : base_(base),
          raise_irq_(std::move(irq)),
          read_(std::move(reader)),
          write_(std::move(writer)) {}

    void tick() {
        if (!read_) return;
        int c = read_();
        if (c != EOF) {
            rx_ = static_cast<uint8_t>(c);
            if (raise_irq_) raise_irq_();
        }
    }

    Mmio mmio() {
        return Mmio{
            base_, 8,
            [this](uint32_t off) -> uint32_t {
                switch (off) {
                    case 0x0: return 0;
                    case 0x4: {
                        uint32_t v = rx_;
                        rx_ = 0;
                        return v;
                    }
                    default: return 0;
                }
            },
            [this](uint32_t off, uint32_t v) {
                switch (off) {
                    case 0x0: if (write_) write_(static_cast<uint8_t>(v & 0xFF)); break;
                    case 0x4: break;
                    default:  break;
                }
            },
            "TERMINAL"
        };
    }

private:
    uint32_t base_;
    uint32_t rx_{0};
    IrqFn    raise_irq_;
    ReadFn   read_;
    WriteFn  write_;
};
