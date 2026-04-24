use std::fs;
use std::io::Read;

extern "C" {
    fn validate_dex_header(data: *const u8, size: usize) -> bool;
    fn get_dex_class_count(data: *const u8, size: usize) -> u32;
}

pub struct DexParser {
    pub path: String,
}

impl DexParser {
    pub fn new(path: &str) -> Self {
        Self {
            path: path.to_string(),
        }
    }

    pub fn analyze(&self) -> Result<Vec<String>, Box<dyn std::error::Error>> {
        let mut file = fs::File::open(&self.path)?;
        let mut buffer = Vec::new();
        file.read_to_end(&mut buffer)?;

        let is_valid = unsafe { validate_dex_header(buffer.as_ptr(), buffer.len()) };
        if !is_valid {
            return Err("Invalid DEX magic (native check)".into());
        }

        let class_count = unsafe { get_dex_class_count(buffer.as_ptr(), buffer.len()) };
        
        let mut info = Vec::new();
        info.push(format!("DEX Validation: Success (Native)"));
        info.push(format!("Class Count (Native): {}", class_count));

        if buffer.len() >= 8 {
            let version = String::from_utf8_lossy(&buffer[4..7]);
            info.push(format!("DEX Version: {}", version));
        }

        Ok(info)
    }
}
