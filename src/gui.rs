use eframe::egui;

pub struct ApkAnalyzerApp {
    input_path: String,
    log_messages: Vec<String>,
}

impl Default for ApkAnalyzerApp {
    fn default() -> Self {
        Self {
            input_path: String::new(),
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
                    self.log_messages.push(format!("Analyzing: {}", self.input_path));
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
