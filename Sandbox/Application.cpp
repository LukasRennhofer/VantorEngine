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

    app.Initialize(appInfo);

    VVector2 mouseDelta;
    auto camera = vCreateActor<VFlyCamera>(VVector3(1.0f, 1.0f, 1.0f));
    camera->MouseSensitivty  = 0.05f;

    VTextureSampler sampler;
    sampler.minFilter       = VTextureFilter::Linear;
    sampler.magFilter       = VTextureFilter::Linear;
    sampler.wrapS           = VTextureWrap::Repeat;
    sampler.wrapT           = VTextureWrap::Repeat;
    sampler.generateMipmaps = false;

    auto m_DiffuseTexture = vLoadTexture2D("AlbedoSphere", "Resources/textures/checkerboard.png", sampler, false)->GetTexture();
    auto m_NormalTexture = vLoadTexture2D("NormalSphere", "Resources/textures/pbr/dull-brass/normal.png", sampler, false)->GetTexture();
    auto m_metallicTexture = vLoadTexture2D("MetallicSphere", "Resources/textures/pbr/dull-brass/metallic.png", sampler, false)->GetTexture();
    auto m_roughnessTexture = vLoadTexture2D("RoughnessSphere", "Resources/textures/pbr/dull-brass/roughness.png", sampler, false)->GetTexture();
    auto m_AOTexture = vLoadTexture2D("AOSphere", "Resources/textures/pbr/dull-brass/ao.png", sampler, false)->GetTexture();

    auto cube = vCreateActor<VSphere>(); // Create Cube Entity

    cube->AddComponentVoid<VMeshComponent>(); // Add a MeshComponent for Render
    cube->AddComponentVoid<VTransformComponent>(); // Add a TransformComponent for Render
    cube->AddComponentVoid<VMaterialComponent>(); // Add a MaterialComponent for Render

    VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = RenderDevice->CreateMesh(cubeCreateInfo);

    cube->GetComponent<VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent

    cube->GenerateMesh();

    auto cubeMaterial = VMaterialLibrary::Instance().CreateMaterial("VDefault");

    cubeMaterial->SetTexture("VTextureDiffuse", m_DiffuseTexture.get(), 1, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerDiffuse);
    cubeMaterial->SetTexture("VTextureNormal", m_NormalTexture.get(), 2, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerNormal);
    cubeMaterial->SetTexture("VTextureMetallic", m_metallicTexture.get(), 3, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMR);
    cubeMaterial->SetTexture("VTextureRoughness", m_roughnessTexture.get(), 4, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMR);
    cubeMaterial->SetTexture("VTextureAO", m_AOTexture.get(), 5, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerAO);

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

    auto m_AppendCube = VResourceManager::Instance().GetTexture2D("VDeffered");

    glfwSwapInterval(0); // VSync OFF : TODO: Implement in Context

    // RenderPath Test
    auto renderpath = RenderDevice->CreateRenderPath3D();

    vStudioInitialize();

    camera->SetPerspective(45.0f, (float)Vantor::VServiceLocator::GetContextWidth()/(float)Vantor::VServiceLocator::GetContextHeight(), 0.1f, 100.0f);
    
    renderpath->SetCamera(camera.get());

    auto deviceone = Vantor::Input::CreateInputDevice(app.GetWindow()); // TODO: Write vCreateInputDevice(VWindow* window); function
    app.GetInputManager()->AddDevice(deviceone);

    app.GetInputManager()->MapAction("fire", VInputButton{VInputDeviceType::Keyboard, (int)VInputKey::KEY_ESCAPE});

    float rotationAngle = 0.0f;
    VQuaternion tilt = VQuaternion::FromAxisAngle(VVector3{1, 0, 0}, -40.0f);

    while (app.IsRunning()) {
            app.BeginFrame();

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

            if (!app.GetInputManager()->devices[0]->GetMouseCursorState()) {
                mouseDelta = app.GetInputManager()->devices[0]->GetMouseDelta();
                camera->InputMouse(mouseDelta.x, -mouseDelta.y);
            }
            camera->Update(app.GetDeltaTime());

            renderpath->PushRender(cube.get());

            renderpath->PushPointLight(pointLight1);

            renderpath->Render();

            vStudioRender(renderpath->GetOutBuffer().get());

            // #ifdef VANTOR_INTEGRATION_IMGUI
            // // To enable it only when Tab is pressed
            // if (app.GetInputManager()->devices[0]->GetMouseCursorState()) {
            //     Vantor::Integration::Imgui::BuildUpDocking();
            //     Vantor::Integration::Imgui::ShowSceneHierachy();
            //     ImGui::Begin("Camera");
            //     ImGui::Text("X: %f Y: %f Z: %f", camera->Position.x, camera->Position.y, camera->Position.z);
            //     ImGui::Text("FPS: %f", 1.0f/app.GetDeltaTime());
            //     ImGui::End();

            //     ImGui::Begin("Viewport");
            //     auto texture = renderpath->GetOutBuffer()->GetColorTexture(0)->getID();
            //     ImGui::Image((void*)(intptr_t)texture, ImVec2((float)1920, (float)1080));
            //     ImGui::End();
            // }
            // #endif

            app.EndFrame();
    }

    renderpath->Shutdown();

    vStudioShutdown();

	app.Shutdown();

    return 0;
}