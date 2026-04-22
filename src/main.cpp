#include <iostream>
#include <vector>
#include <string>
#include "ZipUtility.hpp"
#include "ElfParser.hpp"
#include "DexParser.hpp"
#include "Exporter.hpp"
#include "Menu.hpp"
#include "Logger.hpp"

void printHelp() {
    std::cout << "Usage: apk_offset_gen <apk_path> [options]\n";
    std::cout << "Options:\n";
    std::cout << "  --libs          List all .so libraries and their symbols\n";
    std::cout << "  --dex           List all methods in classes.dex\n";
    std::cout << "  --find <name>   Find address/index of a specific symbol/method\n";
    std::cout << "  --export <pkg>  Export offsets to ExportedOffsets/<pkg>/ directory\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    std::string apkPath = argv[1];
    std::string mode = (argc > 2) ? argv[2] : "";
    std::string pkgName = "com.default.game";

    if (mode == "--export" && argc > 3) {
        pkgName = argv[3];
    }

    Logger::log(Logger::INFO, "Analyzing APK: " + apkPath);

    auto files = ZipUtility::listFiles(apkPath);
    if (files.empty()) {
        Logger::log(Logger::WARNING, "Could not read APK or APK is empty.");
        return 1;
    }

    if (mode == "--libs" || mode == "--export") {
        for (const auto& file : files) {
            if (file.find(".so") != std::string::npos) {
                Logger::log(Logger::INFO, "Processing Library: " + file);
                auto data = ZipUtility::readFile(apkPath, file);
                ElfParser parser(data);
                if (parser.isValid()) {
                    auto symbols = parser.getSymbols();
                    // New: Attempt IL2CPP scan if libil2cpp detected
                    if (file.find("libil2cpp") != std::string::npos) {
                        Il2CppScanner scanner({}, data);
                        scanner.scanAllMethods(symbols);
                    }

                    if (mode == "--export") {
                        if (Exporter::exportOffsets(pkgName, file, symbols)) {
                            Logger::log(Logger::SUCCESS, "Exported to ExportedOffsets/" + pkgName + "/Export" + file + ".h");
                        }
                    } else {
                        for (const auto& sym : symbols) {
                             if (sym.info != 0) std::cout << "      0x" << std::hex << sym.address << " " << sym.name << std::endl;
                        }
                    }
                }
            }
        }
    }
    if (mode == "--dex" || mode == "--export") {
        for (const auto& file : files) {
            if (file.find(".dex") != std::string::npos) {
                std::cout << "\n[+] Processing DEX: " << file << std::endl;
                auto data = ZipUtility::readFile(apkPath, file);
                DexParser parser(data);
                if (parser.isValid()) {
                    auto methods = parser.getMethods();
                    if (mode == "--export") {
                        if (Exporter::exportDex(pkgName, methods)) {
                            std::cout << "    [V] Exported DEX methods to ExportedOffsets/" << pkgName << "/ExportDex.h" << std::endl;
                        }
                    } else {
                        for (size_t i = 0; i < std::min(methods.size(), (size_t)100); ++i) {
                             std::cout << "      [" << i << "] " << methods[i].className << "->" << methods[i].methodName << std::endl;
                        }
                    }
                }
            }
        }
    }
 else if (mode == "--find") {
        if (argc < 4) {
            std::cout << "Usage: apk_offset_gen <apk_path> --find <name>" << std::endl;
            return 1;
        }
        std::string target = argv[3];
        std::cout << "[*] Searching for: " << target << std::endl;
        
        for (const auto& file : files) {
            if (file.find(".dex") != std::string::npos) {
                auto data = ZipUtility::readFile(apkPath, file);
                DexParser parser(data);
                if (parser.isValid()) {
                    auto methods = parser.getMethods();
                    for (const auto& m : methods) {
                        if (m.methodName.find(target) != std::string::npos || m.className.find(target) != std::string::npos) {
                            std::cout << "[+] Found in DEX (" << file << "): " << m.className << "->" << m.methodName << " [Index: " << m.methodIdx << "]" << std::endl;
                        }
                    }
                }
            }
            if (file.find(".so") != std::string::npos) {
                auto data = ZipUtility::readFile(apkPath, file);
                ElfParser parser(data);
                if (parser.isValid()) {
                    auto symbols = parser.getSymbols();
                    for (const auto& sym : symbols) {
                        if (sym.name.find(target) != std::string::npos) {
                            std::cout << "[+] Found in LIB (" << file << "): " << sym.name << " [RVA: 0x" << std::hex << sym.address << "]" << std::endl;
                        }
                    }
                }
            }
        }
    } else {
        std::cout << "[*] APK contains " << files.size() << " files." << std::endl;
        std::cout << "[*] Use --libs or --dex for detailed analysis." << std::endl;
    }

    return 0;
}
