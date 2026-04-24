use std::fs;
use std::io::{Read, Seek, SeekFrom};

pub struct Il2CppScanner;

impl Il2CppScanner {
    pub fn scan_metadata(path: &str) -> Result<u32, Box<dyn std::error::Error>> {
        let mut file = fs::File::open(path)?;
        let mut magic = [0u8; 4];
        file.read_exact(&mut magic)?;

        if &magic != b"\xAF\x1B\xB1\xFA" {
            return Err("Invalid IL2CPP metadata magic".into());
        }

        let mut version_buf = [0u8; 4];
        file.read_exact(&mut version_buf)?;
        let version = u32::from_le_bytes(version_buf);

        Ok(version)
    }

    pub fn find_metadata_in_apk(apk_path: &str) -> Result<Option<String>, Box<dyn std::error::Error>> {
        let files = crate::zip_utility::ZipUtility::list_files(apk_path)?;
        for file in files {
            if file.ends_with("global-metadata.dat") {
                return Ok(Some(file));
            }
        }
        Ok(None)
    }
}
