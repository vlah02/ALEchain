#pragma once
#include <vector>
#include <string>
#include <unordered_map>

class Section {
public:
    explicit Section(const std::string& name) { this->name = name; }

    [[nodiscard]] std::string getName() const { return this->name; }
    std::vector<unsigned char> &getData() { return this->data; }
    [[nodiscard]] int getSize() const { return static_cast<int>(this->data.size()); }
    static std::unordered_map<std::string, Section*>& getSections() { return sections; }

    static Section* extract(const std::string& name);

    void add_instruction(unsigned char OC = 0, unsigned char MOD = 0,
                        unsigned char RegA = 0, unsigned char RegB = 0, unsigned char RegC = 0,
                        signed short Disp = 0);
    void insertInt(int number);
    void insertByte(int number);

    static void out(std::ostream &out);

private:
    std::string name;
    std::vector<unsigned char> data;

    static std::unordered_map<std::string, Section*> sections;
};
