use webbrowser;

pub fn open_docs() {
    println!("[📚] Opening documentation website...");
    
    if webbrowser::open("https://vantor.net").is_err() {
        println!("[❌] Failed to open documentation website");
    }
}