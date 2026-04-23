cmake_minimum_required(VERSION 3.10)
project(ApkAnalyzer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui/backends)

# Source files
set(SOURCES
    src/main.cpp
    src/ZipUtility.cpp
    src/ElfParser.cpp
    src/DexParser.cpp
    src/Il2CppScanner.cpp
    src/ManifestParser.cpp
    src/Exporter.cpp
    src/Menu.cpp
    src/MemoryUtils.cpp
    src/MemoryScanner.cpp
    src/PatternScanner.cpp
    src/Deobfuscator.cpp
    src/Decompiler.cpp
    src/NativeTracer.cpp
    src/Logger.cpp
    src/core/Decoder.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
)

set(GUI_SOURCES
    ${SOURCES}
    src/gui_main.cpp
    third_party/imgui/backends/imgui_impl_win32.cpp
    third_party/imgui/backends/imgui_impl_dx11.cpp
)

# Header files
set(HEADERS
    include/ZipUtility.hpp
    include/ElfParser.hpp
    include/DexParser.hpp
    include/Il2CppScanner.hpp
    include/ManifestParser.hpp
    include/Exporter.hpp
    include/Menu.hpp
)

# Shared Library (for Android .so)
add_library(apkanalyzer_lib SHARED ${SOURCES})
set_target_properties(apkanalyzer_lib PROPERTIES OUTPUT_NAME "apkcore")

# CLI Executable
add_executable(apkanalyzer_cli src/main.cpp ${SOURCES})
set_target_properties(apkanalyzer_cli PROPERTIES OUTPUT_NAME "apk_offset_gen")

# Windows GUI (if on Windows)
if(WIN32)
    add_executable(apkanalyzer_gui WIN32 ${GUI_SOURCES})
    target_link_libraries(apkanalyzer_gui d3d11 d3dcompiler dwmapi)
endif()
