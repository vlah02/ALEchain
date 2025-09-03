#pragma once
#include <vector>
#include <utility>
#include <memory>
#include "emulator_memory.hpp"
#include "emulator_device.hpp"

class Bus {
public:
    explicit Bus(Memory& ram) : ram_(ram) {}

    void map(std::unique_ptr<Device> d) { devices_.push_back(std::move(d)); }

    uint32_t read32(uint32_t addr) {
        for (auto& d : devices_) {
            if (d->contains(addr)) return d->read32(addr - d->base());
        }
        return ram_.load32(addr);
    }

    void write32(uint32_t addr, uint32_t val) {
        for (auto& d : devices_) {
            if (d->contains(addr)) { d->write32(addr - d->base(), val); return; }
        }
        ram_.store32(addr, val);
    }

private:
    Memory& ram_;
    std::vector<std::unique_ptr<Device>> devices_;
};
