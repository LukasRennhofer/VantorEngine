use std::fs;
use std::process;
use toml::Value;
use crate::utils::create_spinner;
use std::path::Path;

pub fn build_project(platform: &str) {
    if !Path::new("Vantor.toml").exists() {
        println!("[❌] Vantor.toml not found. Are you in a Vantor project directory?");
        process::exit(1);
    }

    let config_content = fs::read_to_string("Vantor.toml").expect("[❌] Failed to read Vantor.toml");
    let _config: Value = toml::from_str(&config_content).expect("[❌] Failed to parse Vantor.toml");

    // Simulating build with spinner
    let spinner = create_spinner();
    spinner.set_message(format!("[🏗️] Building project for platform: {}", platform));
    spinner.enable_steady_tick(std::time::Duration::from_millis(100));

    // TODO: Implement build logic here
    std::thread::sleep(std::time::Duration::from_secs(2)); // Simulated work

    spinner.finish_with_message(format!("[✅] Build for {} completed", platform));
}