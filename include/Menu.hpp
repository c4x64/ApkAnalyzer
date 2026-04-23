#pragma once
#include <string>
#include <vector>
#include <map>
#include "imgui.h"
#include "ElfParser.hpp"
#include "DexParser.hpp"

class Menu {
public:
    static void draw(bool* p_open);
    static void SetupTheme();
    static void setSymbols(const std::string& libName, const std::vector<ElfSymbol>& syms);
    static void setMethods(const std::vector<DexMethod>& meths);
    static void setFields(const std::vector<DexField>& flds);
    static void setClasses(const std::vector<DexClass>& clss);
    static void setBytecode(const std::string& methodName, const std::vector<DexInstruction>& insts);
    static std::vector<uint8_t>& getCurrentDexData();
    static std::map<std::string, std::vector<ElfSymbol>>& getSymbolsMap();
    static void clearData();

private:
    static std::vector<DexMethod>& getMethods();
    static std::vector<DexField>& getFields();
    static std::vector<DexClass>& getClasses();
    static std::vector<DexInstruction>& getBytecode();
    static std::string& getCurrentMethodName();
};
