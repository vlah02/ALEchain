#pragma once
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <vector>

class SymTab {
public:
    static void add_definition(const std::string& name, const std::string& section, int line);
    static void add_occurrence(const std::string& name, const std::string& section, int line, bool inPool = true);
    static void add_type(const std::string& symbol, const std::string& type);
    static void out(std::ostream& os);

    struct symOccurrence {
        std::string section;
        int line;
        bool inPool;
    };
    struct symDefinition {
        std::string section;
        int line = -1;
        std::vector<symOccurrence> occurences;
    };

    static std::unordered_map<std::string, symDefinition*> table;
    static std::unordered_set<std::string> globals;
    static std::unordered_set<std::string> weaks;
    static std::unordered_map<std::string, std::string> types;
};
