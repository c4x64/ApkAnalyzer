#include "Exporter.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;

std::string Exporter::getBasePath(const std::string& package) {
#ifdef __ANDROID__
    return "/storage/emulated/0/Android/data/" + package + "/files/ExportedOffsets/";
#else
    return "ExportedOffsets/" + package + "/";
#endif
}

std::string getInsight(const std::string& name) {
    if (name.find("Update") != std::string::npos) return "Likely a frame-based update loop.";
    if (name.find("Init") != std::string::npos) return "Initialization logic.";
    if (name.find("Decrypt") != std::string::npos || name.find("Crypto") != std::string::npos) return "Involved in encryption/decryption.";
    if (name.find("Network") != std::string::npos || name.find("Send") != std::string::npos) return "Handles network communication.";
    if (name.find("Player") != std::string::npos) return "Related to player entity/stats.";
    if (name.find("Health") != std::string::npos) return "Related to health/vitality mechanics.";
    return "General method.";
}

bool Exporter::exportOffsets(const std::string& package, const std::string& libName, const std::vector<ElfSymbol>& symbols) {
    std::string path = getBasePath(package);
    fs::create_directories(path);
    
    std::string filename = "Export" + libName + ".h";
    if (filename.find("/") != std::string::npos) filename = filename.substr(filename.find_last_of("/") + 1);

    std::ofstream out(path + filename);
    if (!out.is_open()) return false;

    out << "// Auto-generated Offsets for " << libName << "\n";
    out << "// Package: " << package << "\n\n";
    out << "#pragma once\n\n";
    out << "namespace " << libName << "_Offsets {\n";
    
    for (const auto& sym : symbols) {
        if (sym.address != 0 && !sym.name.empty()) {
            out << "    // Insight: " << getInsight(sym.name) << "\n";
            out << "    uintptr_t " << sym.name << " = 0x" << std::hex << sym.address << ";\n";
        }
    }
    
    out << "}\n";
    return true;
}

bool Exporter::exportDex(const std::string& package, const std::vector<DexMethod>& methods) {
    std::string path = getBasePath(package);
    fs::create_directories(path);
    
    std::ofstream out(path + "ExportDex.h");
    if (!out.is_open()) return false;

    out << "#pragma once\n\nnamespace DexOffsets {\n";
    for (const auto& m : methods) {
        std::string cleanName = m.className + "_" + m.methodName;
        // Clean up name for C++ variable
        for (char &c : cleanName) if (c == '.' || c == '/' || c == '$' || c == ';') c = '_';
        
        out << "    // " << m.className << "->" << m.methodName << "\n";
        out << "    uint32_t " << cleanName << "_Idx = " << std::dec << m.methodIdx << ";\n";
    }
    out << "}\n";
    return true;
}
