#include "DexParser.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>

struct DexHeader {
    uint8_t magic[8];
    uint32_t checksum;
    uint8_t signature[20];
    uint32_t file_size;
    uint32_t header_size;
    uint32_t endian_tag;
    uint32_t link_size;
    uint32_t link_off;
    uint32_t map_off;
    uint32_t string_ids_size;
    uint32_t string_ids_off;
    uint32_t type_ids_size;
    uint32_t type_ids_off;
    uint32_t proto_ids_size;
    uint32_t proto_ids_off;
    uint32_t field_ids_size;
    uint32_t field_ids_off;
    uint32_t method_ids_size;
    uint32_t method_ids_off;
    uint32_t class_defs_size;
    uint32_t class_defs_off;
    uint32_t data_size;
    uint32_t data_off;
};

struct DexMethodId {
    uint16_t class_idx;
    uint16_t proto_idx;
    uint32_t name_idx;
};

struct DexFieldId {
    uint16_t class_idx;
    uint16_t type_idx;
    uint32_t name_idx;
};

struct DexClassDef {
    uint32_t class_idx;
    uint32_t access_flags;
    uint32_t superclass_idx;
    uint32_t interfaces_off;
    uint32_t source_file_idx;
    uint32_t annotations_off;
    uint32_t class_data_off;
    uint32_t static_values_off;
};

struct DexCodeItem {
    uint16_t registers_size;
    uint16_t ins_size;
    uint16_t outs_size;
    uint16_t tries_size;
    uint32_t debug_info_off;
    uint32_t insns_size;
    uint16_t insns[1];
};

struct DexTypeList {
    uint32_t size;
    uint16_t list[1];
};

struct DexTypeId {
    uint32_t descriptor_idx;
};

DexParser::DexParser(const std::vector<uint8_t>& data) : data(data) {
    if (data.size() < sizeof(DexHeader)) return;
    if (memcmp(data.data(), "dex\n", 4) != 0) return;
    valid = true;
    parse();
}

bool DexParser::isValid() const { return valid; }

// Helper to read LEB128 (used for string lengths in DEX)
uint32_t readULeb128(const uint8_t*& ptr) {
    uint32_t result = 0;
    int shift = 0;
    while (true) {
        uint8_t byte = *ptr++;
        result |= (byte & 0x7f) << shift;
        if ((byte & 0x80) == 0) break;
        shift += 7;
    }
    return result;
}

struct DexProtoId {
    uint32_t shorty_idx;
    uint32_t return_type_idx;
    uint32_t parameters_off;
};

std::string getAccessFlags(uint32_t flags) {
    std::string res = "";
    if (flags & 0x1) res += "public ";
    if (flags & 0x2) res += "private ";
    if (flags & 0x8) res += "static ";
    if (flags & 0x10) res += "final ";
    if (flags & 0x100) res += "native ";
    return res;
}

void DexParser::parse() {
    const DexHeader* header = reinterpret_cast<const DexHeader*>(data.data());
    
    auto getString = [&](uint32_t idx) -> std::string {
        if (idx >= header->string_ids_size) return "";
        uint32_t off = *reinterpret_cast<const uint32_t*>(data.data() + header->string_ids_off + idx * 4);
        if (off >= data.size()) return "";
        const uint8_t* ptr = data.data() + off;
        readULeb128(ptr); // Skip length
        return std::string(reinterpret_cast<const char*>(ptr));
    };
    
    auto getClassName = [&](uint32_t typeIdx) -> std::string {
        if (typeIdx >= header->type_ids_size) return "";
        uint32_t nameIdx = *reinterpret_cast<const uint32_t*>(data.data() + header->type_ids_off + typeIdx * 4);
        return getString(nameIdx);
    };

    auto getProto = [&](uint32_t protoIdx) -> std::string {
        if (protoIdx >= header->proto_ids_size) return "";
        const DexProtoId* protos = reinterpret_cast<const DexProtoId*>(data.data() + header->proto_ids_off);
        std::string retType = getClassName(protos[protoIdx].return_type_idx);
        std::string shorty = getString(protos[protoIdx].shorty_idx);
        
        std::string params = "";
        if (shorty.length() > 1) {
            for (size_t i = 1; i < shorty.length(); ++i) {
                if (i > 1) params += ", ";
                params += shorty[i]; // Simplify: use shorty chars (I, L, Z, etc.)
            }
        }
        return retType + " (" + params + ")";
    };
    
    const DexMethodId* methodIds = reinterpret_cast<const DexMethodId*>(data.data() + header->method_ids_off);
    for (uint32_t i = 0; i < header->method_ids_size; ++i) {
        DexMethod m;
        m.methodIdx = i;
        m.className = getClassName(methodIds[i].class_idx);
        m.methodName = getString(methodIds[i].name_idx);
        m.signature = getProto(methodIds[i].proto_idx);
        methods.push_back(m);
    }

    const DexFieldId* fieldIds = reinterpret_cast<const DexFieldId*>(data.data() + header->field_ids_off);
    for (uint32_t i = 0; i < header->field_ids_size; ++i) {
        DexField f;
        f.fieldIdx = i;
        f.className = getClassName(fieldIds[i].class_idx);
        f.fieldName = getString(fieldIds[i].name_idx);
        f.type = getClassName(fieldIds[i].type_idx);
        fields.push_back(f);
    }

    const DexClassDef* classDefs = reinterpret_cast<const DexClassDef*>(data.data() + header->class_defs_off);
    for (uint32_t i = 0; i < header->class_defs_size; ++i) {
        DexClass c;
        c.classIdx = i;
        c.name = getClassName(classDefs[i].class_idx);
        c.superClass = getClassName(classDefs[i].superclass_idx);
        
        // Resolve interfaces
        if (classDefs[i].interfaces_off != 0) {
            const DexTypeList* typeList = reinterpret_cast<const DexTypeList*>(data.data() + classDefs[i].interfaces_off);
            if (classDefs[i].interfaces_off + 4 <= data.size()) {
                for (uint32_t j = 0; j < typeList->size; j++) {
                    c.interfaces.push_back(getClassName(typeList->list[j]));
                }
            }
        }
        classes.push_back(c);
    }

    const DexProtoId* protoIds = reinterpret_cast<const DexProtoId*>(data.data() + header->proto_ids_off);
    for (uint32_t i = 0; i < header->proto_ids_size; ++i) {
        DexProto p;
        p.protoIdx = i;
        p.shorty = getString(protoIds[i].shorty_idx);
        p.returnType = getClassName(protoIds[i].return_type_idx);
        
        if (protoIds[i].parameters_off != 0) {
            const DexTypeList* typeList = reinterpret_cast<const DexTypeList*>(data.data() + protoIds[i].parameters_off);
            for (uint32_t j = 0; j < typeList->size; j++) {
                p.parameters.push_back(getClassName(typeList->list[j]));
            }
        }
        protos.push_back(p);
    }
}

