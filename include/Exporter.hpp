#pragma once
#include <string>
#include <vector>
#include <map>
#include "ElfParser.hpp"
#include "DexParser.hpp"

class Exporter {
public:
    static bool exportOffsets(const std::string& package, const std::string& libName, const std::vector<ElfSymbol>& symbols);
    static bool exportDex(const std::string& package, const std::vector<DexMethod>& methods);

private:
    static std::string getBasePath(const std::string& package);
};
