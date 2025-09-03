#pragma once
#include <iostream>
#include <chrono>
#include "emulator_bus.hpp"

constexpr uint32_t TERM_OUT = 0xFFFFFF00;
constexpr uint32_t TERM_IN  = 0xFFFFFF04;
constexpr uint32_t TIM_CFG  = 0xFFFFFF10;

inline Mmio make_term_out()
{
    return Mmio{
        TERM_OUT,
        4,
        [] (uint32_t) -> uint32_t {
            return 0;
        },
        [] (uint32_t, uint32_t v) {
            std::cout << static_cast<char>(v & 0xFF) << std::flush;
        },
        "TERM_OUT"
    };
}

inline Mmio make_term_in(uint32_t& term_in_value)
{
    return Mmio{
        TERM_IN,
        4,
        [&term_in_value] (uint32_t) -> uint32_t {
            uint32_t v = term_in_value;
            term_in_value = 0;
            return v;
        },
        [] (uint32_t, uint32_t) {

        },
        "TERM_IN"
    };
}

inline Mmio make_tim_cfg(uint32_t& timer_cfg_value, std::chrono::steady_clock::time_point& timer_last) {
    return Mmio{
        TIM_CFG,
        4,
        [&timer_cfg_value] (uint32_t) -> uint32_t {
            return timer_cfg_value;
        },
        [&timer_cfg_value, &timer_last] (uint32_t, uint32_t v) {
            timer_cfg_value = v;
            timer_last = std::chrono::steady_clock::now();
        },
        "TIM_CFG"
    };
}
