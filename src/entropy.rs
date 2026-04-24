use std::io::Read;

extern "C" {
    fn calculate_shannon_entropy(data: *const u8, size: usize) -> f64;
}

pub struct EntropyCalculator;

impl EntropyCalculator {
    pub fn calculate_native(data: &[u8]) -> f64 {
        if data.is_empty() {
            return 0.0;
        }
        unsafe {
            calculate_shannon_entropy(data.as_ptr(), data.len())
        }
    }

    pub fn calculate(data: &[u8]) -> f64 {
        if data.is_empty() {
            return 0.0;
        }

        let mut counts = [0usize; 256];
        for &byte in data {
            counts[byte as usize] += 1;
        }

        let size = data.len() as f64;
        let mut entropy = 0.0;

        for &count in counts.iter() {
            if count > 0 {
                let p = count as f64 / size;
                entropy -= p * p.log2();
            }
        }

        entropy
    }

    pub fn calculate_from_file(path: &str) -> std::io::Result<f64> {
        let mut file = std::fs::File::open(path)?;
        let mut counts = [0usize; 256];
        let mut total_size = 0usize;
        let mut buffer = [0u8; 4096];

        while let Ok(n) = file.read(&mut buffer) {
            if n == 0 {
                break;
            }
            total_size += n;
            for &byte in &buffer[..n] {
                counts[byte as usize] += 1;
            }
        }

        if total_size == 0 {
            return Ok(0.0);
        }

        let size = total_size as f64;
        let mut entropy = 0.0;

        for &count in counts.iter() {
            if count > 0 {
                let p = count as f64 / size;
                entropy -= p * p.log2();
            }
        }

        Ok(entropy)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_entropy() {
        let data = vec![0u8; 100];
        assert!((EntropyCalculator::calculate(&data) - 0.0).abs() < 0.001);
        assert!((EntropyCalculator::calculate_native(&data) - 0.0).abs() < 0.001);

        let mut data2 = Vec::new();
        for i in 0..256 {
            data2.push(i as u8);
        }
        assert!((EntropyCalculator::calculate(&data2) - 8.0).abs() < 0.001);
        assert!((EntropyCalculator::calculate_native(&data2) - 8.0).abs() < 0.001);
    }
}
