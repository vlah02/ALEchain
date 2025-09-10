#include "../inc/assembler_pool.hpp"
#include "../inc/assembler_section.hpp"
#include "../inc/assembler_symtab.hpp"
#include "../inc/assembler_reloc.hpp"

#include <vector>
#include <limits>
#include <cstdio>

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

void AsmPool::flush() {
    SymTab::resolve_pending_equs();

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

    SymTab::validate_weaks_or_die();
}

void AsmPool::clear() {
    pool_items().clear();
}
