#include <Vantor/Vantor.hpp>
#include <Vantor/Math/Linear.hpp> // Linear Maths : Vector, Matrix
#include <Vantor/Renderer/Geometry.hpp> // Basic Geometry Figures

int main() {
    // Create a OpenGL RenderDevice (VANTOR_API_OPENGL must be compiled)
    auto RenderDevice = vCreateRenderDevice(VRenderAPI::OPENGL);

    Vantor::VApplication app; // This is our VApplication object class

    // Application Creation Data (as a struct)
    Vantor::VApplicationCreateInfo appInfo;
    appInfo.windowWidth = 1280; // Window Width
    appInfo.windowHeight = 720; // Window Height
    appInfo.windowTitle = "MyVantorApplication"; // windowTitle

    // Initialize the Application
    app.Initialize(appInfo);

    // Creating the RenderPath3D to render 3D Graphics
    auto RenderPath3D = RenderDevice->CreateRenderPath3D();

    // Creating a Camera
    auto camera = vCreateActor<VCamera>(VVector3(1.0f, 1.0f, 1.0f));
    // Set the Perspective of the Camera
    camera->SetPerspective(45.0f, (float)Vantor::VServiceLocator::GetContextWidth()/(float)Vantor::VServiceLocator::GetContextHeight(), 0.1f, 100.0f);

    // Our Sampler for the textures we load
    VTextureSampler sampler;
    sampler.minFilter       = VTextureFilter::Linear;
    sampler.magFilter       = VTextureFilter::Linear;
    sampler.wrapS           = VTextureWrap::Repeat;
    sampler.wrapT           = VTextureWrap::Repeat;
    sampler.generateMipmaps = false;

    // Loading all the Textures for the Sphere
    auto m_DiffuseTexture = vLoadTexture2D("AlbedoSphere", "Resources/textures/checkerboard.png", sampler, false)->GetTexture();
    auto m_NormalTexture = vLoadTexture2D("NormalSphere", "Resources/textures/pbr/dull-brass/normal.png", sampler, false)->GetTexture();
    auto m_metallicTexture = vLoadTexture2D("MetallicSphere", "Resources/textures/pbr/dull-brass/metallic.png", sampler, false)->GetTexture();
    auto m_roughnessTexture = vLoadTexture2D("RoughnessSphere", "Resources/textures/pbr/dull-brass/roughness.png", sampler, false)->GetTexture();
    auto m_AOTexture = vLoadTexture2D("AOSphere", "Resources/textures/pbr/dull-brass/ao.png", sampler, false)->GetTexture();

    // Creating a Sphere
    auto sphere = vCreateActor<VSphere>();

    sphere->AddComponentVoid<VTransformComponent>(); // Adding a Transform Component to the sphere
    sphere->AddComponentVoid<VMaterialComponent>(); // Adding a Material Component to the sphere

    // Creating a Material for the Sphere (so textures, material properties like roughness, metallic, etc.)
    auto sphereMaterial = VMaterialLibrary::Instance().CreateMaterial("VDefault");

    // Now we set the loaded textures to the material
    sphereMaterial->SetTexture("VTextureDiffuse", m_DiffuseTexture.get(), 1, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerDiffuse);
    sphereMaterial->SetTexture("VTextureNormal", m_NormalTexture.get(), 2, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerNormal);
    sphereMaterial->SetTexture("VTextureMetallic", m_metallicTexture.get(), 3, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerMetallic);
    sphereMaterial->SetTexture("VTextureRoughness", m_roughnessTexture.get(), 4, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerRoughness);
    sphereMaterial->SetTexture("VTextureAO", m_AOTexture.get(), 5, VUniformType::UniformTypeSAMPLER2D, VSamplerType::SamplerAO);

    sphere->GetComponent<VMaterialComponent>()->SetMaterial(sphereMaterial.get());

    sphere->GetComponent<VTransformComponent>()->SetPosition(VVector3(1.0f, 1.0f, 1.0f));
    sphere->GetComponent<VTransformComponent>()->SetScale({0.1f, 0.1f, 0.1f});

    // Now we create PointLight Data, that we will push to the RenderPath
    VPointLightData pointLight1;
    pointLight1.position = { 2.0f, 2.0f, 2.0f }; // Slightly above and in front of the object
    pointLight1.ambient  = { 0.05f, 0.05f, 0.05f };
    pointLight1.diffuse  = {  1.0f, 1.0f, 1.0f };  // Full white diffuse for brightness
    pointLight1.specular = { 0.1f, 0.1f, 0.1f };
    pointLight1.constant = 1.0f;
    pointLight1.linear   = 0.09f;
    pointLight1.quadratic = 0.032f;
    pointLight1.radius = 5.0f;

    VAmbientLightData ambientLight {
        .ambientColor = VVector3(0.02f, 0.02f, 0.05f)
    };

    VDirectionalLightData sunLight {
        .direction = VVector3(-0.3f, -1.0f, -0.5f),
        .ambient   = VVector3(0.05f, 0.05f, 0.1f),
        .diffuse   = VVector3(0.6f, 0.6f, 0.9f),
        .specular  = VVector3(0.3f, 0.3f, 0.4f)
    };

    VSpotLightData spotLight {
        .position     = VVector3(0.0f, 1.8f, 2.0f),
        .constant     = 1.0f,
        .direction    = VVector3(0.0f, -0.3f, -1.0f).Normalized(),
        .linear       = 0.09f,
        .ambient      = VVector3(0.02f, 0.02f, 0.02f),
        .quadratic    = 0.032f,
        .diffuse      = VVector3(1.0f, 1.0f, 0.8f),
        .cutOff       = std::cos(Vantor::Math::DegToRad(12.5f)),
        .specular     = VVector3(1.0f, 1.0f, 1.0f),
        .outerCutOff  = std::cos(Vantor::Math::DegToRad(17.5f)),
        .radius       = 15.0f
    };

    // Push the Camera to the RenderPath
    RenderPath3D->SetCamera(camera.get());

    // Run app logic
    while (app.IsRunning()) {
            app.BeginFrame();

            // Push the Sphere to the RenderPath for rendering
            RenderPath3D->PushRender(sphere.get());
            // Push the PointLight to the RenderPath for rendering
            // RenderPath3D->PushPointLight(pointLight1);
            RenderPath3D->PushDirectionalLight(sunLight);
            // Render the Scene
            RenderPath3D->Render();

            app.EndFrame();
    }

	app.Shutdown();

    return 0;
}