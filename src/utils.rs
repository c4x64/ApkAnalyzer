pub fn hex_to_bytes(hex: &str) -> Vec<u8> {
    let clean_hex: String = hex.chars().filter(|c| !c.is_whitespace()).collect();
    hex::decode(clean_hex).unwrap_or_default()
}

pub fn bytes_to_hex(data: &[u8]) -> String {
    hex::encode(data)
}

pub fn file_exists(path: &str) -> bool {
    std::path::Path::new(path).exists()
}

pub fn get_file_name(path: &str) -> String {
    std::path::Path::new(path)
        .file_name()
        .and_then(|n| n.to_str())
        .unwrap_or(path)
        .to_string()
}
