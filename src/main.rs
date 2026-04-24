mod utils;
mod entropy;
mod gui;
mod elf_parser;
mod dex_parser;

use eframe::egui;

fn main() -> eframe::Result {
    let native_options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default()
            .with_inner_size([800.0, 600.0]),
        ..Default::default()
    };
    eframe::run_native(
        "ApkAnalyzer",
        native_options,
        Box::new(|cc| Ok(Box::new(gui::ApkAnalyzerApp::new(cc)))),
    )
}
