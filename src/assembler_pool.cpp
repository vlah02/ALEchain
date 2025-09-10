#include "../inc/assembler_pool.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"
#include "../inc/assembler_reloc.hpp"

#include <vector>
#include <limits>
#include <cstdio>
#include <iostream>

std::unordered_map<std::string, long> AsmPool::equs;
std::vector<EquEntry> AsmPool::pending_equs;

static std::vector<PoolItem>& pool_items() {
    static std::vector<PoolItem> items;
    return items;
}

void AsmPool::enqueue_value(const std::string& sec, int site, int value, bool patchInPlace) {
    pool_items().push_back(PoolItem{sec, site, value, "", 0, patchInPlace});
}

void AsmPool::enqueue_symbol(const std::string& sec, int site, const std::string& sym, int addend, bool patchInPlace) {
    pool_items().push_back(PoolItem{sec, site, -1, sym, addend, patchInPlace});
}

void AsmPool::add_literal(const std::string& sec, int site, const std::string& sym) {
    std::string base = sym;
    int addend = 0;
    if (auto plus = base.find('+'); plus != std::string::npos) {
        addend = std::stoi(base.substr(plus + 1));
        base = base.substr(0, plus);
    } else if (auto minus = base.find('-'); minus != std::string::npos) {
        addend = -std::stoi(base.substr(minus + 1));
        base = base.substr(0, minus);
    }

    if (auto it = AsmPool::equs.find(base); it != AsmPool::equs.end()) {
        const long equVal = it->second + addend;
        AsmPool::enqueue_value(sec, site, static_cast<int>(equVal), true);
        return;
    }

    AsmPool::enqueue_symbol(sec, site, base, addend, false);
}


void AsmPool::flush() {
    resolve_equs();

    auto& items = pool_items();
    for (auto& it : items) {
        Section* section = Section::extract(it.section);
        auto& data = section->getData();

        const int lineToReplace = it.site;
        const int literalLine   = (int)data.size();
        const int displacement  = literalLine - lineToReplace - 4;

        data[lineToReplace + 3] = displacement & 0xFF;
        data[lineToReplace + 2] |= (displacement >> 8) & 0x0F;

        if (!it.symbol.empty()) {
            int valueToInsert = 0;
            bool needReloc = true;

            auto symEntryIt = SymTab::table.find(it.symbol);
            const bool localDefined = (symEntryIt != SymTab::table.end() && symEntryIt->second->line != -1);

            if (localDefined) {
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
            section->insertInt((int)it.value);
        }
    }

    check_weaks();
}

void AsmPool::resolve_equs() {
    auto lookup = [&](const std::string& sym, long& out) -> bool {
        if (auto it = AsmPool::equs.find(sym); it != AsmPool::equs.end()) {
            out = it->second;
            return true;
        }
        if (auto it = SymTab::table.find(sym); it != SymTab::table.end() && it->second->line != -1) {
            out = static_cast<long>(it->second->line);
            return true;
        }
        return false;
    };

    for (auto& e : AsmPool::pending_equs) {
        long L = 0, R = 0;
        bool okL = lookup(e.lhs, L);
        bool okR = lookup(e.rhs, R);

        long val = 0;
        if (!okL || !okR) {
            std::cerr << "ERROR: undefined symbol in deferred .equ: "
                      << (!okL ? e.lhs : "") << ((!okL && !okR) ? " and " : "")
                      << (!okR ? e.rhs : "") << "\n";
        } else {
            val = (e.op == EquEntry::Op::ADD) ? (L + R) : (L - R);
        }

        AsmPool::equs[e.dst] = val;
        SymTab::add_definition(e.dst, "", static_cast<int>(val));
    }

    AsmPool::pending_equs.clear();
}

void AsmPool::check_weaks() {
    for (const auto& w : SymTab::weaks) {
        auto it = SymTab::table.find(w);
        if (it == SymTab::table.end()) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
        auto* def = it->second;
        bool hasDef = (def->line != -1);
        bool hasOcc = !def->occurences.empty();
        if (!hasDef && !hasOcc) {
            std::cerr << "ERROR: .weak symbol '" << w << "' was declared but never defined nor referenced in this module.\n";
            exit(1);
        }
    }
}

void AsmPool::clear() {
    pool_items().clear();
}
