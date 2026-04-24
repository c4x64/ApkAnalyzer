use eframe::egui;

pub struct ApkAnalyzerApp {
    input_path: String,
    scan_pattern: String,
    log_messages: Vec<String>,
}

impl Default for ApkAnalyzerApp {
    fn default() -> Self {
        Self {
            input_path: String::new(),
            scan_pattern: String::new(),
            log_messages: vec!["Welcome to ApkAnalyzer (Rust Edition)".to_string()],
        }
    }
}

impl ApkAnalyzerApp {
    pub fn new(_cc: &eframe::CreationContext<'_>) -> Self {
        Default::default()
    }
}

impl eframe::App for ApkAnalyzerApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("ApkAnalyzer");

            ui.horizontal(|ui| {
                ui.label("Input Path: ");
                ui.text_edit_singleline(&mut self.input_path);
                if ui.button("Analyze").clicked() {
                    let path = self.input_path.clone();
                    if path.is_empty() {
                        self.log_messages.push("Error: No input path provided".to_string());
                    } else if path.ends_with(".so") || path.contains("lib") {
                        let parser = crate::elf_parser::ElfParser::new(&path);
                        match parser.dump_symbols() {
                            Ok(symbols) => {
                                self.log_messages.push(format!("ELF Analysis for {}: {} symbols found", path, symbols.len()));
                                for sym in symbols.iter().take(10) {
                                    self.log_messages.push(format!("  {}", sym));
                                }
                                if symbols.len() > 10 {
                                    self.log_messages.push(format!("  ... and {} more", symbols.len() - 10));
                                }
                            }
                            Err(e) => self.log_messages.push(format!("Error parsing ELF: {}", e)),
                        }
                    } else if path.ends_with(".dex") {
                        let parser = crate::dex_parser::DexParser::new(&path);
                        match parser.analyze() {
                            Ok(info) => {
                                self.log_messages.push(format!("DEX Analysis for {}:", path));
                                for line in info {
                                    self.log_messages.push(format!("  {}", line));
                                }
                            }
                            Err(e) => self.log_messages.push(format!("Error parsing DEX: {}", e)),
                        }
                    } else if path.ends_with(".apk") || path.ends_with(".zip") {
                        match crate::zip_utility::ZipUtility::list_files(&path) {
                            Ok(files) => {
                                self.log_messages.push(format!("APK/ZIP Contents for {}: {} files found", path, files.len()));
                                for f in files.iter().take(15) {
                                    self.log_messages.push(format!("  {}", f));
                                }
                                if files.len() > 15 {
                                    self.log_messages.push(format!("  ... and {} more", files.len() - 15));
                                }
                                
                                // Check for IL2CPP
                                if files.iter().any(|f| f.ends_with("global-metadata.dat")) {
                                    self.log_messages.push("  [+] IL2CPP Metadata found!".to_string());
                                }
                            }
                            Err(e) => self.log_messages.push(format!("Error listing ZIP: {}", e)),
                        }
                    } else if path.ends_with(".dat") {
                        match crate::il2cpp_scanner::Il2CppScanner::scan_metadata(&path) {
                            Ok(version) => self.log_messages.push(format!("IL2CPP Metadata version: {}", version)),
                            Err(e) => self.log_messages.push(format!("Error scanning metadata: {}", e)),
                        }
                    } else if path.ends_with(".xml") {
                        match std::fs::read(&path) {
                            Ok(data) => {
                                match crate::manifest_parser::ManifestParser::parse_xml(&data) {
                                    Ok(info) => {
                                        self.log_messages.push(format!("Manifest Analysis for {}:", path));
                                        for line in info.lines() {
                                            self.log_messages.push(format!("  {}", line));
                                        }
                                    }
                                    Err(e) => self.log_messages.push(format!("Error parsing XML: {}", e)),
                                }
                            }
                            Err(e) => self.log_messages.push(format!("Error reading XML file: {}", e)),
                        }
                    } else {
                        // General entropy analysis
                        match crate::entropy::EntropyCalculator::calculate_from_file(&path) {
                            Ok(e) => self.log_messages.push(format!("Entropy of {}: {:.4}", path, e)),
                            Err(err) => self.log_messages.push(format!("Error: {}", err)),
                        }
                    }
                }
            });

            ui.horizontal(|ui| {
                ui.label("Scan Pattern: ");
                ui.text_edit_singleline(&mut self.scan_pattern);
                if ui.button("Scan").clicked() {
                    let path = self.input_path.clone();
                    let pattern = self.scan_pattern.clone();
                    if path.is_empty() || pattern.is_empty() {
                        self.log_messages.push("Error: Path or Pattern is empty".to_string());
                    } else {
                        match std::fs::read(&path) {
                            Ok(data) => {
                                let results = crate::scanner::PatternScanner::scan(&data, &pattern);
                                self.log_messages.push(format!("Scan results for {}: {} matches", pattern, results.len()));
                                for r in results.iter().take(10) {
                                    self.log_messages.push(format!("  Match at 0x{:x}", r));
                                }
                                if results.len() > 10 {
                                    self.log_messages.push(format!("  ... and {} more", results.len() - 10));
                                }
                            }
                            Err(e) => self.log_messages.push(format!("Error reading file for scan: {}", e)),
                        }
                    }
                }
            });

            ui.separator();

            ui.label("Logs:");
            egui::ScrollArea::vertical().show(ui, |ui| {
                for msg in &self.log_messages {
                    ui.label(msg);
                }
            });
        });
    }
}
