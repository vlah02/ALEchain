#include "../inc/assembler_pool.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"
#include "../inc/assembler_reloc.hpp"

#include <vector>
#include <limits>
#include <cstdio>
#include <iostream>

std::vector<AsmPool::PoolItem> AsmPool::items;
std::unordered_map<std::string, long> AsmPool::equs;
std::vector<EquEntry> AsmPool::pending_equs;

void AsmPool::enqueue_value(const std::string& sec, const int site, const int value, const bool patchInPlace) {
    items.push_back(PoolItem{sec, site, value, "", 0, patchInPlace});
}

void AsmPool::enqueue_symbol(const std::string& sec, const int site, const std::string& sym, const int addend, const bool patchInPlace) {
    items.push_back(PoolItem{sec, site, -1, sym, addend, patchInPlace});
}

void AsmPool::add_literal(const std::string& section, const int site, const std::string& sym) {
    std::string base = sym;
    int addend = 0;
    if (const auto plus = base.find('+'); plus != std::string::npos) {
        addend = std::stoi(base.substr(plus + 1));
        base = base.substr(0, plus);
    } else if (const auto minus = base.find('-'); minus != std::string::npos) {
        addend = -std::stoi(base.substr(minus + 1));
        base = base.substr(0, minus);
    }

    if (const auto it = equs.find(base); it != equs.end()) {
        const long equVal = it->second + addend;
        enqueue_value(section, site, static_cast<int>(equVal), true);
        return;
    }

    enqueue_symbol(section, site, base, addend, false);
}


void AsmPool::flush() {
    resolve_equs();

    for (auto& it : items) {
        Section* section = Section::extract(it.section);
        auto& data = section->getData();

        const int lineToReplace = it.site;
        const int literalLine = static_cast<int>(data.size());
        const int displacement = literalLine - lineToReplace - 4;

        data[lineToReplace + 3] = displacement & 0xFF;
        data[lineToReplace + 2] |= (displacement >> 8) & 0x0F;

        if (!it.symbol.empty()) {
            if (auto symEntryIt = SymTab::table.find(it.symbol); symEntryIt != SymTab::table.end() && symEntryIt->second->line != -1) {
                int valueToInsert = 0;
                valueToInsert = symEntryIt->second->line + it.addend;

                if (it.patchInPlace) {
                    data[lineToReplace + 2] = ((valueToInsert >> 8) & 0x0F) | (data[lineToReplace + 2] & 0xF0);
                    data[lineToReplace + 3] = (valueToInsert & 0xFF);
                }

                section->insertInt(valueToInsert);
                std::fprintf(stderr, "[POOL] insertInt(%d) for LOCAL symbol=%s at section=%s, offset=%zu\n",
                             valueToInsert, it.symbol.c_str(), it.section.c_str(), data.size());
            } else {
                if (it.patchInPlace) {
                    data[lineToReplace + 2] &= 0xF0;
                    data[lineToReplace + 3]  = 0;
                }
                section->insertInt(0);
                std::fprintf(stderr, "[POOL] insertInt(0) for EXTERNAL symbol=%s at section=%s, offset=%zu\n",
                             it.symbol.c_str(), it.section.c_str(), data.size());
            }

            const int patch_off = literalLine;
            AsmRelocs::add(it.section, patch_off, "R_ABS32", it.symbol, it.addend);

        } else {
            section->insertInt(static_cast<int>(it.value));
        }
    }

    check_weaks();
}

void AsmPool::resolve_equs() {
    auto lookup = [&](const std::string& sym, long& out) -> bool {
        if (const auto it = equs.find(sym); it != equs.end()) {
            out = it->second;
            return true;
        }
        if (const auto it = SymTab::table.find(sym); it != SymTab::table.end() && it->second->line != -1) {
            out = static_cast<long>(it->second->line);
            return true;
        }
        return false;
    };

    for (auto& e : pending_equs) {
        long L = 0, R = 0;
        const bool okL = lookup(e.lhs, L);
        const bool okR = lookup(e.rhs, R);

        long val = 0;
        if (!okL || !okR) {
            std::cerr << "ERROR: undefined symbol in deferred .equ: "
                      << (!okL ? e.lhs : "") << ((!okL && !okR) ? " and " : "")
                      << (!okR ? e.rhs : "") << "\n";
        } else {
            val = (e.op == EquEntry::Op::ADD) ? (L + R) : (L - R);
        }

        equs[e.dst] = val;
        SymTab::add_definition(e.dst, "", static_cast<int>(val));
    }

    pending_equs.clear();
}

void AsmPool::check_weaks() {
    for (const auto& w : SymTab::weaks) {
        auto it = SymTab::table.find(w);
        if (it == SymTab::table.end()) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
        const auto* def = it->second;
        const bool hasDef = (def->line != -1);
        if (const bool hasOcc = !def->occurences.empty(); !hasDef && !hasOcc) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
    }
}

void AsmPool::clear() {
    items.clear();
}
