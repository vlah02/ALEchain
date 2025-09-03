#pragma once
#include <unordered_map>
#include <memory>

struct MemPage {
    static constexpr uint32_t kSize = 4096;
    uint8_t data[kSize]{};
};

class Memory {
public:
    uint8_t load8 (uint32_t addr) const {
        auto [p, off] = locate(addr);
        return p ? p->data[off] : 0;
    }

    void store8(uint32_t addr, uint8_t v) const {
        auto [p, off] = ensure(addr);
        p->data[off] = v;
    }

    uint32_t load32(uint32_t addr) const {
        uint32_t v = 0;
        v |= static_cast<uint32_t>(load8(addr + 0)) << 0;
        v |= static_cast<uint32_t>(load8(addr + 1)) << 8;
        v |= static_cast<uint32_t>(load8(addr + 2)) << 16;
        v |= static_cast<uint32_t>(load8(addr + 3)) << 24;
        return v;
    }

    void store32(uint32_t addr, uint32_t v) const {
        store8(addr + 0, static_cast<uint8_t>(v & 0xFF));
        store8(addr + 1, static_cast<uint8_t>((v >> 8) & 0xFF));
        store8(addr + 2, static_cast<uint8_t>((v >> 16) & 0xFF));
        store8(addr + 3, static_cast<uint8_t>((v >> 24) & 0xFF));
    }

private:
    mutable std::unordered_map<uint32_t, std::unique_ptr<MemPage>> pages;

    static constexpr uint32_t kMask = MemPage::kSize - 1;
    static uint32_t pageBase(uint32_t addr) { return addr & ~kMask; }
    static uint32_t offset  (uint32_t addr) { return addr &  kMask; }

    std::pair<MemPage*, uint32_t> locate(uint32_t addr) const {
        auto it = pages.find(pageBase(addr));
        return { it == pages.end() ? nullptr : it->second.get(), offset(addr) };
    }

    std::pair<MemPage*, uint32_t> ensure(uint32_t addr) const {
        auto key = pageBase(addr);
        auto it = pages.find(key);
        if (it == pages.end()) {
            it = pages.emplace(key, std::make_unique<MemPage>()).first;
        }
        return { it->second.get(), offset(addr) };
    }
};
