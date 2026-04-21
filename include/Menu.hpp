#pragma once
#include <string>
#include <vector>
#include "imgui.h"
#include "ElfParser.hpp"
#include "DexParser.hpp"

class Menu {
public:
    static void draw(bool* p_open);
    static void setSymbols(const std::string& libName, const std::vector<ElfSymbol>& syms);
    static void setMethods(const std::vector<DexMethod>& meths);

private:
    static std::map<std::string, std::vector<ElfSymbol>> symbolsMap;
    static std::vector<DexMethod> methods;
};
