#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class Section {
public:
    Section(std::string name) { this->name = name; }

    void add_instruction(unsigned char OC = 0, unsigned char MOD = 0, unsigned char RegA = 0,
                         unsigned char RegB = 0, unsigned char RegC = 0, signed short Disp = 0);
    void add_literal(const std::string& literal, int addend = 0, bool patchInPlace = false);
    void add_literal(int literal, bool patchInPlace = false);

    static Section* extract(const std::string& name);

    std::string getName() const { return this->name; }
    int getSize() const { return static_cast<int>(this->data.size()); }
    static std::unordered_map<std::string, Section*>& getSections() { return sections; }
    std::vector<unsigned char> &getData() { return this->data; }
    static void dumpPool();
    static void out(std::ostream &out);
    void insertInt(int number);
    void insertByte(int number);

private:
    struct literal {
        unsigned long line;
        long value;
        std::string symbol;
        int addend;
        bool patchInPlace;
        literal(unsigned long line, long value, const std::string& symbol, int addend = 0, bool patch = false)
            : line(line), value(value), symbol(symbol), addend(addend), patchInPlace(patch) {}
    };

    std::string name;
    std::vector<unsigned char> data; 
    std::vector<literal> pool;       

    static std::unordered_map<std::string, Section*> sections;
};
