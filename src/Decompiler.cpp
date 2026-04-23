#include "DexParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Pattern for a basic Dalvik invoke-virtual instruction
// This is a naive implementation; a full decompiler would parse the opcode and registers
std::string decompileInstruction(const DexInstruction& inst) {
    if (inst.opcode.find("invoke") != std::string::npos) {
        return "    // Call method (Native/Java) -> " + inst.details;
    }
    if (inst.opcode == "move") return "    " + inst.details + ";";
    if (inst.opcode == "return") return "    return " + inst.details + ";";
    if (inst.opcode.find("const") != std::string::npos) return "    // Const assignment: " + inst.details;
    return "    // " + inst.opcode + " " + inst.details;
}

std::string DexParser::decompileMethod(uint32_t methodIdx) {
    std::vector<DexInstruction> insts = disassembleMethod(methodIdx);
    std::stringstream ss;
    ss << "/* Decompiled Method Index: " << methodIdx << " */\n";
    for (const auto& inst : insts) {
        ss << decompileInstruction(inst) << "\n";
    }
    return ss.str();
}
