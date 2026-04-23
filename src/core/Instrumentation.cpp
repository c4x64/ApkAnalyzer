#include "Instrumentation.hpp"
#include "Logger.hpp"
#include <windows.h>
#include <psapi.h>

namespace Instrumentation {
    bool placeHook(uintptr_t target, void* replacement, void** original) {
        DWORD oldProtect;
        if (!VirtualProtect((void*)target, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

        uint8_t* patch = (uint8_t*)target;
        patch[0] = 0xE9;
        *(int32_t*)(patch + 1) = (int32_t)((uintptr_t)replacement - target - 5);

        VirtualProtect((void*)target, 5, oldProtect, &oldProtect);
        return true;
    }

    // This is the function we call when a hook is triggered
    void __stdcall DecryptionTriggerHook(uintptr_t base, size_t size) {
        Logger::log(Logger::SUCCESS, "Decryption routine triggered! Dumping memory range...");
        // Here we would implement the memory dumping logic
        // This keeps it 100% self-contained within this app
    }

    void installDecryptionWatchdog(uintptr_t libBase, size_t libSize) {
        Logger::log(Logger::INFO, "Instrumentation engine active. Monitoring for decryption triggers...");
        
        // Scan the library memory for common decryption prologue patterns
        // We look for patterns typical of custom library loaders/decryptors
        uint8_t* scanPtr = (uint8_t*)libBase;
        for (size_t i = 0; i < libSize - 4; ++i) {
            // Looking for ARM/X64 function prologues or custom decryption patterns
            // Example: XOR or ADD loops found earlier
            if (scanPtr[i] == 0x4A && scanPtr[i+1] == 0x00) { 
                placeHook((uintptr_t)(scanPtr + i), (void*)DecryptionTriggerHook, nullptr);
                Logger::log(Logger::INFO, "Hooked potential decryption routine at: 0x" + std::to_string(libBase + i));
                break;
            }
        }
    }
}
