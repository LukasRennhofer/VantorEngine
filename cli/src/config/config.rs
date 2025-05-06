use std::fs;
use toml::Value;

pub fn read_config() -> Value {
    let config_content = fs::read_to_string("Vantor.toml").expect("[❌] Failed to read Vantor.toml");
    toml::from_str(&config_content).expect("[❌] Failed to parse Vantor.toml") 
}