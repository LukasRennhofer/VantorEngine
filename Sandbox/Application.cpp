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

    glfwSwapInterval(0); // VSync OFF : TODO: Implement in Context

    // RenderPath Test
    auto renderpath = app.GetRenderDevice()->CreateRenderPath3D();

    // Properly initialize the camera with perspective projection
    camera->SetPerspective(45.0f, (float)VServiceLocator::GetContextWidth()/(float)VServiceLocator::GetContextHeight(), 0.1f, 100.0f);
    
    renderpath->SetCamera(camera.get());

    // renderpath->SetWireframeMode(true); // Re-enable wireframe for debugging

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

    // Run app logic
    while (app.IsRunning()) {
            app.BeginFrame();

            // Show Camera Position if ImGui is enabled
            #ifdef VANTOR_INTEGRATION_IMGUI
            Vantor::Integration::Imgui::BuildUpDocking();
            Vantor::Integration::Imgui::ShowSceneHierachy();
            ImGui::Begin("Camera");
            ImGui::Text("X: %f Y: %f Z: %f", camera->Position.x, camera->Position.y, camera->Position.z);
            ImGui::Text("FPS: %f", 1.0f/app.GetDeltaTime());
            ImGui::End();
            #endif

            if (app.GetInputManager()->WasPressed(Input::KEY_ESCAPE, Input::VEInputDeviceType::Keyboard)) {
                app.Break();
            }

            if (app.GetInputManager()->IsPressed(Input::KEY_W, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_FORWARD);
            } else if (app.GetInputManager()->IsPressed(Input::KEY_A, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_LEFT);
            } else if (app.GetInputManager()->IsPressed(Input::KEY_D, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_RIGHT);
            } else if (app.GetInputManager()->IsPressed(Input::KEY_S, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_BACK);
            } else if (app.GetInputManager()->IsPressed(Input::KEY_E, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_UP);
            } else if (app.GetInputManager()->IsPressed(Input::KEY_Q, Input::VEInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), Vantor::Renderer::CAMERA_DOWN);
            }

            camera->Update(app.GetDeltaTime());

            renderpath->Render();

            app.EndFrame();
    }

    renderpath->Shutdown();

	app.Shutdown();

    return 0;
}