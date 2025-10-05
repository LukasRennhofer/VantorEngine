/****************************************************************************
 * Vantor Engine™ - Rotating Cube Demo with Motion Blur (2025)
 * Updated: Renders to render target with movement blur effect.
 ****************************************************************************/

#include <Vantor/Vantor.hpp>

#include <iostream>
#include <memory>
#include <chrono>

void ResizeCallback(int w, int h) {
    VE::GEngine()->GetDevice()->SetViewport(0, 0, w, h);
}

int main() {
    const uint32_t width = 1280, height = 720;
    VE::Internal::Context::VWindow* window = new VE::Internal::Context::VWindow(width, height, "Vantor Engine Sandbox - Motion Blur");

    window->setResizeCallback(ResizeCallback);
    VE::GEngine()->GetDevice()->SetViewport(0, 0, width, height);

    std::shared_ptr<VE::Render::VRenderPath3D> RenderPath = std::make_shared<VE::Render::VRenderPath3D>();

    RenderPath->Initialize(VE::GEngine()->GetDevice());

    VE::Internal::RenderModules::AFlyCamera cam(VE::Math::VVector3(0.0f, 0.0f, 3.0f));

    // Set up the camera's projection matrix  
    cam.SetPerspective(45.0f, (float)width / (float)height, 0.001f, 100.0f);
    cam.MovementSpeed = 0.01f;

    RenderPath->SetCamera(&cam);

    auto texture = VE::GEngine()->GetAssetMngr()->LoadTexture("Resources/meshes/assault_rifle/textures/Base_baseColor.png");

    if (!texture->CreateRHITexture(VE::GEngine()->GetDevice())) {
        VE::Internal::Core::Backlog::Log("Application", "RHI Asset Texture could not be created");
    }

    auto testShader = VE::GEngine()->GetAssetMngr()->LoadText("VForward.txt");

    // InputDevice
    auto inputDevice = VE::Input::CreateInputDevice(window);

    VE::GEngine()->GetInputMngr()->AddDevice(inputDevice);
   
    auto modelasset = VE::GEngine()->GetAssetMngr()->LoadModel("Resources/meshes/assault_rifle/scene.gltf");

    if (modelasset->CreateGPUResources(VE::GEngine()->GetDevice())) {
        std::cout << "GPU Resource successfully created!" << std::endl;
    }

    // TODO: Move away to context
    // glfwSwapInterval(0); // Turn VSync off

    // --- PBR Shader ---
    std::string pbrVert = R"(
        #version 450 core

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        uniform vec3 uCameraPos;

        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNormal;
        layout(location=2) in vec2 aUV;
        layout(location=3) in vec3 aTangent;
        layout(location=4) in vec3 aBitangent;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 UV;
        out vec3 CameraPos;
        out mat3 TBN;

        void main() {
            vec4 worldPos = uModel * vec4(aPos, 1.0);
            FragPos = worldPos.xyz;
            
            // Transform normal, tangent, and bitangent to world space
            mat3 normalMatrix = mat3(transpose(inverse(uModel)));
            Normal = normalize(normalMatrix * aNormal);
            vec3 Tangent = normalize(normalMatrix * aTangent);
            vec3 Bitangent = normalize(normalMatrix * aBitangent);
            
            // Create TBN matrix for normal mapping
            TBN = mat3(Tangent, Bitangent, Normal);
            
            UV = aUV;
            CameraPos = uCameraPos;
            
            gl_Position = uProj * uView * worldPos;
        }
    )";

    std::string pbrFrag = R"(
        #version 450 core
        
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 UV;
        in vec3 CameraPos;
        in mat3 TBN;
        
        out vec4 FragColor;
        
        // PBR Material uniforms
        uniform sampler2D uAlbedoMap;
        uniform sampler2D uMetallicRoughnessMap;  // R=?, G=Roughness, B=Metallic
        uniform sampler2D uNormalMap;
        uniform sampler2D uAOMap;                 // Ambient Occlusion (optional)
        
        // Material parameters
        uniform vec3 uAlbedo;
        uniform float uMetallic;
        uniform float uRoughness;
        uniform float uAO;
        
        // Lighting
        uniform vec3 uLightPositions[4];
        uniform vec3 uLightColors[4];
        uniform int uNumLights;
        
        const float PI = 3.14159265359;
        
        // Normal Distribution Function (GGX/Trowbridge-Reitz)
        float DistributionGGX(vec3 N, vec3 H, float roughness) {
            float a = roughness * roughness;
            float a2 = a * a;
            float NdotH = max(dot(N, H), 0.0);
            float NdotH2 = NdotH * NdotH;
            
            float nom = a2;
            float denom = (NdotH2 * (a2 - 1.0) + 1.0);
            denom = PI * denom * denom;
            
            return nom / denom;
        }
        
        // Geometry Function (Smith's method)
        float GeometrySchlickGGX(float NdotV, float roughness) {
            float r = (roughness + 1.0);
            float k = (r * r) / 8.0;
            
            float nom = NdotV;
            float denom = NdotV * (1.0 - k) + k;
            
            return nom / denom;
        }
        
        float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
            float NdotV = max(dot(N, V), 0.0);
            float NdotL = max(dot(N, L), 0.0);
            float ggx2 = GeometrySchlickGGX(NdotV, roughness);
            float ggx1 = GeometrySchlickGGX(NdotL, roughness);
            
            return ggx1 * ggx2;
        }
        
        // Fresnel equation (Schlick's approximation)
        vec3 FresnelSchlick(float cosTheta, vec3 F0) {
            return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
        }
        
        void main() {
            // Sample material maps
            vec3 albedoSample = texture(uAlbedoMap, UV).rgb;
            
            // Simplified PBR without complex gamma correction
            vec3 albedo = albedoSample * uAlbedo;
            
            // DEBUG: Show ONLY the albedo texture, nothing else
            // FragColor = vec4(albedo, 1.0); return;
            
            float metallic = texture(uMetallicRoughnessMap, UV).b * uMetallic;
            float roughness = texture(uMetallicRoughnessMap, UV).g * uRoughness;
            float ao = texture(uAOMap, UV).r * uAO;
            
            // Use vertex normals only (no normal mapping)
            vec3 N = normalize(Normal);
            
            vec3 V = normalize(CameraPos - FragPos);
            
            vec3 F0 = vec3(0.04);
            F0 = mix(F0, albedo, metallic);
            
            vec3 Lo = vec3(0.0);
            
            // Calculate lighting for each light source
            for(int i = 0; i < uNumLights && i < 4; ++i) {
                vec3 L = normalize(uLightPositions[i] - FragPos);
                vec3 H = normalize(V + L);
                float distance = length(uLightPositions[i] - FragPos);
                float attenuation = 1.0 / (distance * distance);
                vec3 radiance = uLightColors[i] * attenuation;
                
                // BRDF components
                float NDF = DistributionGGX(N, H, roughness);
                float G = GeometrySmith(N, V, L, roughness);
                vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
                
                vec3 kS = F;
                vec3 kD = vec3(1.0) - kS;
                kD *= 1.0 - metallic;
                
                vec3 numerator = NDF * G * F;
                float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
                vec3 specular = numerator / denominator;
                
                float NdotL = max(dot(N, L), 0.0);
                Lo += (kD * albedo / PI + specular) * radiance * NdotL;
            }
            
            // Ambient lighting (simplified)
            vec3 ambient = vec3(0.1) * albedo * ao; // Increased ambient for better visibility
            vec3 color = ambient + Lo;
            
            // Simple clamping instead of complex tone mapping
            color = clamp(color, 0.0, 1.0);
            
            FragColor = vec4(color, 1.0);
        }
    )";

    auto pbrShader = VE::GEngine()->GetDevice()->CreateShader(pbrVert, pbrFrag);

    VE::VMaterial material(pbrShader.get());

    // Load additional PBR textures
    auto metallicRoughnessTexture = VE::GEngine()->GetAssetMngr()->LoadTexture("Resources/meshes/assault_rifle/textures/Base_metallicRoughness.png");
    auto normalTexture = VE::GEngine()->GetAssetMngr()->LoadTexture("Resources/meshes/assault_rifle/textures/Base_normal.png");
    auto aoTexture = VE::GEngine()->GetAssetMngr()->LoadTexture("Resources/textures/white.png"); // Default white AO map (no AO effect)
    
    // Create RHI textures
    if (!metallicRoughnessTexture->CreateRHITexture(VE::GEngine()->GetDevice())) {
        VE::Internal::Core::Backlog::Log("Application", "Failed to create metallic-roughness texture");
    }
    if (!normalTexture->CreateRHITexture(VE::GEngine()->GetDevice())) {
        VE::Internal::Core::Backlog::Log("Application", "Failed to create normal texture");
    }
    if (!aoTexture->CreateRHITexture(VE::GEngine()->GetDevice())) {
        VE::Internal::Core::Backlog::Log("Application", "Failed to create AO texture");
    }

    // Set PBR material parameters
    material.SetTexture("uAlbedoMap", texture->GetRHITexture().get(), 0);        // This actually binds to albedo slot
    material.SetTexture("uMetallicRoughnessMap", metallicRoughnessTexture->GetRHITexture().get(), 1);
    material.SetTexture("uNormalMap", normalTexture->GetRHITexture().get(), 2);              // This actually binds to normal slot  
    material.SetTexture("uAOMap", aoTexture->GetRHITexture().get(), 3);
    
    // Material properties - adjusted for better visual results
    material.SetVector("uAlbedo", VE::Math::VVector3(1.0f, 1.0f, 1.0f)); // White multiplier (let texture control color)
    material.SetFloat("uMetallic", 0.5f);  // Reduced from 1.0 - let texture control metallicness
    material.SetFloat("uRoughness", 0.5f); // Reduced from 1.0 - let texture control roughness
    material.SetFloat("uAO", 1.0f);
    
    // Lighting setup - 4 point lights
    VE::Math::VVector3 lightPositions[4] = {
        VE::Math::VVector3(2.0f, 2.0f, 2.0f),
    };
    
    VE::Math::VVector3 lightColors[4] = {
        VE::Math::VVector3(5.0f, 5.0f, 5.0f),    // Much lower intensity
    };
    
    // Set light uniforms
    for (int i = 0; i < 4; i++) {
        material.SetVector("uLightPositions[" + std::to_string(i) + "]", lightPositions[i]);
        material.SetVector("uLightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
    material.SetInt("uNumLights", 4);
    
    auto tStart = std::chrono::high_resolution_clock::now();
    auto lastFrameTime = tStart;
    
    // FPS calculation variables
    float frameTime = 0.0f;
    float fps = 0.0f;
    int frameCount = 0;
    float fpsUpdateTimer = 0.0f;
    const float fpsUpdateInterval = 0.5f; // Update FPS display every 0.5 seconds

    bool GameMode = true;

    inputDevice->SetMouseCursorState(!GameMode);

    while (!window->shouldWindowClose()) {
        VE::GEngine()->Update();

        window->pollEvents();
        auto tNow = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float>(tNow - tStart).count();
        
        // Calculate frame time and FPS
        frameTime = std::chrono::duration<float>(tNow - lastFrameTime).count();
        lastFrameTime = tNow;
        
        frameCount++;
        fpsUpdateTimer += frameTime;
        
        // Update FPS every fpsUpdateInterval seconds
        if (fpsUpdateTimer >= fpsUpdateInterval) {
            fps = frameCount / fpsUpdateTimer;
            frameCount = 0;
            fpsUpdateTimer = 0.0f;
        }

        if (VE::GEngine()->GetInputMngr()->WasPressed(
            static_cast<int>(VE::Input::EInputKey::KEY_TAB),
            VE::Input::EInputDeviceType::Keyboard)) {
            
            GameMode = !GameMode;

            inputDevice->SetMouseCursorState(!GameMode);
        }

        if (VE::GEngine()->GetInputMngr()->IsPressed(
            static_cast<int>(VE::Input::EInputKey::KEY_W),
            VE::Input::EInputDeviceType::Keyboard) && GameMode)
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_FORWARD);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_S),
                VE::Input::EInputDeviceType::Keyboard) && GameMode)
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_BACK);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_A),
                VE::Input::EInputDeviceType::Keyboard) && GameMode)
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_LEFT);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_D),
                VE::Input::EInputDeviceType::Keyboard) && GameMode)
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_RIGHT);
        }

        if (GameMode) {
            auto delta = inputDevice->GetMouseDelta();

            cam.InputMouse(delta.x, -delta.y);

        }

        cam.Update(time);

        // rotation around Y only so the cube always faces camera initially
        float yawDeg = time * 45.0f;
        
        VE::Math::VMat4 model = VE::Math::VMat4::RotationYaw(yawDeg);
        
        // Update camera position for PBR shader
        material.SetVector("uCameraPos", cam.Position);

        VE::GEngine()->GetDevice()->Clear(0.1f, 0.1f, 0.12f, 1.0f);
      
        RenderPath->PushRender(modelasset->GetModel().get(), &material, model);

        RenderPath->Render();

        // ImGui Render
        if (!GameMode) {
            VE::Internal::Integration::Imgui::NewFrame();

            ImGui::Begin("Performance Monitor");
            ImGui::Text("FPS: %.1f", fps);
            ImGui::Text("Frame Time: %.3f ms", frameTime * 1000.0f);
            ImGui::Text("Total Time: %.2f s", time);
            
            ImGui::Separator();
            ImGui::Text("Controls:");
            ImGui::Text("TAB - Toggle Game Mode");
            ImGui::Text("WASD - Move Camera (Game Mode)");
            ImGui::Text("Mouse - Look Around (Game Mode)");
            ImGui::End();

            VE::Internal::Integration::Imgui::Render();
        }

        window->swapBuffers();
    }

    VE::Internal::Integration::Imgui::Destroy();

    window->close();

    return 0;
}
