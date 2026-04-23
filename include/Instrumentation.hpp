#pragma once
#include <vector>
#include <cstdint>
#include <string>

// Lightweight instrumentation engine for native hooking
namespace Instrumentation {
    // Hook a function at a specific address in memory
    bool placeHook(uintptr_t target, void* replacement, void** original);
    
    // Scan for decryption routines and hook them
    void installDecryptionWatchdog(uintptr_t libBase, size_t libSize);
}
