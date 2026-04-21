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

static char libFilter[128] = "";
static char dexFilter[128] = "";

void Menu::draw(bool* p_open) {
    if (!ImGui::Begin("ApkAnalyzer In-Game Menu", p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::CollapsingHeader("Libraries (.so)")) {
        ImGui::InputText("Search Symbols", libFilter, sizeof(libFilter));
        for (auto& [lib, syms] : symbolsMap) {
            if (ImGui::TreeNode(lib.c_str())) {
                if (ImGui::BeginTable("SymTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 300))) {
                    ImGui::TableSetupColumn("Offset", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableSetupColumn("Name");
                    ImGui::TableSetupColumn("Insight");
                    ImGui::TableHeadersRow();

                    std::string filter = libFilter;
                    for (const auto& sym : syms) {
                        if (!filter.empty() && sym.name.find(filter) == std::string::npos) continue;

                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        std::stringstream ss;
                        ss << "0x" << std::hex << sym.address;
                        ImGui::Text("%s", ss.str().c_str());
                        
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", sym.name.c_str());

                        ImGui::TableSetColumnIndex(2);
                        extern std::string getInsight(const std::string& name);
                        ImGui::Text("%s", getInsight(sym.name).c_str());
                    }
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }
        }
    }

    if (ImGui::CollapsingHeader("Dex Classes/Methods")) {
        ImGui::InputText("Search Methods", dexFilter, sizeof(dexFilter));
        if (ImGui::BeginTable("DexTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0, 500))) {
            ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("Class");
            ImGui::TableSetupColumn("Method");
            ImGui::TableSetupColumn("Signature");
            ImGui::TableHeadersRow();

            std::string filter = dexFilter;
            for (const auto& m : methods) {
                if (!filter.empty() && m.methodName.find(filter) == std::string::npos && m.className.find(filter) == std::string::npos) continue;

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", m.methodIdx);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", m.className.c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", m.methodName.c_str());
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", m.signature.c_str());
            }
            ImGui::EndTable();
        }
    }

    ImGui::End();
}
