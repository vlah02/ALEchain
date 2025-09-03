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
    static uint32_t period_ms(uint32_t cfg) noexcept {
        static constexpr uint32_t table[8] = {
            500, 1000, 1500, 2000, 5000, 10000, 30000, 60000    
        };
        return table[cfg & 0x7u];
    }

    uint32_t cfg_{0};
    clock::time_point last_{};
    std::function<void()> raise_irq_;
};
