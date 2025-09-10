#pragma once
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <ostream>

class Symbols {
public:
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

    void add_definition(const std::string& name, const std::string& section, int line);
    void add_occurrence(const std::string& name, const std::string& section, int line, bool inPool = true);

    const std::unordered_map<std::string, symDefinition>& get_table() const { return table; }
    std::unordered_map<std::string, symDefinition>& get_table() { return table; }
    const std::unordered_set<std::string>& get_weaks() const { return weaks; }
    const std::unordered_map<std::string, std::string>& get_types() const { return types; }

    void mark_global(const std::string& name) { globals.insert(name); }
    void mark_weak(const std::string& name) { weaks.insert(name); }
    void set_type(const std::string& name, const std::string& type) { types[name] = type; }

    bool is_global(const std::string& name) const { return globals.find(name) != globals.end(); }
    bool is_weak(const std::string& name) const { return weaks.find(name) != weaks.end(); }

    void out(std::ostream& os) const;
    void clear();

private:
    std::unordered_map<std::string, symDefinition> table;
    std::unordered_set<std::string> globals;
    std::unordered_set<std::string> weaks;
    std::unordered_map<std::string, std::string> types;
};
