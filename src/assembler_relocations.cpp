#include "../inc/assembler_relocations.hpp"

void Relocations::add(std::string sec, int off, std::string type, std::string sym, int addend) {
    items_.push_back(Entry{std::move(sec), off, std::move(type), std::move(sym), addend});
}

void Relocations::out(std::ostream& os) const {
    if (items_.empty()) return;
    os << ".relocations\n";
    for (const auto& r : items_) {
        os << r.section << " " << r.offset << " " << r.type << " " << r.symbol << " " << r.addend << "\n";
    }
}

void Relocations::clear() {
    items_.clear();
}
