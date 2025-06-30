/*
*    				~ Vantor (Sandbox) ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 25.04.2025
*
* File: Application.hpp
* Last Change:
*/

#include <Vantor/Vantor.hpp>
#include <Vantor/Math/Linear.hpp>
#include <Vantor/RenderModules/FlyCamera.hpp>

// test Entity
class Player : public Vantor::Object::VObject {
    public:
        int health;
};

int main() {
    Vantor::Application app;
    
    // Application Creation Data
    Vantor::VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280;
    appInfo.windowHeight = 720;
    appInfo.windowTitle = "Vantor Sandbox";

    // Initialize the Application
    app.Initialize(appInfo);

    // Basic Camera Math
    Vantor::Math::VMat4 view;

    view = Vantor::Math::VMat4::LookAt(Vantor::Math::VVector3(0.0f, 0.0f, 3.0f), 
  		               Vantor::Math::VVector3(0.0f, 0.0f, 0.0f), 
  		               Vantor::Math::VVector3(0.0f, 1.0f, 0.0f));

    Vantor::Math::VVector3 cameraPosition(1.0f, 1.0f, 1.0f);
    Vantor::Math::VVector3 cameraFront(0.0f, 0.0f, -1.0f);
    Vantor::Math::VVector3 cameraUp(0.0f, 1.0f,  0.0f);

    // ==== Object (Entity) System Test ====
    // auto player = Vantor::Object::VORegistry::CreateEntity<Player>();
    // std::cout << "Created entity of type: " << "Player"
    //           << ", ID: " << player->id << "\n";

    // auto player2 = Vantor::Object::VORegistry::CreateEntity<Player>();

    // player->AddChild(player2); // Add Player 2 as child of main player

    // std::cout << "Child player of main player has id: " << player->GetChildren()[0]->id << std::endl;

    // Fly Camera Render Module
    Vantor::RenderModules::FlyCamera camera(Vantor::Math::VVector3(1.0f, 1.0f, 1.0f));
    // std::cout << camera.FOV << std::endl;

    // ==== Input ====
    // Set up InputDevice
    auto deviceone = Vantor::Input::CreateInputDevice(app.GetWindow());
    app.GetInputManager()->AddDevice(deviceone);

    app.GetInputManager()->MapAction("fire", Vantor::Input::VInputButton{Vantor::Input::VEInputDeviceType::Keyboard, (int)Vantor::Input::VEInputKey::KEY_ESCAPE});


    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    const char *vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 newColor;\n"
        
        "void main()\n"
            "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
        "uniform vec4 newColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = newColor;\n"
        "}\n\0";

    // Set up Shader
    auto shaderProgram = app.GetRenderDevice()->CreateShader(vertexShaderSource, fragmentShaderSource);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // Main loop
    while (app.IsRunning())
    {
        // Run app logic (empty for now)
        app.Run();

        if (app.GetInputManager()->WasActionPressed("fire")) {
            std::cout << app.GetInputManager()->devices[0]->GetMousePosition().x << std::endl;
            break;
        }
        // draw our first triangle
        shaderProgram->use();
        // Test Uniforms with custom Vector
        shaderProgram->setVec4("newColor", Vantor::Math::VVector4(1.0f, 1.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	app.Shutdown();

    return 0;
}