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
    void add_symbol_or_equ_literal(const std::string& sym);

    static Section* extract(const std::string& name);

    std::string getName() const { return this->name; }
    int getSize() const { return static_cast<int>(this->data.size()); }
    static std::unordered_map<std::string, Section*>& getSections() { return sections; }
    std::vector<unsigned char> &getData() { return this->data; }
    static void out(std::ostream &out);
    void insertInt(int number);
    void insertByte(int number);

private:
    std::string name;
    std::vector<unsigned char> data;

    static std::unordered_map<std::string, Section*> sections;
};
