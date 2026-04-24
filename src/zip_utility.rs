use zip::ZipArchive;
use std::fs;
use std::io::copy;
use std::path::Path;

pub struct ZipUtility;

impl ZipUtility {
    pub fn list_files(path: &str) -> Result<Vec<String>, Box<dyn std::error::Error>> {
        let file = fs::File::open(path)?;
        let mut archive = ZipArchive::new(file)?;
        let mut files = Vec::new();

        for i in 0..archive.len() {
            let file = archive.by_index(i)?;
            files.push(file.name().to_string());
        }

        Ok(files)
    }

    pub fn extract_file(zip_path: &str, file_to_extract: &str, output_path: &str) -> Result<(), Box<dyn std::error::Error>> {
        let file = fs::File::open(zip_path)?;
        let mut archive = ZipArchive::new(file)?;
        let mut file = archive.by_name(file_to_extract)?;

        let outpath = Path::new(output_path);
        if let Some(p) = outpath.parent() {
            if !p.exists() {
                fs::create_dir_all(&p)?;
            }
        }
        let mut outfile = fs::File::create(&outpath)?;
        copy(&mut file, &mut outfile)?;

        Ok(())
    }
}
