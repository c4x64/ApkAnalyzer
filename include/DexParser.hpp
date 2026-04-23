#pragma once
#include <string>
#include <vector>
#include <cstdint>

struct DexMethod {
    std::string className;
    std::string methodName;
    std::string signature;
    std::string accessFlags;
    uint32_t methodIdx;
};

struct DexField {
    std::string className;
    std::string fieldName;
    std::string type;
    std::string accessFlags;
    uint32_t fieldIdx;
};

struct DexClass {
    std::string name;
    std::string superClass;
    std::vector<std::string> interfaces;
    uint32_t classIdx;
};

struct DexAnnotation {
    std::string visibility;
    std::string type;
    uint32_t annotationIdx;
};

struct DexInstruction {
    uint32_t offset;
    std::string opcode;
    std::string details;
};

struct DexProto {
    std::string shorty;
    std::string returnType;
    std::vector<std::string> parameters;
    uint32_t protoIdx;
};

class DexParser {
public:
    DexParser(const std::vector<uint8_t>& data);
    
    bool isValid() const;
    std::vector<DexMethod> getMethods() const;
    std::vector<DexField> getFields() const;
    std::vector<DexClass> getClasses() const;
    std::vector<DexAnnotation> getAnnotations() const;
    std::vector<DexProto> getProtos() const;
    std::vector<DexInstruction> disassembleMethod(uint32_t methodIdx);
    std::string decompileMethod(uint32_t methodIdx);

private:
    std::vector<uint8_t> data;
    bool valid = false;
    std::vector<DexMethod> methods;
    std::vector<DexField> fields;
    std::vector<DexClass> classes;
    std::vector<DexAnnotation> annotations;
    std::vector<DexProto> protos;
    
    void parse();
};
