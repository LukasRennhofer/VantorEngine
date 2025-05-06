use std::fs;
use std::path::Path;

pub fn init_project(project_name: &str) {
    // Create project directory
    fs::create_dir_all(project_name).expect("[❌] Failed to create project directory");

    // Create src directory
    let src_path = Path::new(project_name).join("src");
    fs::create_dir_all(&src_path).expect("[❌] Failed to create ./src/ directory");

    // Create target directory
    let target_path = Path::new(project_name).join("target");
    fs::create_dir_all(target_path).expect("[❌] Failed to create ./target/ directory");

    // Create main.cpp (basic template)
    let main_cpp_content = r#"#include <Vantor.h>

extern "C" int main(int argc, char **argv)
{
    vantor::Application app;
    app.Initialize();
}
"#;
    let main_cpp_path = src_path.join("main.cpp");
    fs::write(main_cpp_path, main_cpp_content).expect("[❌] Failed to create main.cpp");

    // Create Vantor.toml (basic template)
    let config_content = format!(r#"[project]
name = "{}"
version = "0.1.0"
author = ""

[build]
target_platforms = ["Windows", "Linux", "MacOS"]
default_platform = "Windows"
"#, project_name);

    let config_path = Path::new(project_name).join("Vantor.toml");
    fs::write(config_path, config_content).expect("[❌] Failed to create Vantor.toml");

    println!("[✅] Successfully created project '{}'", project_name);
}
