#pragma once
#include <string>
#include <vector>
#include <ostream>

class Relocations {
public:
    struct Entry {
        std::string section;
        int offset;
        std::string type;
        std::string symbol;
        int addend;
    };

    Relocations() = default;
    Relocations(const Relocations&) = delete;
    Relocations& operator=(const Relocations&) = delete;

    void add(std::string sec, int off, std::string type, std::string sym, int addend);
    void out(std::ostream& os) const;
    void clear();

    [[nodiscard]] const std::vector<Entry>& items() const { return items_; }

private:
    std::vector<Entry> items_;
};
