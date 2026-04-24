pub struct PatternScanner;

impl PatternScanner {
    pub fn scan(data: &[u8], pattern: &str) -> Vec<usize> {
        let pattern_bytes: Vec<Option<u8>> = pattern
            .split_whitespace()
            .map(|s| {
                if s == "??" || s == "?" {
                    None
                } else {
                    Some(u8::from_str_radix(s, 16).unwrap_or(0))
                }
            })
            .collect();

        if pattern_bytes.is_empty() || pattern_bytes.len() > data.len() {
            return Vec::new();
        }

        let mut matches = Vec::new();
        for i in 0..=(data.len() - pattern_bytes.len()) {
            let mut matched = true;
            for (j, p_byte) in pattern_bytes.iter().enumerate() {
                if let Some(b) = p_byte {
                    if data[i + j] != *b {
                        matched = false;
                        break;
                    }
                }
            }
            if matched {
                matches.push(i);
            }
        }
        matches
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_scan() {
        let data = vec![0x11, 0x22, 0x33, 0x44, 0x55, 0x66];
        let matches = PatternScanner::scan(&data, "22 ?? 44");
        assert_eq!(matches, vec![1]);
    }
}
