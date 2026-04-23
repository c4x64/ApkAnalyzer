#include "Menu.hpp"
#include <map>
#include <iomanip>
#include <sstream>

std::map<std::string, std::vector<ElfSymbol>>& Menu::getSymbolsMap() {
    static std::map<std::string, std::vector<ElfSymbol>> symbolsMap;
    return symbolsMap;
}

std::vector<DexMethod>& Menu::getMethods() {
    static std::vector<DexMethod> methods;
    return methods;
}

std::vector<DexField>& Menu::getFields() {
    static std::vector<DexField> fields;
    return fields;
}

std::vector<DexClass>& Menu::getClasses() {
    static std::vector<DexClass> classes;
    return classes;
}

std::vector<DexInstruction>& Menu::getBytecode() {
    static std::vector<DexInstruction> bytecode;
    return bytecode;
}

std::string& Menu::getCurrentMethodName() {
    static std::string currentMethodName;
    return currentMethodName;
}

std::vector<uint8_t>& Menu::getCurrentDexData() {
    static std::vector<uint8_t> currentDexData;
    return currentDexData;
}

void Menu::setSymbols(const std::string& libName, const std::vector<ElfSymbol>& syms) {
    getSymbolsMap()[libName] = syms;
}

void Menu::setMethods(const std::vector<DexMethod>& meths) {
    getMethods() = meths;
}

void Menu::setFields(const std::vector<DexField>& flds) {
    getFields() = flds;
}

void Menu::setClasses(const std::vector<DexClass>& clss) {
    getClasses() = clss;
}

void Menu::setBytecode(const std::string& methodName, const std::vector<DexInstruction>& insts) {
    getCurrentMethodName() = methodName;
    getBytecode() = insts;
}

void Menu::clearData() {
    getSymbolsMap().clear();
    getMethods().clear();
    getFields().clear();
    getClasses().clear();
    getBytecode().clear();
    getCurrentMethodName().clear();
    getCurrentDexData().clear();
}

static char libFilter[128] = "";
static char dexFilter[128] = "";
static char fieldFilter[128] = "";
static char classFilter[128] = "";

void Menu::SetupTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Catppuccin Mocha Palette
    colors[ImGuiCol_WindowBg] = ImVec4(0.118f, 0.118f, 0.180f, 1.00f); // --bg-4
    colors[ImGuiCol_ChildBg] = ImVec4(0.094f, 0.094f, 0.145f, 1.00f); // --bg-3
    colors[ImGuiCol_Border] = ImVec4(0.192f, 0.196f, 0.353f, 1.00f);  // --active
    colors[ImGuiCol_FrameBg] = ImVec4(0.192f, 0.196f, 0.353f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.118f, 0.118f, 0.180f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.271f, 0.275f, 0.435f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.094f, 0.094f, 0.145f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.192f, 0.196f, 0.353f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.271f, 0.275f, 0.435f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.192f, 0.196f, 0.353f, 1.00f);
    colors[ImGuiCol_Text] = ImVec4(0.804f, 0.839f, 0.957f, 1.00f);     // --text-2
    colors[ImGuiCol_TextDisabled] = ImVec4(0.345f, 0.357f, 0.439f, 1.00f); // --text-5
}

void Menu::draw(bool* p_open) {
    SetupTheme();
    if (!ImGui::Begin("ApkAnalyzer In-Game Menu", p_open)) {
        ImGui::End();
        return;
    }

    if (ImGui::BeginTabBar("Tabs")) {
        if (ImGui::BeginTabItem("Libraries (.so)")) {
            ImGui::InputText("Search Symbols", libFilter, sizeof(libFilter));
            for (auto& [lib, syms] : getSymbolsMap()) {
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
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Dex Methods")) {
            ImGui::InputText("Search Methods", dexFilter, sizeof(dexFilter));
            if (ImGui::BeginTable("DexTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0, 500))) {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Access");
                ImGui::TableSetupColumn("Class");
                ImGui::TableSetupColumn("Method");
                ImGui::TableSetupColumn("Signature");
                ImGui::TableHeadersRow();

                std::string filter = dexFilter;
                for (const auto& m : getMethods()) {
                    if (!filter.empty() && m.methodName.find(filter) == std::string::npos && m.className.find(filter) == std::string::npos) continue;

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::Selectable(std::to_string(m.methodIdx).c_str(), false, ImGuiSelectableFlags_SpanAllColumns)) {
                        DexParser parser(getCurrentDexData());
                        if (parser.isValid()) {
                            setBytecode(m.className + "->" + m.methodName, parser.disassembleMethod(m.methodIdx));
                        }
                    }
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", m.accessFlags.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", m.className.c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", m.methodName.c_str());
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%s", m.signature.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Dex Fields")) {
            ImGui::InputText("Search Fields", fieldFilter, sizeof(fieldFilter));
            if (ImGui::BeginTable("FieldTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0, 500))) {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Access");
                ImGui::TableSetupColumn("Class");
                ImGui::TableSetupColumn("Field");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();

                std::string filter = fieldFilter;
                for (const auto& f : getFields()) {
                    if (!filter.empty() && f.fieldName.find(filter) == std::string::npos && f.className.find(filter) == std::string::npos) continue;

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", f.fieldIdx);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", f.accessFlags.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", f.className.c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", f.fieldName.c_str());
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%s", f.type.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Dex Classes")) {
            ImGui::InputText("Search Classes", classFilter, sizeof(classFilter));
            if (ImGui::BeginTable("ClassTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY, ImVec2(0, 500))) {
                ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed, 60.0f);
                ImGui::TableSetupColumn("Class Name");
                ImGui::TableSetupColumn("Superclass");
                ImGui::TableSetupColumn("Interfaces");
                ImGui::TableHeadersRow();

                std::string filter = classFilter;
                for (const auto& c : getClasses()) {
                    if (!filter.empty() && c.name.find(filter) == std::string::npos) continue;

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", c.classIdx);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", c.name.c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", c.superClass.c_str());
                    ImGui::TableSetColumnIndex(3);
                    std::string combinedInterfaces = "";
                    for (const auto& i : c.interfaces) combinedInterfaces += i + " ";
                    ImGui::Text("%s", combinedInterfaces.c_str());
                }
                ImGui::EndTable();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    if (!getCurrentMethodName().empty()) {
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Method Disassembly", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::TextColored(ImVec4(0, 1, 1, 1), "Method: %s", getCurrentMethodName().c_str());
        
        if (ImGui::BeginTabBar("DisasmTabs")) {
            if (ImGui::BeginTabItem("Bytecode")) {
                if (ImGui::BeginTable("BytecodeTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)) {
                    ImGui::TableSetupColumn("Offset", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Opcode", ImGuiTableColumnFlags_WidthFixed, 120.0f);
                    ImGui::TableSetupColumn("Details");
                    ImGui::TableHeadersRow();

                    for (const auto& inst : getBytecode()) {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("0x%04X", inst.offset);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", inst.opcode.c_str());
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%s", inst.details.c_str());
                    }
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Decompiled")) {
                DexParser parser(getCurrentDexData());
                std::string code = parser.decompileMethod(0); // Using 0 as placeholder
                ImGui::InputTextMultiline("##code", (char*)code.c_str(), code.size(), ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        if (ImGui::Button("Close")) getCurrentMethodName().clear();
        ImGui::End();
    }

    ImGui::End();
}
