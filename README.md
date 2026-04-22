<div align="center">

```
 █████╗ ██████╗ ██╗  ██╗ █████╗ ███╗   ██╗ █████╗ ██╗  ██╗   ██╗███████╗███████╗██████╗
██╔══██╗██╔══██╗██║ ██╔╝██╔══██╗████╗  ██║██╔══██╗██║  ╚██╗ ██╔╝╚══███╔╝██╔════╝██╔══██╗
███████║██████╔╝█████╔╝ ███████║██╔██╗ ██║███████║██║   ╚████╔╝   ███╔╝ █████╗  ██████╔╝
██╔══██║██╔═══╝ ██╔═██╗ ██╔══██║██║╚██╗██║██╔══██║██║    ╚██╔╝   ███╔╝  ██╔══╝  ██╔══██╗
██║  ██║██║     ██║  ██╗██║  ██║██║ ╚████║██║  ██║███████╗██║   ███████╗███████╗██║  ██║
╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚═╝   ╚══════╝╚══════╝╚═╝  ╚═╝
```

**High-performance, cross-platform engine for APK analysis, native symbol extraction, Java method introspection, and IL2CPP metadata reconstruction.**

<br/>

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=flat-square&logo=github-actions&logoColor=white)](https://github.com/c4x64/ApkAnalyzer)
[![Version](https://img.shields.io/badge/version-v1.0.0--stable-blue?style=flat-square)](https://github.com/c4x64/ApkAnalyzer/releases)
[![License](https://img.shields.io/badge/license-MIT-orange?style=flat-square)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20Android-informational?style=flat-square&logo=android&logoColor=white)](https://github.com/c4x64/ApkAnalyzer)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-red?style=flat-square&logo=cplusplus&logoColor=white)](https://github.com/c4x64/ApkAnalyzer)
[![Stars](https://img.shields.io/github/stars/c4x64/ApkAnalyzer?style=flat-square&color=yellow)](https://github.com/c4x64/ApkAnalyzer/stargazers)

</div>

---

## ✦ Overview

**ApkAnalyzer** is a hardened, low-level APK inspection tool built for reverse engineers, security researchers, and game modders who require precision introspection of Android application internals. It directly parses ELF binaries, DEX bytecode, and IL2CPP metadata without relying on heavyweight runtimes or fragile third-party pipelines.

It is not a toy. It is not a GUI wrapper. It is a purpose-built analysis engine.

---

## ⚡ Feature Matrix

<table>
<tr>
<td width="33%" valign="top">

### 🔬 ELF Symbol Dumper
Deep-parses `.so` shared libraries to extract all exported and internal native symbols. Supports stripped binaries with heuristic fallback recovery.

</td>
<td width="33%" valign="top">

### 📦 DEX Method Dumper
Enumerates all classes, methods, fields, and access flags from Dalvik Executable (`.dex`) files. Reconstructs human-readable method signatures.

</td>
<td width="33%" valign="top">

### 🧩 IL2CPP Metadata Dumper
Reconstructs managed code structure from compiled IL2CPP `global-metadata.dat` and `libil2cpp.so`. Targets Unity-built mobile games with full type/method mapping.

</td>
</tr>
<tr>
<td width="33%" valign="top">

### 🎯 Pattern Scanner
Signature-based byte pattern scanning with obfuscation-aware logic. Includes pre-built heuristics for heavily obfuscated targets such as **Garena Free Fire**.

</td>
<td width="33%" valign="top">

### 🖥️ ImGui Search Interface
Embedded Dear ImGui-powered interactive search UI for navigating dumped symbols, methods, and offsets in real-time without leaving the tool.

</td>
<td width="33%" valign="top">

### 📄 C++ Header Export
Generates `.h` header files from recovered symbols and IL2CPP type data — drop-in-ready for use in hook frameworks and mod SDKs.

</td>
</tr>
</table>

---

## 🗂️ Technical Specifications

| Parameter | Value |
|---|---|
| **Language Standard** | C++17 |
| **Build System** | CMake ≥ 3.16 |
| **UI Backend** | Dear ImGui (docking branch) |
| **Compression** | miniz (zlib-compatible) |
| **Architecture Support** | `ARM64-v8a` · `ARMv7` · `x86_64` · `x86` |
| **Target Frameworks** | Unity IL2CPP · Android Native (NDK) · Dalvik/ART |
| **Supported Input** | `.apk` · `.so` · `.dex` · `global-metadata.dat` |
| **Output Formats** | `.h` headers · `.txt` dumps · raw offset tables |
| **Host Platforms** | Windows 10/11 · Linux (glibc ≥ 2.17) · Android (CLI) |
| **Binary Size** | < 8 MB (single executable, no install required) |
| **Dependency Model** | Self-contained — no JVM, no Python, no ADB required |

---

## 🚀 Quick Start

### 1 — Clone & Build

```bash
# Clone the repository
git clone https://github.com/c4x64/ApkAnalyzer.git
cd ApkAnalyzer

# Configure with CMake (Release mode recommended)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build build --config Release -j$(nproc)
```

### 2 — Run: APK Analysis

```bash
# Full analysis of an APK — dumps ELF symbols, DEX methods, IL2CPP metadata
./ApkAnalyzer --input target.apk --all --output ./dump/

# Output structure:
# ./dump/
# ├── symbols.txt       ← ELF native symbols
# ├── methods.txt       ← DEX method signatures
# ├── il2cpp.h          ← Reconstructed C++ headers
# └── offsets.txt       ← Pattern scan results
```

### 3 — Run: Targeted Extraction

```bash
# ELF symbol extraction only (from extracted .so)
./ApkAnalyzer --input libil2cpp.so --elf-dump

# DEX method enumeration
./ApkAnalyzer --input classes.dex --dex-dump

# IL2CPP full reconstruction
./ApkAnalyzer --input libil2cpp.so --metadata global-metadata.dat --il2cpp-dump --export-headers
```

### 4 — Run: Pattern Scanning

```bash
# Scan with a custom byte signature
./ApkAnalyzer --input libil2cpp.so --scan "48 8B 05 ?? ?? ?? ?? 48 85 C0"

# Use built-in Free Fire obfuscation profile
./ApkAnalyzer --input libil2cpp.so --profile freefire --scan-all
```

---

<details>
<summary><strong>🔩 Advanced Usage — Click to Expand</strong></summary>

<br/>

### CLI Flag Reference

| Flag | Type | Description |
|---|---|---|
| `--input <path>` | `string` | Primary input file (`.apk`, `.so`, `.dex`) |
| `--metadata <path>` | `string` | Path to `global-metadata.dat` for IL2CPP mode |
| `--output <dir>` | `string` | Output directory for all generated files |
| `--all` | `bool` | Run all analysis passes simultaneously |
| `--elf-dump` | `bool` | Enable ELF symbol extraction |
| `--dex-dump` | `bool` | Enable DEX method enumeration |
| `--il2cpp-dump` | `bool` | Enable IL2CPP metadata reconstruction |
| `--export-headers` | `bool` | Generate C++ `.h` header output |
| `--scan <pattern>` | `string` | Byte pattern (hex, `??` wildcard supported) |
| `--profile <name>` | `string` | Load obfuscation profile (`freefire`, `pubg`, `generic`) |
| `--scan-all` | `bool` | Run all pattern scans in the active profile |
| `--gui` | `bool` | Launch ImGui interactive search interface |
| `--verbose` | `bool` | Enable verbose diagnostic output |

---

### Batch Processing via CMake Script

```cmake
# CMakeLists.txt — custom integration target example
add_custom_target(analyze_apk
    COMMAND $<TARGET_FILE:ApkAnalyzer>
        --input ${APK_PATH}
        --all
        --output ${CMAKE_BINARY_DIR}/analysis
    DEPENDS ApkAnalyzer
    COMMENT "Running ApkAnalyzer on target APK..."
)
```

---

### Android CLI Deployment (via ADB)

```bash
# Push binary to device (root shell required for most targets)
adb push ./build/ApkAnalyzer /data/local/tmp/
adb shell chmod +x /data/local/tmp/ApkAnalyzer

# Execute on-device
adb shell "/data/local/tmp/ApkAnalyzer --input /data/app/com.target.pkg/lib/arm64/libil2cpp.so --elf-dump"

# Pull results
adb pull /data/local/tmp/dump/ ./results/
```

</details>

---

<details>
<summary><strong>🧠 Heuristic Logic & Obfuscation Engine — Click to Expand</strong></summary>

<br/>

### Obfuscation Profiles

ApkAnalyzer ships with a heuristic engine capable of defeating common obfuscation strategies encountered in commercial mobile games.

**Profile: `freefire`**
- Targets Garena Free Fire's obfuscated `libil2cpp.so`
- Applies entropy-based section detection to recover stripped symbol tables
- Uses prologue fingerprinting (function entry byte sequences) to recover method boundaries
- Cross-references recovered offsets against DEX class structure for validation

**Profile: `generic`**
- Class-name entropy scoring to detect ProGuard/R8-minified DEX files
- String reference graphing to anchor renamed classes to semantic meaning
- Offset continuity analysis for split/relocated symbol tables

### Pattern Scan Engine

The pattern scanner operates at the byte level with the following capabilities:

```
Wildcard support : ?? (single-byte wildcard, arbitrary count)
Alignment hints  : Patterns can be anchored to 4-byte or 8-byte boundaries
SIMD acceleration: Uses SSE4.2 / NEON intrinsics on supported hosts
Multi-match mode : Returns all occurrences, not just the first hit
Result scoring   : Ranks matches by surrounding code heuristics
```

### IL2CPP Reconstruction Pipeline

```
[libil2cpp.so]         [global-metadata.dat]
      │                         │
      ▼                         ▼
 ELF Parser              Metadata Parser
      │                         │
      └──────────┬──────────────┘
                 ▼
         Cross-Reference Engine
                 │
         ┌───────┴────────┐
         ▼                ▼
   Type Resolver    Method Mapper
         │                │
         └───────┬─────────┘
                 ▼
         C++ Header Generator
                 │
                 ▼
           output.h  ←  Drop-in ready for Dobby / LSPlant / KittyMemory
```

</details>

---

## 🏗️ Project Architecture

```
ApkAnalyzer/
├── src/
│   ├── core/
│   │   ├── apk_parser.cpp        ← ZIP/APK extraction layer
│   │   ├── elf_dumper.cpp        ← ELF symbol extraction
│   │   ├── dex_dumper.cpp        ← DEX method enumeration
│   │   └── il2cpp_dumper.cpp     ← IL2CPP metadata reconstruction
│   ├── scanner/
│   │   ├── pattern_scanner.cpp   ← Byte-pattern engine (SIMD)
│   │   └── profiles/             ← Obfuscation heuristic profiles
│   ├── export/
│   │   └── header_export.cpp     ← C++ .h file generator
│   └── ui/
│       └── imgui_frontend.cpp    ← Dear ImGui search interface
├── include/
├── vendor/
│   ├── imgui/                    ← Dear ImGui (docking)
│   └── miniz/                    ← miniz compression
├── CMakeLists.txt
└── README.md
```

---

## 📦 Dependencies

| Library | Role | License |
|---|---|---|
| [Dear ImGui](https://github.com/ocornut/imgui) | Interactive search UI | MIT |
| [miniz](https://github.com/richgel999/miniz) | APK (ZIP) decompression | MIT / Unlicense |
| [Il2CppDumper](https://github.com/Perfare/Il2CppDumper) | IL2CPP research reference | MIT |

> All dependencies are vendored. No package manager required.

---

## 🙏 Credits & Acknowledgements

<table>
<tr>
<td align="center" width="33%">

**[Perfare](https://github.com/Perfare)**
<br/>
*Il2CppDumper* — foundational IL2CPP research and metadata format documentation that made this work possible.

</td>
<td align="center" width="33%">

**[ocornut](https://github.com/ocornut)**
<br/>
*Dear ImGui* — the bloat-free, immediate-mode GUI library powering the interactive search interface.

</td>
<td align="center" width="33%">

**[richgel999](https://github.com/richgel999)**
<br/>
*miniz* — single-file, high-performance zlib/deflate implementation enabling dependency-free APK decompression.

</td>
</tr>
</table>

---

## ⚖️ Legal Disclaimer

This tool is intended exclusively for **authorized security research, personal modding, and educational analysis** of software you own or have explicit permission to inspect. The authors do not condone or support any use of this tool in violation of applicable laws, terms of service, or intellectual property rights. **Use responsibly.**

---

<div align="center">

### If ApkAnalyzer saved you time, consider leaving a ⭐

*Every star helps the project stay visible and signals continued maintenance.*

[![Star on GitHub](https://img.shields.io/badge/★%20Star%20this%20repo-ApkAnalyzer-yellow?style=for-the-badge&logo=github)](https://github.com/c4x64/ApkAnalyzer)

<br/>

**Built by [c4x64](https://github.com/c4x64)** · [ctlang.netlify.app](https://ctlang.netlify.app)

<sub>Released under the MIT License · No warranty implied · Reverse engineering is not a crime</sub>

</div>
