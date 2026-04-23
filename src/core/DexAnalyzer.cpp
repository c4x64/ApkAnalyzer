#include "DexParser.hpp"
#include <vector>

// Helper to quickly scan for class/method obfuscation patterns
namespace DexAnalyzer {
    // Detects common ProGuard/R8 obfuscation: Short class names (e.g. 'a', 'b')
    bool isObfuscatedClass(const std::string& name) {
        if (name.length() <= 2) return true;
        return false;
    }

    // Heuristic: Method code density check for packed/encrypted code
    bool isPackedMethod(const std::vector<DexInstruction>& insts) {
        if (insts.size() < 4) return true;
        // If all instructions are 'nop' or 'unknown' (simplified), it might be packed
        return false;
    }
}
