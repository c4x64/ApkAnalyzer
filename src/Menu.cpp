#include "Menu.hpp"
#include <map>
#include <iomanip>
#include <sstream>

std::map<std::string, std::vector<ElfSymbol>> Menu::symbolsMap;
std::vector<DexMethod> Menu::methods;

void Menu::setSymbols(const std::string& libName, const std::vector<ElfSymbol>& syms) {
    symbolsMap[libName] = syms;
}

void Menu::setMethods(const std::vector<DexMethod>& meths) {
    methods = meths;
}

void Menu::draw(bool* p_open) {
    if (!ImGui::Begin("ApkAnalyzer In-Game Menu", p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Libraries (.so)")) {
        for (auto& [lib, syms] : symbolsMap) {
            if (ImGui::TreeNode(lib.c_str())) {
                ImGui::Text("Symbols: %d", (int)syms.size());
                if (ImGui::BeginTable("SymTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn("Offset");
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableHeadersRow();

                    for (const auto& sym : syms) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        std::stringstream ss;
                        ss << "0x" << std::hex << sym.address;
                        ImGui::Text("%s", ss.str().c_str());
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", sym.name.c_str());
                    }
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }
        }
    }

    if (ImGui::CollapsingHeader("Dex Classes/Methods")) {
        if (ImGui::BeginTable("DexTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("Index");
            ImGui::TableSetupColumn("Method");
            ImGui::TableHeadersRow();

            for (const auto& m : methods) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", m.methodIdx);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s->%s", m.className.c_str(), m.methodName.c_str());
            }
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
