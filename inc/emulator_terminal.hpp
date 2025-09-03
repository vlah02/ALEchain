#pragma once
#include <cstdint>
#include <functional>
#include <iostream>
#include "emulator_device.hpp"

constexpr uint32_t TERM_CFG = 0xFFFFFF00;

class TerminalDevice : public Device {
public:
    using IrqFn = std::function<void()>;
    using ReadFn = std::function<int()>;
    using WriteFn = std::function<void(uint8_t)>;

    TerminalDevice(uint32_t base, IrqFn irq, ReadFn reader, WriteFn writer)
        : Device(base, 8, std::string_view{"TERM_CFG"})
        , raise_irq_(std::move(irq))
        , read_(std::move(reader))
        , write_(std::move(writer))
    {}

    void tick() override {
        if (!read_) return;
        const int c = read_();
        if (c != EOF) {
            rx_ = static_cast<uint8_t>(c);
            if (raise_irq_) raise_irq_();
        }
    }

    uint32_t read32(uint32_t off) override {
        switch (off) {
        case 0x0: return 0;
        case 0x4: {
            uint32_t v = rx_;
            rx_ = 0;
            return v;
        }
        default:  return 0;
        }
    }

    void write32(uint32_t off, uint32_t val) override {
        switch (off) {
        case 0x0:
            if (write_) write_(static_cast<uint8_t>(val & 0xFF));
            break;
        case 0x4:
        default:  break;
        }
    }

private:
    uint32_t rx_{0};
    IrqFn raise_irq_;
    ReadFn read_;
    WriteFn write_;
};
