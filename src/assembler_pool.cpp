#include "../inc/assembler_pool.hpp"
#include "../inc/assembler_sections.hpp"
#include "../inc/assembler_symbols.hpp"
#include "../inc/assembler_relocations.hpp"

#include <limits>
#include <cstdio>
#include <iostream>
#include <cstdlib>

void Pool::enqueue_value(const std::string& sec, const int site, const int value, const bool patchInPlace) {
    items.push_back(PoolItem{sec, site, value, "", 0, patchInPlace});
}

void Pool::enqueue_symbol(const std::string& sec, const int site,
                             const std::string& sym, const int addend, const bool patchInPlace) {
    items.push_back(PoolItem{sec, site, -1, sym, addend, patchInPlace});
}

void Pool::add_literal(const std::string& section, const int site, const std::string& token) {
    std::string base = token;
    int addend = 0;
    if (const auto plus = base.find('+'); plus != std::string::npos) {
        addend = std::stoi(base.substr(plus + 1));
        base   = base.substr(0, plus);
    } else if (const auto minus = base.find('-'); minus != std::string::npos) {
        addend = -std::stoi(base.substr(minus + 1));
        base   = base.substr(0, minus);
    }

    if (const auto it = equs.find(base); it != equs.end()) {
        const long equVal = it->second + addend;
        enqueue_value(section, site, static_cast<int>(equVal), true);
        return;
    }

    enqueue_symbol(section, site, base, addend, false);
}

void Pool::flush(Sections& sections, Symbols& symbols, Relocations& relocs) {
    resolve_equs(symbols);

    for (auto& it : items) {
        Section* sec = sections.extract(it.section);
        auto& data = sec->getData();

        const int lineToReplace = it.site;
        const int literalLine   = static_cast<int>(data.size());
        const int displacement  = literalLine - lineToReplace - 4;

        data[lineToReplace + 3]  = displacement & 0xFF;
        data[lineToReplace + 2] |= (displacement >> 8) & 0x0F;

        if (!it.symbol.empty()) {
            int valueToInsert = 0;

            auto& tbl = symbols.get_table();
            auto symEntryIt = tbl.find(it.symbol);
            const bool localDefined = (symEntryIt != tbl.end() && symEntryIt->second.line != -1);

            if (localDefined) {
                valueToInsert = symEntryIt->second.line + it.addend;

                if (it.patchInPlace) {
                    data[lineToReplace + 2] = ((valueToInsert >> 8) & 0x0F) | (data[lineToReplace + 2] & 0xF0);
                    data[lineToReplace + 3] = (valueToInsert & 0xFF);
                }

                sec->insertInt(valueToInsert);
                std::fprintf(stderr, "[POOL] insertInt(%d) for LOCAL symbol=%s at section=%s, offset=%zu\n",
                             valueToInsert, it.symbol.c_str(), it.section.c_str(), data.size());
            } else {
                if (it.patchInPlace) {
                    data[lineToReplace + 2] &= 0xF0;
                    data[lineToReplace + 3]  = 0;
                }
                sec->insertInt(0);
                std::fprintf(stderr, "[POOL] insertInt(0) for EXTERNAL symbol=%s at section=%s, offset=%zu\n",
                             it.symbol.c_str(), it.section.c_str(), data.size());
            }

            relocs.add(it.section, literalLine, "R_ABS32", it.symbol, it.addend);

        } else {
            sec->insertInt(static_cast<int>(it.value));
        }
    }

    check_weaks(symbols);
}

void Pool::clear() {
    items.clear();
    pending_equs.clear();
    equs.clear();
}

void Pool::resolve_equs(Symbols& symbols) {
    auto lookup = [&](const std::string& sym, long& out) -> bool {
        if (const auto it = equs.find(sym); it != equs.end()) {
            out = it->second;
            return true;
        }
        const auto& tbl2 = symbols.get_table();
        if (const auto it = tbl2.find(sym); it != tbl2.end() && it->second.line != -1) {
            out = static_cast<long>(it->second.line);
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
        symbols.add_definition(e.dst, "", static_cast<int>(val));
    }

    pending_equs.clear();
}

void Pool::check_weaks(const Symbols& symbols) const {
    const auto& weaks = symbols.get_weaks();
    const auto& tbl = symbols.get_table();
    for (const auto& w : weaks) {
        auto it = tbl.find(w);
        if (it == tbl.end()) {
            std::cerr << "ERROR: .weak symbol '" << w
                      << "' was declared but never defined nor referenced in this module.\n";
            std::exit(1);
        }
        const auto& def = it->second;
        const bool hasDef = (def.line != -1);
        const bool hasOcc = !def.occurences.empty();
        if (!hasDef && !hasOcc) {
            std::cerr << "ERROR: .weak symbol '" << w
                      << "' was declared but never defined nor referenced in this module.\n";
            std::exit(1);
        }
    }
}
