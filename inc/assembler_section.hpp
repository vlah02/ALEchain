#pragma once
#include <vector>
#include <string>
#include <unordered_map>

struct RelocationEntry {
    std::string section;    
    int offset;             
    std::string type;       
    std::string symbol;     
    int addend;             
    RelocationEntry(const std::string& section, int offset, const std::string& type,
                    const std::string& symbol, int addend = 0)
        : section(section), offset(offset), type(type), symbol(symbol), addend(addend) {}
};

class Section {
public:
    Section(std::string name) { this->name = name; }

    void add_instruction(unsigned char OC = 0, unsigned char MOD = 0, unsigned char RegA = 0,
                         unsigned char RegB = 0, unsigned char RegC = 0, signed short Disp = 0);
    void add_literal(const std::string& literal, int addend = 0);
    void add_literal(int literal);

    static Section* extract(const std::string& name);

    std::string getName() const { return this->name; }
    int getSize() const { return static_cast<int>(this->data.size()); }
    static std::unordered_map<std::string, Section*>& getSections() { return sections; }
    std::vector<unsigned char> &getData() { return this->data; }
    static void dumpPool();
    static void out(std::ostream &out);
    static void outRelocations(std::ostream& out);
    void insertInt(int number);
    void insertByte(int number);

    static std::vector<RelocationEntry> relocations;

private:
    struct literal {
        unsigned long line;
        long value;
        std::string symbol;
        int addend;
        literal(unsigned long line, long value, const std::string& symbol, int addend = 0)
            : line(line), value(value), symbol(symbol), addend(addend) {}
    };

    std::string name;
    std::vector<unsigned char> data; 
    std::vector<literal> pool;       

    static std::unordered_map<std::string, Section*> sections;
};
