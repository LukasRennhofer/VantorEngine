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

    MeshData meshdata = LoadMeshFromGLTF("Resources/meshes/assault_rifle/scene.gltf");

    std::vector<VVector3> positions = meshdata.positions;
    std::vector<VVector3> normals = meshdata.normals;
    std::vector<VVector2> texCoords = meshdata.uvs;
    std::vector<unsigned int> indicies = meshdata.indices;

    meshdata.diffuseTexturePath = "Resources/meshes/assault_rifle/" + meshdata.diffuseTexturePath;
    meshdata.metallicTexturePath = "Resources/meshes/assault_rifle/" + meshdata.metallicTexturePath;
    meshdata.normalTexturePath = "Resources/meshes/assault_rifle/" + meshdata.normalTexturePath;
    meshdata.roughnessTexturePath = "Resources/meshes/assault_rifle/" + meshdata.roughnessTexturePath;
    
    // auto m_ResDeffered = vLoadTexture2D("m_Deffered", "Resources/textures/container2.png", sampler, false);
    // auto m_ResSpecular = vLoadTexture2D("m_Specular", "Resources/textures/container2_specular.png", sampler, false);

    auto m_DiffuseTexture = LoadDiffuseTexture(meshdata, RenderDevice.get(), sampler);
    auto m_NormalTexture = LoadNormalTexture(meshdata, RenderDevice.get(), sampler);
    auto m_metallicTexture = LoadMetallicTexture(meshdata, RenderDevice.get(), sampler);
    auto m_RoughnessTexture = LoadRoughnessTexture(meshdata, RenderDevice.get(), sampler);
    auto m_AOTexture = LoadAOTexture(meshdata, RenderDevice.get(), sampler);

    auto cube = vCreateActor<VObject>(); // Create Entity

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

    cubeMaterial->SetTexture("VTextureDiffuse", m_DiffuseTexture.get(), 1, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerDiffuse);
    cubeMaterial->SetTexture("VTextureNormal", m_NormalTexture.get(), 2, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerNormal);
    cubeMaterial->SetTexture("VTextureMetallic", m_metallicTexture.get(), 3, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMR);
    cubeMaterial->SetTexture("VTextureRoughness", m_RoughnessTexture.get(), 4, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMR);
    // cubeMaterial->SetTexture("VTextureAO", m_AOTexture.get(), 5, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerAO);

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

            app.EndFrame();
    }

    renderpath->Shutdown();

    vStudioShutdown();

	app.Shutdown();

    return 0;
}