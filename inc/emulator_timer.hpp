#pragma once
#include <cstdint>
#include <chrono>
#include <functional>
#include "emulator_device.hpp"

constexpr uint32_t TIM_CFG = 0xFFFFFF10;

class TimerDevice : public Device {
public:
    using IrqFn = std::function<void()>;
    using clock = std::chrono::steady_clock;

    explicit TimerDevice(uint32_t base, IrqFn irq)
        : Device(base, 4, std::string_view{"TIM_CFG"})
        , raise_irq_(std::move(irq))
    {
        cfg_  = 0;
        last_ = clock::now();
    }

    void tick() override {
        const auto now = clock::now();
        const uint32_t per = period_ms(cfg_);
        if (per == 0) return;

        const auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();
        if (elapsed >= static_cast<long long>(per)) {
            last_ = now;
            if (raise_irq_) raise_irq_();
        }
    }

    uint32_t read32(uint32_t) override {
        return cfg_;
    }

    void write32(uint32_t, uint32_t v) override {
        cfg_ = v;
        last_ = clock::now();
    }

private:
    static uint32_t period_ms(uint32_t cfg) {
        switch (cfg & 0x7u) {
        case 0: return 500;
        case 1: return 1000;
        case 2: return 1500;
        case 3: return 2000;
        case 4: return 5000;
        case 5: return 10000;
        case 6: return 30000;
        case 7: return 60000;
        }
        return 0;
    }

    uint32_t cfg_{0};
    clock::time_point last_{};
    std::function<void()> raise_irq_;
};
