#include "NativeTracer.hpp"
#include <chrono>

std::vector<TraceEvent>& NativeTracer::getEvents() {
    static std::vector<TraceEvent> events;
    return events;
}

void NativeTracer::startTrace(const std::string& libName, uintptr_t offset) {
    TraceEvent e;
    e.functionName = libName + " + 0x" + std::to_string(offset);
    e.address = offset;
    e.timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    getEvents().push_back(e);
}

std::vector<TraceEvent> NativeTracer::getTraceLog() {
    return getEvents();
}
