#include "../inc/assembler_reloc.hpp"

std::vector<AsmRelocs::AsmReloc> AsmRelocs::items;

void AsmRelocs::add(std::string sec, int off, std::string type, std::string sym, int addend) {
    items.push_back(AsmReloc{std::move(sec), off, std::move(type), std::move(sym), addend});
}

void AsmRelocs::out(std::ostream& os) {
    if (items.empty()) return;
    os << ".relocations\n";
    for (const auto& r : items) {
        os << r.section << " " << r.offset << " " << r.type << " " << r.symbol << " " << r.addend << "\n";
    }
}

void AsmRelocs::clear() {
    items.clear();
}
