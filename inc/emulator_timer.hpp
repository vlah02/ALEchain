#pragma once
#include <chrono>
#include <functional>
#include "emulator_bus.hpp"

constexpr uint32_t TIM_CFG  = 0xFFFFFF10;

class TimerDevice {
public:
    using clock = std::chrono::steady_clock;
    using time_point = clock::time_point;

    explicit TimerDevice(uint32_t base, std::function<void()> cb)
        : base_(base), raise_irq_(std::move(cb)) { reset(); }

    void reset() {
        cfg_  = 0;
        last_ = clock::now();
    }

    Mmio mmio() {
        return Mmio{
            base_, 4,
            [this](uint32_t) -> uint32_t { return cfg_; },
            [this](uint32_t, uint32_t v) {
                cfg_ = v;
                last_ = clock::now();
            },
            "TIM_CFG"
        };
    }

    void tick(time_point now) {
        if ((cfg_ & 0x7u) <= 7u) {
            const uint32_t per = period_ms(cfg_);
            const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_).count();
            if (elapsed >= static_cast<long long>(per)) {
                last_ = now;
                if (raise_irq_) raise_irq_();
            }
        }
    }

    uint32_t cfg() const { return cfg_; }

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
        return 500;
    }

    uint32_t base_;
    uint32_t cfg_{0};
    time_point last_{};
    std::function<void()> raise_irq_;
};
