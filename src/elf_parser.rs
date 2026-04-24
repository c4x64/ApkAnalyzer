use object::{Object, ObjectSection, ObjectSymbol};
use std::fs;

pub struct ElfParser {
    pub path: String,
}

impl ElfParser {
    pub fn new(path: &str) -> Self {
        Self {
            path: path.to_string(),
        }
    }

    pub fn dump_symbols(&self) -> Result<Vec<String>, Box<dyn std::error::Error>> {
        let binary_data = fs::read(&self.path)?;
        let obj_file = object::File::parse(&*binary_data)?;
        
        let mut symbols = Vec::new();

        // Dynamic symbols
        for symbol in obj_file.dynamic_symbols() {
            if let Ok(name) = symbol.name() {
                if !name.is_empty() {
                    symbols.push(format!("[DYNAMIC] {} @ 0x{:x}", name, symbol.address()));
                }
            }
        }

        // Normal symbols (if not stripped)
        for symbol in obj_file.symbols() {
            if let Ok(name) = symbol.name() {
                if !name.is_empty() {
                    symbols.push(format!("[SYMBOL] {} @ 0x{:x}", name, symbol.address()));
                }
            }
        }

        // Section names
        for section in obj_file.sections() {
            if let Ok(name) = section.name() {
                symbols.push(format!("[SECTION] {} (0x{:x} - 0x{:x})", name, section.address(), section.address() + section.size()));
            }
        }

        Ok(symbols)
    }
}
