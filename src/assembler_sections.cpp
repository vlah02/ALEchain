#include "../inc/assembler_sections.hpp"
#include "../inc/assembler_symbols.hpp"
#include <bitset>
#include <iostream>

Section* Sections::extract(const std::string& name) {
    auto it = map.find(name);
    if (it != map.end()) return it->second.get();

    auto ptr = std::make_unique<Section>(name);
    auto raw = ptr.get();
    map.emplace(name, std::move(ptr));
    order.push_back(name);
    return raw;
}

void Section::add_instruction(unsigned char OC, unsigned char MOD,
                              unsigned char RegA, unsigned char RegB, unsigned char RegC,
                              signed short Disp) {
    std::fprintf(stderr,
        "add_instruction: OC=0x%X MOD=0x%X RegA=0x%X RegB=0x%X RegC=0x%X Disp=%d | Offset=%zu\n",
        OC, MOD, RegA, RegB, RegC, Disp, data.size());

    std::fprintf(stderr, "   Will emit at instruction index %zu\n", data.size() / 4);

    data.push_back(static_cast<unsigned char>((OC << 4) | (MOD & 0x0F)));
    data.push_back(static_cast<unsigned char>((RegA << 4) | (RegB & 0x0F)));
    data.push_back(static_cast<unsigned char>((RegC << 4) | ((Disp >> 8) & 0x0F)));
    data.push_back(static_cast<unsigned char>(Disp & 0xFF));

    std::fprintf(stderr, "   Bytes: %02X %02X %02X %02X\n",
                 data[data.size()-4], data[data.size()-3],
                 data[data.size()-2], data[data.size()-1]);
}

void Section::insertInt(int number) {
    data.push_back(static_cast<unsigned char>( number        & 0xFF));
    data.push_back(static_cast<unsigned char>((number >>  8) & 0xFF));
    data.push_back(static_cast<unsigned char>((number >> 16) & 0xFF));
    data.push_back(static_cast<unsigned char>((number >> 24) & 0xFF));
}

void Section::insertByte(int number) {
    data.push_back(static_cast<unsigned char>(number));
}

void Sections::out(std::ostream& out) const {
    out << ".sections\n";
    for (const auto& name : order) {
        const auto& sec = *map.at(name);
        out << "." << sec.getName() << "\n";

        int i = 1;
        for (auto byte : sec.getData()) {
            std::bitset<8> b(byte);
            out << b.to_string() << (i++ % 4 ? " " : "\n");
        }
        out << "\n";
    }
}
