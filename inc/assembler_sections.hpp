#pragma once
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <ostream>

class Section {
public:
    explicit Section(std::string name) : name(std::move(name)) {}

    [[nodiscard]] std::string getName() const { return name; }
    std::vector<unsigned char>& getData() { return data; }
    [[nodiscard]] const std::vector<unsigned char>& getData() const { return data; }
    [[nodiscard]] int getSize() const { return static_cast<int>(data.size()); }

    void add_instruction(unsigned char OC = 0, unsigned char MOD = 0,
                         unsigned char RegA = 0, unsigned char RegB = 0, unsigned char RegC = 0,
                         signed short Disp = 0);
    void insertInt(int number);
    void insertByte(int number);

private:
    std::string name;
    std::vector<unsigned char> data;
};

class Sections {
public:
    Sections() = default;
    Sections(const Sections&) = delete;
    Sections& operator=(const Sections&) = delete;

    Section* extract(const std::string& name);

    void out(std::ostream& out) const;

    void clear() { order.clear(); map.clear(); }
    bool has(const std::string& name) const { return map.find(name) != map.end(); }

private:
    std::unordered_map<std::string, std::unique_ptr<Section>> map;
    std::vector<std::string> order;
};
