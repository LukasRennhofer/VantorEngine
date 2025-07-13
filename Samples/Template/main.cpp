#include <Vantor/Vantor.hpp>

int main() {
    Vantor::VApplication app; // This is our VApplication object class

    // Application Creation Data (as a struct)
    Vantor::VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280; // Window Width
    appInfo.windowHeight = 720; // Window Height
    appInfo.windowTitle = "MyVantorApplication"; // windowTitle

    // Initialize the Application
    app.Initialize(appInfo);

    // Run app logic
    while (app.IsRunning()) {
            app.BeginFrame();

            // Here you will put your Game's update and rendering logic

            app.EndFrame();
    }

	app.Shutdown();

    return 0;
}