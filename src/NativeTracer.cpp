#include "NativeTracer.hpp"
#include "Instrumentation.hpp"
#include "Logger.hpp"
#include <chrono>

std::vector<TraceEvent>& NativeTracer::getEvents() {
    static std::vector<TraceEvent> events;
    return events;
}

void NativeTracer::startTrace(const std::string& libName, uintptr_t offset) {
    Logger::log(Logger::INFO, "Registering native trace hook for: " + libName);
    
    // Auto-register with the instrumentation watchdog
    Instrumentation::installDecryptionWatchdog(offset, 0x1000); // Scans 4KB from offset

    TraceEvent e;
    e.functionName = libName + " + 0x" + std::to_string(offset);
    e.address = offset;
    e.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    getEvents().push_back(e);
}

std::vector<TraceEvent> NativeTracer::getTraceLog() {
    return getEvents();
}
