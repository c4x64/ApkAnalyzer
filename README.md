# ApkAnalyzer 🚀

A high-performance, cross-platform tool for analyzing APK files, extracting native symbols, Java methods, and dumping IL2CPP metadata. Designed for game researchers and reverse engineers.

## ✨ Features

-   **Cross-Platform**: Windows (GUI/CLI), Linux, and Android (`.so`).
-   **ELF Symbol Dumper**: Extract symbols and RVAs from native `.so` libraries.
-   **DEX Method Dumper**: Reconstruct Java method signatures, return types, and indices.
-   **IL2CPP Dumper**: Dumper-style scanning for `global-metadata.dat` and `libil2cpp.so`.
-   **Search & Filter**: Real-time searchable ImGui interface for large codebases.
-   **Automatic Exporter**: Generates C++ header files (`ExportlibGGP.h`, etc.) with offsets and heuristics-based "insights".
-   **Encrypted Lib Support**: Pattern-based scanning to find registration functions even when symbols are stripped or obfuscated (e.g., FreeFire).

## 🛠️ Components

| Component | Description |
| :--- | :--- |
| `apk_offset_gen.exe` | CLI for batch processing and quick searches. |
| `apkanalyzer_gui.exe` | ImGui-powered Windows GUI with File browser and search. |
| `libapkcore.so` | Shared library for Android. Includes an in-game menu and auto-export logic. |

## 🚀 Usage

### CLI
```bash
# List all native symbols
apk_offset_gen.exe path/to/game.apk --libs

# List all Java methods
apk_offset_gen.exe path/to/game.apk --dex

# Find a specific method (e.g., getScatter)
apk_offset_gen.exe path/to/game.apk --find getScatter

# Export all offsets to the game's data folder
apk_offset_gen.exe path/to/game.apk --export com.game.package
```

### GUI
1. Run `apkanalyzer_gui.exe`.
2. Go to **File -> Open APK**.
3. Browse symbols and methods using the **Search** bars.

## 📦 Building

### GitHub Actions
Every push to `main` or a new `v*` tag automatically builds and releases binaries for Windows and Linux. Check the **Releases** tab.

### Local Build (CMake)
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## 🤝 Credits
-   **Perfare**: Pattern scanning logic inspired by Il2CppDumper.
-   **ocornut**: ImGui interface.
-   **richgel99**: miniz for ZIP handling.

---
Created by **c4x64**
