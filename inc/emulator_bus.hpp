#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <utility>
#include <string>
#include "emulator_memory.hpp"

struct Mmio {
    uint32_t base{};
    uint32_t size{};
    std::function<uint32_t(uint32_t)> read32;
    std::function<void(uint32_t, uint32_t)> write32;
    const char* name{};

    bool contains(uint32_t addr) const {
        return addr >= base && addr < base + size;
    }
};

class Bus {
public:
    explicit Bus(Memory& ram) : ram_(ram) {}

    void map(Mmio dev) { devices_.push_back(std::move(dev)); }

    uint32_t read32(uint32_t addr) {
        for (auto& d : devices_) {
            if (d.contains(addr)) return d.read32(addr - d.base);
        }
        return ram_.load32(addr);
    }

    void write32(uint32_t addr, uint32_t val) {
        for (auto& d : devices_) {
            if (d.contains(addr)) { d.write32(addr - d.base, val); return; }
        }
        ram_.store32(addr, val);
    }

private:
    Memory& ram_;
    std::vector<Mmio> devices_;
};
