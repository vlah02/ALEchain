#pragma once
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include "emulator_memory.hpp"
#include "emulator_device.hpp"

class Bus {
public:
    explicit Bus(Memory& ram) : ram_(ram) {}

    bool map(std::unique_ptr<Device> d) {
        const auto b = d->base();
        const auto s = d->size();
        for (const auto& x : devices_) {
            const bool overlap =
                (b - x->base()) < x->size() || (x->base() - b) < s;
            if (overlap) return false;
        }
        devices_.push_back(std::move(d));
        std::sort(devices_.begin(), devices_.end(),
                  [](const auto& a, const auto& b){ return a->base() < b->base(); });
        return true;
    }

    uint32_t read32(uint32_t addr) const {
        for (auto& d : devices_) {
            if (d->contains(addr)) return d->read32(addr - d->base());
        }
        return ram_.load32(addr);
    }

    void write32(uint32_t addr, uint32_t val) const {
        for (auto& d : devices_) {
            if (d->contains(addr)) { d->write32(addr - d->base(), val); return; }
        }
        ram_.store32(addr, val);
    }

private:
    Memory& ram_;
    std::vector<std::unique_ptr<Device>> devices_;
};
