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

// We do this to not need to write Vantor:: 
// and to not explicitly write the module origin of API calls
#define VANTOR_USE_NO_NAMESPACE

#include <Vantor/Vantor.hpp>
#include <Vantor/Math/Linear.hpp> // Linear Math (VVector, VMatrix)
#include <Vantor/RenderModules/FlyCamera.hpp> // FlyCamera
#include <Vantor/Renderer/Geometry.hpp> // Geometry Objects
#include <Vantor/Integration/ImGui.hpp>
// #include "../Vantor/Source/RenderDevice/OpenGL/VRDO_Texture.hpp"


int main() {
    VApplication app;
    // Application Creation Data
    VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280;
    appInfo.windowHeight = 720;
    appInfo.windowTitle = "Vantor Sandbox";

    // Initialize the Application
    app.Initialize(appInfo);

    // A Basic Color for the rect
    VColor rectColor =  VColor::Cyan();

    // Basic Camera Math
    Math::VMat4 view;

    view = Math::VMat4::LookAt(Math::VVector3(0.0f, 0.0f, 3.0f), 
  		               Math::VVector3(0.0f, 0.0f, 0.0f), 
  		               Math::VVector3(0.0f, 1.0f, 0.0f));

    Math::VVector3 cameraPosition(1.0f, 1.0f, 1.0f);
    Math::VVector3 cameraFront(0.0f, 0.0f, -1.0f);
    Math::VVector3 cameraUp(0.0f, 1.0f,  0.0f);

    // Vantor::RenderDevice::VTextureSampler sampler;
    // sampler.minFilter = Vantor::RenderDevice::VTextureFilter::LinearMipmap;
    // sampler.magFilter = Vantor::RenderDevice::VTextureFilter::Linear;
    // sampler.wrapS = Vantor::RenderDevice::VTextureWrap::Repeat;
    // sampler.wrapT = Vantor::RenderDevice::VTextureWrap::Repeat;
    // sampler.generateMipmaps = true;

    //     // Load texture from file
    // auto texture = Vantor::RenderDevice::VOpenGLTexture2D::CreateFromFile("Resources/textures/glass.png", sampler, true);

    // ===== Object System and build Scene Graph =====
    auto rootNode = Object::VORegistry::CreateEntity<Object::VObject>();

    rootNode->AddComponent<Vantor::Object::VTagComponent>();

    rootNode->GetComponent<Vantor::Object::VTagComponent>()->SetName("Root");
    // Create the Camera with FlyCamera RenderModule and register it as an entity
    auto camera = Object::VORegistry::CreateEntity<RenderModules::FlyCamera>(Math::VVector3(1.0f, 1.0f, 1.0f));

    // Add root childs
    rootNode->AddChild(camera);

    // ==== Cube Object to Render ===
    auto cube = Object::VORegistry::CreateEntity<Renderer::Geometry::VCube>(); // Create Cube Entity

    cube->AddComponentVoid<Vantor::Object::VMeshComponent>(); // Add a MeshComponent for Render

    Vantor::RenderDevice::VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = app.GetRenderDevice()->CreateMesh(cubeCreateInfo);

    cube->GetComponent<Vantor::Object::VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent

    cube->GenerateMesh(); // Generate the Mesh data for the cube

    rootNode->AddChild(cube);

    // Print Hierachy
    // std::cout << "==== Hierachy rootNode (root) id: " << rootNode->GetID() << std::endl;

    // for (auto child : rootNode->GetChildren()) {
    //     std::cout << "          ID: " << child->GetID() << std::endl;
    // }

    // Verify cam ID (should be 1)
    // std::cout << "Camera Entity ID: " << camera->GetID() << std::endl;

    // ==== Object (Entity) System Test ====
    // auto player = Object::VORegistry::CreateEntity<Player>();
    // std::cout << "Created entity of type: " << "Player"
    //           << ", ID: " << player->id << "\n";

    // auto player2 = Object::VORegistry::CreateEntity<Player>();

    // player->AddChild(player2); // Add Player 2 as child of main player

    // std::cout << "Child player of main player has id: " << player->GetChildren()[0]->id << std::endl;

    // std::cout << camera.FOV << std::endl;

    // ==== Input ====
    // Set up InputDevice
    auto deviceone = Input::CreateInputDevice(app.GetWindow());
    app.GetInputManager()->AddDevice(deviceone);

    app.GetInputManager()->MapAction("fire", Input::VInputButton{Input::VEInputDeviceType::Keyboard, (int)Input::VEInputKey::KEY_ESCAPE});


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

    // Run app logic
    app.Run([&]() {
            #ifdef VANTOR_INTEGRATION_IMGUI
            Vantor::Integration::Imgui::BuildUpDocking();
            Vantor::Integration::Imgui::ShowSceneHierachy();
            #endif

            if (app.GetInputManager()->WasPressed(Input::KEY_ESCAPE, Input::VEInputDeviceType::Keyboard)) {
                std::cout << app.GetInputManager()->devices[0]->GetMousePosition().x << std::endl;
                app.Break();
            }
            
            // draw our first triangle
            shaderProgram->use();
            // Test Uniforms with custom Color (turn it into a Vector, to pass it to GPU)
            shaderProgram->setVec4("newColor", rectColor.toFloat4());
            glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        });

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	app.Shutdown();

    return 0;
}