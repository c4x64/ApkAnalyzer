#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct TraceEvent {
    std::string functionName;
    uintptr_t address;
    uint64_t timestamp;
};

class NativeTracer {
public:
    static void startTrace(const std::string& libName, uintptr_t offset);
    static std::vector<TraceEvent> getTraceLog();
private:
    static std::vector<TraceEvent>& getEvents();
};
