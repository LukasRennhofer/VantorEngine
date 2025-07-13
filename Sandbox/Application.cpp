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

float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

int main() {
    VApplication app;
    // Application Creation Data
    VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280;
    appInfo.windowHeight = 720;
    appInfo.windowTitle = "Vantor Sandbox";

    // Initialize the Application
    app.Initialize(appInfo);

    // Generate Cube Data

    std::vector<Vantor::Math::VVector3> positions;
    std::vector<Vantor::Math::VVector3> normals;
    std::vector<Vantor::Math::VVector2> texCoords;

    const int stride = 8;  // 3 pos + 3 norm + 2 tex = 8 floats per vertex
    const int vertexCount = sizeof(vertices) / sizeof(float) / stride;

    for (int i = 0; i < vertexCount; i++) {
        int baseIndex = i * stride;
        
        // Extract position
        Vantor::Math::VVector3 pos(
            vertices[baseIndex],
            vertices[baseIndex + 1],
            vertices[baseIndex + 2]
        );
        positions.push_back(pos);
        
        // Extract normal
        Vantor::Math::VVector3 norm(
            vertices[baseIndex + 3],
            vertices[baseIndex + 4],
            vertices[baseIndex + 5]
        );
        normals.push_back(norm);
        
        // Extract texture coordinates
        Vantor::Math::VVector2 uv(
            vertices[baseIndex + 6],
            vertices[baseIndex + 7]
        );
        texCoords.push_back(uv);
    }

    const char* version = (const char*)glGetString(GL_VERSION);
    std::cout << "OpenGL version: " << version << std::endl;


    // for Mouse Movement
    Vantor::Math::VVector2 mouseDelta;

    // Create the Camera with FlyCamera RenderModule and register it as an entity
    auto camera = Object::VORegistry::CreateEntity<RenderModules::FlyCamera>(Math::VVector3(1.0f, 1.0f, 1.0f));

    camera->MouseSensitivty  = 0.05f; // Set Sensivity a bit lower

    // ==== Cube Object to Render ===

    // Get the texture
    // Texture Sampler
    Vantor::RenderDevice::VTextureSampler sampler;
    sampler.minFilter       = Vantor::RenderDevice::VTextureFilter::Linear;
    sampler.magFilter       = Vantor::RenderDevice::VTextureFilter::Linear;
    sampler.wrapS           = Vantor::RenderDevice::VTextureWrap::Repeat;
    sampler.wrapT           = Vantor::RenderDevice::VTextureWrap::Repeat;
    sampler.generateMipmaps = false;

    auto m_ResDeffered = Vantor::Resource::VResourceManager::Instance().LoadTexture2D("m_Deffered", "Resources/textures/container2.png", sampler, false);
    auto m_ResSpecular = Vantor::Resource::VResourceManager::Instance().LoadTexture2D("m_Specular", "Resources/textures/container2_specular.png", sampler, false);

    auto m_DiffuseTexture = m_ResDeffered->GetTexture();
    auto m_SpecularTexture = m_ResSpecular->GetTexture();


    auto cube = Object::VORegistry::CreateEntity<Renderer::Geometry::VCube>(); // Create Cube Entity

    cube->AddComponentVoid<Vantor::Object::VMeshComponent>(); // Add a MeshComponent for Render

    Vantor::RenderDevice::VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = app.GetRenderDevice()->CreateMesh(cubeCreateInfo);

    cube->GetComponent<Vantor::Object::VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent

    cube->GetComponent<Vantor::Object::VMeshComponent>()->GetMesh()->SetPositions(positions);
    cube->GetComponent<Vantor::Object::VMeshComponent>()->GetMesh()->SetNormals(normals);
    cube->GetComponent<Vantor::Object::VMeshComponent>()->GetMesh()->SetUVs(texCoords);

    cube->GetComponent<Vantor::Object::VMeshComponent>()->GetMesh()->Finalize();

    auto resMaterialShader = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("VForwardMaterial", "Shaders/Private/VForward.vglsl", "Shaders/Private/VForward.fglsl");

    Vantor::Renderer::VMaterial cubeMaterial(resMaterialShader->GetShader().get());

    // Give the Material the Sampler Textures
    // Diffuse
    cubeMaterial.SetTexture("texture_diffuse1", m_DiffuseTexture.get(), 1);
    // Specular
    cubeMaterial.SetTexture("texture_specular1", m_SpecularTexture.get(), 2);
    

    cubeMaterial.color = Vantor::Core::Types::VColor::Gray();

    auto modelTransform = Vantor::Math::VMat4::Identity();
    modelTransform = modelTransform.Translate(Vantor::Math::VVector3(1.0f, 1.0f, 1.0f));

    // Point Lights 
    Vantor::Renderer::VPointLightData pointLight1;
    pointLight1.position = { 20.5f, 2.0f, 1.5f }; // Slightly above and in front of the cube
    pointLight1.ambient  = { 0.05f, 0.05f, 0.05f };
    pointLight1.diffuse  = { 0.1f, 0.1f, 0.1f };  // Full white diffuse for brightness
    pointLight1.specular = { 0.1f, 0.1f, 0.1f };
    pointLight1.constant = 1.0f;
    pointLight1.linear   = 0.09f;
    pointLight1.quadratic = 0.032f;

    glfwSwapInterval(0); // VSync OFF : TODO: Implement in Context

    // RenderPath Test
    auto renderpath = app.GetRenderDevice()->CreateRenderPath3D();

    // Properly initialize the camera with perspective projection
    camera->SetPerspective(45.0f, (float)VServiceLocator::GetContextWidth()/(float)VServiceLocator::GetContextHeight(), 0.1f, 100.0f);
    
    renderpath->SetCamera(camera.get());

    // renderpath->SetWireframeMode(true); // Re-enable wireframe for debugging

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
            // To enable it only when Tab is pressedw
            if (app.GetInputManager()->devices[0]->GetMouseCursorState()) {
                Vantor::Integration::Imgui::BuildUpDocking();
                Vantor::Integration::Imgui::ShowSceneHierachy();
                ImGui::Begin("Camera");
                ImGui::Text("X: %f Y: %f Z: %f", camera->Position.x, camera->Position.y, camera->Position.z);
                ImGui::Text("FPS: %f", 1.0f/app.GetDeltaTime());
                ImGui::End();
            }
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
            
            if (app.GetInputManager()->WasPressed(Input::KEY_TAB, Input::VEInputDeviceType::Keyboard)) {
                app.GetInputManager()->devices[0]->SetMouseCursorState(!app.GetInputManager()->devices[0]->GetMouseCursorState());
            }

            // See if Cursor is disabled
            if (!app.GetInputManager()->devices[0]->GetMouseCursorState()) {
                mouseDelta = app.GetInputManager()->devices[0]->GetMouseDelta();
                camera->InputMouse(mouseDelta.x, -mouseDelta.y);
            }
            camera->Update(app.GetDeltaTime());

            // Our Render Pushes
            renderpath->PushRender(cubeMesh.get(), &cubeMaterial, modelTransform);

            renderpath->PushPointLight(pointLight1);

            renderpath->Render();

            app.EndFrame();
    }

    renderpath->Shutdown();

	app.Shutdown();

    return 0;
}