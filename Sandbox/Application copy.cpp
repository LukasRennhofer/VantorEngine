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

#include "common.h"

#include "LoadMesh.hpp"

int main() {
    // Create a OpenGL RenderDevice (VANTOR_API_OPENGL must be compiled)
    auto RenderDevice = vCreateRenderDevice(VRenderAPI::OPENGL);

    VApplication app;
    // Application Creation Data
    VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280;
    appInfo.windowHeight = 720;
    appInfo.windowTitle = "Vantor Sandbox";

    // Initialize the Application
    app.Initialize(appInfo);

    // Test Vector 
    // VVector<int> new_vector;
    // new_vector.push_back(2);
    // new_vector.push_back(2);

    // Test SafeString
    // VSafeString new_string = "Hello, from Vantor!";
    // std::cout << "Hashed String: " << new_string.hash() << std::endl;

    // std::cout << new_vector.at(1) << std::endl;
    MeshData meshdata = LoadMeshFromGLTF("Resources/meshes/DamagedHelmet/DamagedHelmet.gltf");

    std::vector<VVector3> positions = meshdata.positions;
    std::vector<VVector3> normals = meshdata.normals;
    std::vector<VVector2> texCoords = meshdata.uvs;
    std::vector<unsigned int> indicies = meshdata.indices;

    // const char* version = (const char*)glGetString(GL_VERSION);
    // std::cout << "OpenGL version: " << version << std::endl;

    // for Mouse Movement
    VVector2 mouseDelta;

    // Create the Camera with FlyCamera RenderModule and register it as an entity
    auto camera = vCreateActor<VFlyCamera>(VVector3(1.0f, 1.0f, 1.0f));

    camera->MouseSensitivty  = 0.05f; // Set Sensivity a bit lower

    // ==== Cube Object to Render ===

    // Get the texture
    // Texture Sampler
    VTextureSampler sampler;
    sampler.minFilter       = VTextureFilter::Linear;
    sampler.magFilter       = VTextureFilter::Linear;
    sampler.wrapS           = VTextureWrap::Repeat;
    sampler.wrapT           = VTextureWrap::Repeat;
    sampler.generateMipmaps = false;

    
    // auto m_ResDeffered = vLoadTexture2D("m_Deffered", "Resources/textures/container2.png", sampler, false);
    // auto m_ResSpecular = vLoadTexture2D("m_Specular", "Resources/textures/container2_specular.png", sampler, false);

    auto m_DiffuseTexture = LoadDiffuseTexture(meshdata, RenderDevice.get(), sampler);
    auto m_NormalTexture = LoadNormalTexture(meshdata, RenderDevice.get(), sampler);
    auto m_metallicRoughnessTexture = LoadMetallicTexture(meshdata, RenderDevice.get(), sampler);
    auto m_AOTexture = LoadAOTexture(meshdata, RenderDevice.get(), sampler);

    auto cube = vCreateActor<VCube>(); // Create Cube Entity

    cube->AddComponentVoid<VMeshComponent>(); // Add a MeshComponent for Render
    cube->AddComponentVoid<VTransformComponent>(); // Add a TransformComponent for Render
    cube->AddComponentVoid<VMaterialComponent>(); // Add a MaterialComponent for Render

    VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = RenderDevice->CreateMesh(cubeCreateInfo);

    cube->GetComponent<VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent

    cube->GetComponent<VMeshComponent>()->GetMesh()->SetPositions(positions);
    cube->GetComponent<VMeshComponent>()->GetMesh()->SetNormals(normals);
    cube->GetComponent<VMeshComponent>()->GetMesh()->SetUVs(texCoords);
    cube->GetComponent<VMeshComponent>()->GetMesh()->SetIndices(indicies);
    
    cube->GetComponent<VMeshComponent>()->GetMesh()->Finalize();

    auto cubeMaterial = VMaterialLibrary::Instance().CreateMaterial("VDefault");

    // // Give the Material the Sampler Textures
    // // Diffuse
    cubeMaterial->SetTexture("VTextureDiffuse", m_DiffuseTexture.get(), 1, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerDiffuse);
    cubeMaterial->SetTexture("VTextureNormal", m_NormalTexture.get(), 2, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerNormal);
    cubeMaterial->SetTexture("VTextureMR", m_metallicRoughnessTexture.get(), 3, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMR);
    cubeMaterial->SetTexture("VTextureAO", m_metallicRoughnessTexture.get(), 4, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerAO);
    // // Specular
    // cubeMaterial->SetTexture("VTextureSpecular", m_SpecularTexture.get(), 2);

    cube->GetComponent<VMaterialComponent>()->SetMaterial(cubeMaterial.get());

    cube->GetComponent<VTransformComponent>()->SetPosition(VVector3(1.0f, 1.0f, 1.0f));
    cube->GetComponent<VTransformComponent>()->SetScale({0.1f, 0.1f, 0.1f});
    // Point Lights 
    VPointLightData pointLight1;
    pointLight1.position = { 2.0f, 2.0f, 2.0f }; // Slightly above and in front of the object
    pointLight1.ambient  = { 0.05f, 0.05f, 0.05f };
    pointLight1.diffuse  = {  1.0f, 1.0f, 1.0f };  // Full white diffuse for brightness
    pointLight1.specular = { 0.1f, 0.1f, 0.1f };
    pointLight1.constant = 1.0f;
    pointLight1.linear   = 0.09f;
    pointLight1.quadratic = 0.032f;
    pointLight1.radius = 5.0f;

    glfwSwapInterval(0); // VSync OFF : TODO: Implement in Context

    // RenderPath Test
    auto renderpath = RenderDevice->CreateRenderPath3D();

    // Properly initialize the camera with perspective projection
    camera->SetPerspective(45.0f, (float)Vantor::VServiceLocator::GetContextWidth()/(float)Vantor::VServiceLocator::GetContextHeight(), 0.1f, 100.0f);
    
    renderpath->SetCamera(camera.get());

    // renderpath->SetWireframeMode(true); // Re-enable wireframe for debugging
    // ==== Input ====
    // Set up InputDevice
    auto deviceone = Vantor::Input::CreateInputDevice(app.GetWindow()); // TODO: Write vCreateInputDevice(VWindow* window); function
    app.GetInputManager()->AddDevice(deviceone);

    app.GetInputManager()->MapAction("fire", VInputButton{VInputDeviceType::Keyboard, (int)VInputKey::KEY_ESCAPE});

    float rotationAngle = 0.0f;
    // Tilt the object down 45 degrees around the X axis
    VQuaternion tilt = VQuaternion::FromAxisAngle(VVector3{1, 0, 0}, -40.0f);

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

            if (app.GetInputManager()->WasPressed(VInputKey::KEY_ESCAPE, VInputDeviceType::Keyboard)) {
                app.Break();
            }

            if (app.GetInputManager()->IsPressed(VInputKey::KEY_W, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_FORWARD);
            } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_A, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_LEFT);
            } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_D, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_RIGHT);
            } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_S, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_BACK);
            } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_E, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_UP);
            } else if (app.GetInputManager()->IsPressed(VInputKey::KEY_Q, VInputDeviceType::Keyboard)) {
                camera->InputKey(app.GetDeltaTime(), VCameraMovement::CAMERA_DOWN);
            } 
            
            if (app.GetInputManager()->WasPressed(VInputKey::KEY_TAB, VInputDeviceType::Keyboard)) {
                app.GetInputManager()->devices[0]->SetMouseCursorState(!app.GetInputManager()->devices[0]->GetMouseCursorState());
            }

            // See if Cursor is disabled
            if (!app.GetInputManager()->devices[0]->GetMouseCursorState()) {
                mouseDelta = app.GetInputManager()->devices[0]->GetMouseDelta();
                camera->InputMouse(mouseDelta.x, -mouseDelta.y);
            }
            camera->Update(app.GetDeltaTime());

            rotationAngle += app.GetDeltaTime() * 50.0f;

            // Now spin around the tilted "up" vector (originally Y)
            VVector3 upAfterTilt = tilt.Rotate(VVector3{0, 1, 0});

            // Rotate 90 degrees around this new "up" axis
            VQuaternion spin = VQuaternion::FromAxisAngle(upAfterTilt, rotationAngle);

            // Final combined rotation
            VQuaternion finalRotation = tilt * spin;


            cube->GetComponent<VTransformComponent>()->SetRotation(finalRotation);
            // Our Render Pushes
            renderpath->PushRender(cube.get());

            renderpath->PushPointLight(pointLight1);

            renderpath->Render();

            app.EndFrame();
    }

    renderpath->Shutdown();

	app.Shutdown();

    return 0;
}