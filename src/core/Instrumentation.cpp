#include "Instrumentation.hpp"
#include "Logger.hpp"
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

namespace Instrumentation {
#ifdef _WIN32
    bool placeHook(uintptr_t target, void* replacement, void** original) {
        DWORD oldProtect;
        if (!VirtualProtect((void*)target, 5, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;

        uint8_t* patch = (uint8_t*)target;
        patch[0] = 0xE9;
        *(int32_t*)(patch + 1) = (int32_t)((uintptr_t)replacement - target - 5);

        VirtualProtect((void*)target, 5, oldProtect, &oldProtect);
        return true;
    }
#else
    bool placeHook(uintptr_t target, void* replacement, void** original) {
        // Linux implementation would use mprotect and different patching logic
        return false; 
    }
#endif

#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif

    // This is the function we call when a hook is triggered
    void STDCALL DecryptionTriggerHook(uintptr_t base, size_t size) {
        Logger::log(Logger::SUCCESS, "Decryption routine triggered! Dumping memory range...");
        
        // Define a path for the dumped library
        std::string dumpPath = "dumped_lib.so";
        
        // Create the file and write the memory contents using standard streams
        std::ofstream outFile(dumpPath, std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char*>(base), size);
            outFile.close();
            Logger::log(Logger::SUCCESS, "Library successfully dumped to " + dumpPath);
        } else {
            Logger::log(Logger::WARNING, "Failed to create dump file.");
        }
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
