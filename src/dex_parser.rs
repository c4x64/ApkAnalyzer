use std::fs;
use std::io::{Read, Seek, SeekFrom};

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
        let mut magic = [0u8; 8];
        file.read_exact(&mut magic)?;

        if &magic[0..4] != b"dex\n" {
            return Err("Invalid DEX magic".into());
        }

        let version = String::from_utf8_lossy(&magic[4..7]);
        let mut info = Vec::new();
        info.push(format!("DEX Version: {}", version));

        // Basic DEX header info (simplified)
        // Offset 32: File Size (u32)
        file.seek(SeekFrom::Start(32))?;
        let mut size_buf = [0u8; 4];
        file.read_exact(&mut size_buf)?;
        let file_size = u32::from_le_bytes(size_buf);
        info.push(format!("DEX File Size: {} bytes", file_size));

        // Offset 52: Class IDs Size (u32)
        file.seek(SeekFrom::Start(52))?;
        file.read_exact(&mut size_buf)?;
        let class_ids_size = u32::from_le_bytes(size_buf);
        info.push(format!("Class Count: {}", class_ids_size));

        Ok(info)
    }
}
