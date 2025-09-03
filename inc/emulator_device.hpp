#pragma once
#include <cstdint>
#include <string_view>

class Device {
public:
    Device(uint32_t base, uint32_t size, std::string_view name)
      : base_(base), size_(size), name_(name) {}
    virtual ~Device() = default;

    [[nodiscard]] bool contains(uint32_t addr) const noexcept {
        return (addr - base_) < size_;
    }

    [[nodiscard]] uint32_t base() const noexcept { return base_; }
    [[nodiscard]] uint32_t size() const noexcept { return size_; }
    [[nodiscard]] std::string_view name() const noexcept { return name_; }

    virtual uint32_t read32(uint32_t off) = 0;
    virtual void write32(uint32_t off, uint32_t val) = 0;
    virtual void tick() = 0;

private:
    uint32_t base_;
    uint32_t size_;
    std::string_view name_;
};