const char* opcode_names[] = {
    "nop", "move", "move/from16", "move/16", "move-wide", "move-wide/from16",
    "move-wide/16", "move-object", "move-object/from16", "move-object/16",
    "move-result", "move-result-wide", "move-result-object", "move-exception",
    "return-void", "return", "return-wide", "return-object", "const/4",
    "const/16", "const", "const/high16", "const-wide/16", "const-wide/32",
    "const-wide", "const-wide/high16", "const-string", "const-string/jumbo",
    "const-class", "monitor-enter", "const-string/jumbo", "const-class",
    "monitor-enter", "monitor-exit", "check-cast", "instance-of", "array-length",
    "new-instance", "new-array", "filled-new-array", "filled-new-array/range",
    "fill-array-data", "throw", "goto", "goto/16", "goto/32", "packed-switch",
    "sparse-switch", "cmpl-float", "cmpg-float", "cmpl-double", "cmpg-double",
    "cmp-long", "if-eq", "if-ne", "if-lt", "if-ge", "if-gt", "if-le", "if-eqz",
    "if-nez", "if-ltz", "if-gez", "if-gtz", "if-lez"
};

int getOpcodeLength(uint8_t opcode) {
    // Dalvik instruction format lengths based on opcode (roughly)
    // 1-byte opcodes: 00-0F, 10, 11
    if (opcode <= 0x11) return 1;
    // 2-byte opcodes: 12-19
    if (opcode <= 0x19) return 2;
    // 3-byte opcodes: 1A-1C
    if (opcode <= 0x1C) return 3;
    // Longer formats (e.g. range, switch)
    if (opcode == 0x24 || opcode == 0x25 || opcode == 0x2B || opcode == 0x2C) return 3;
    return 1;
}

std::vector<DexInstruction> DexParser::disassembleMethod(uint32_t methodIdx) {
    std::vector<DexInstruction> insts;
    if (data.empty()) return insts;
    
    // In a real DEX, we would locate the code_item for methodIdx
    // For now, we simulate starting from a data offset
    const DexHeader* header = reinterpret_cast<const DexHeader*>(data.data());
    uint32_t code_off = header->data_off; 
    
    uint32_t current_offset = 0;
    for (int i = 0; i < 32; i++) {
        DexInstruction ins;
        ins.offset = current_offset;
        
        uint32_t pos = code_off + current_offset;
        if (pos >= data.size()) break;
        
        uint8_t op = data[pos];
        ins.opcode = opcode_names[op % 64];
        
        int len = getOpcodeLength(op);
        std::stringstream ss;
        for(int j = 1; j < len; j++) {
            if(pos + j < data.size()) ss << std::hex << (int)data[pos + j] << " ";
        }
        ins.details = ss.str();
        insts.push_back(ins);
        
        current_offset += len;
    }
    
    return insts;
}

std::vector<DexMethod> DexParser::getMethods() const { return methods; }
std::vector<DexField> DexParser::getFields() const { return fields; }
std::vector<DexClass> DexParser::getClasses() const { return classes; }
std::vector<DexAnnotation> DexParser::getAnnotations() const { return annotations; }
std::vector<DexProto> DexParser::getProtos() const { return protos; }
