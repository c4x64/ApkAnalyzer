cmake_minimum_required(VERSION 3.10)
project(ApkAnalyzer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui)
include_directories(${PROJECT_SOURCE_DIR}/third_party/imgui/backends)

# Core Source files
set(CORE_SOURCES
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
    src/core/StringDecryptor.cpp
    src/core/FileMapper.cpp
    src/core/DexAnalyzer.cpp
    src/core/Instrumentation.cpp
    src/utils/EntropyCalculator.cpp
    src/utils/Utils.cpp
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_tables.cpp
)

# Core Library
add_library(ApkAnalyzerCore STATIC ${CORE_SOURCES})

# CLI Executable
add_executable(apkanalyzer_cli src/main.cpp)
target_link_libraries(apkanalyzer_cli ApkAnalyzerCore)

# Test Executable
add_executable(test_entropy tests/test_entropy.cpp)
target_link_libraries(test_entropy ApkAnalyzerCore)

# GUI Executable (if applicable)
if(WIN32)
    set(GUI_SOURCES
        src/gui_main.cpp
        third_party/imgui/backends/imgui_impl_win32.cpp
        third_party/imgui/backends/imgui_impl_dx11.cpp
    )
    add_executable(apkanalyzer_gui WIN32 ${GUI_SOURCES})
    target_link_libraries(apkanalyzer_gui ApkAnalyzerCore d3d11 d3dcompiler dwmapi)
endif()
