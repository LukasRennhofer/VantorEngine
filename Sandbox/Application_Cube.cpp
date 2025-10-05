/****************************************************************************
 * Vantor Engine™ - Rotating Cube Demo (2025)
 * Updated: Cube initially faces the camera directly.
 ****************************************************************************/

#include <Vantor/Vantor.hpp>

#include <iostream>
#include <memory>
#include <chrono>

struct Vertex {
    VE::Math::VVector3 position;
    VE::Math::VVector3 normal;
    VE::Math::VVector2 uv;
};

void ResizeCallback(int w, int h) {
    VE::GEngine()->GetDevice()->SetViewport(0, 0, w, h);
}

int main() {
    const uint32_t width = 1280, height = 720;
    VE::Internal::Context::VWindow* window = new VE::Internal::Context::VWindow(width, height, "Rotating Cube - Vantor");

    window->setResizeCallback(ResizeCallback);
    VE::GEngine()->GetDevice()->SetViewport(0, 0, width, height);

    std::shared_ptr<VE::Renderer::VRenderPath3D> RenderPath = std::make_shared<VE::Renderer::VRenderPath3D>();

    RenderPath->Initialize(VE::GEngine()->GetDevice());

    VE::Internal::RenderModules::AFlyCamera cam(VE::Math::VVector3(0.0f, 0.0f, 3.0f));

    // Set up the camera's projection matrix
    cam.SetPerspective(45.0f, (float)width / (float)height, 0.001f, 100.0f);
    cam.MovementSpeed = 0.1f;

    RenderPath->SetCamera(&cam);

    auto texture = VE::GEngine()->GetAssetMngr()->LoadTexture("checkerboard.png");

    if (!texture->CreateRHITexture(VE::GEngine()->GetDevice())) {
        VE::Internal::Core::Backlog::Log("Application", "RHI Asset Texture could not be created");
    }

    // InputDevice
    auto inputDevice = VE::Input::CreateInputDevice(window);

    VE::GEngine()->GetInputMngr()->AddDevice(inputDevice);
    // --- Cube mesh ---
    std::vector<Vertex> vertices = {
        {{-0.5f,-0.5f,-0.5f},{0,0,-1},{0,0}}, {{0.5f,-0.5f,-0.5f},{0,0,-1},{1,0}}, {{0.5f,0.5f,-0.5f},{0,0,-1},{1,1}}, {{-0.5f,0.5f,-0.5f},{0,0,-1},{0,1}},
        {{-0.5f,-0.5f,0.5f},{0,0,1},{0,0}}, {{0.5f,-0.5f,0.5f},{0,0,1},{1,0}}, {{0.5f,0.5f,0.5f},{0,0,1},{1,1}}, {{-0.5f,0.5f,0.5f},{0,0,1},{0,1}},
        {{-0.5f,-0.5f,-0.5f},{-1,0,0},{0,0}}, {{-0.5f,0.5f,-0.5f},{-1,0,0},{1,0}}, {{-0.5f,0.5f,0.5f},{-1,0,0},{1,1}}, {{-0.5f,-0.5f,0.5f},{-1,0,0},{0,1}},
        {{0.5f,-0.5f,-0.5f},{1,0,0},{0,0}}, {{0.5f,0.5f,-0.5f},{1,0,0},{1,0}}, {{0.5f,0.5f,0.5f},{1,0,0},{1,1}}, {{0.5f,-0.5f,0.5f},{1,0,0},{0,1}},
        {{-0.5f,-0.5f,-0.5f},{0,-1,0},{0,0}}, {{0.5f,-0.5f,-0.5f},{0,-1,0},{1,0}}, {{0.5f,-0.5f,0.5f},{0,-1,0},{1,1}}, {{-0.5f,-0.5f,0.5f},{0,-1,0},{0,1}},
        {{-0.5f,0.5f,-0.5f},{0,1,0},{0,0}}, {{0.5f,0.5f,-0.5f},{0,1,0},{1,0}}, {{0.5f,0.5f,0.5f},{0,1,0},{1,1}}, {{-0.5f,0.5f,0.5f},{0,1,0},{0,1}}
    };
    std::vector<uint32_t> indices = {
        0,1,2, 2,3,0, 4,5,6, 6,7,4, 8,9,10, 10,11,8,
        12,13,14, 14,15,12, 16,17,18, 18,19,16, 20,21,22, 22,23,20
    };

    VE::Internal::RHI::VVertexLayout layout;
    layout.stride = sizeof(Vertex);
    layout.attributes = {
        {0,VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT,offsetof(Vertex,position)},
        {1,VE::Internal::RHI::ERHIFormat::R32G32B32_FLOAT,offsetof(Vertex,normal)},
        {2,VE::Internal::RHI::ERHIFormat::R32G32_FLOAT,offsetof(Vertex,uv)}
    };
    auto mesh = VE::GEngine()->GetDevice()->CreateMesh(vertices.data(), (uint32_t)(vertices.size()*sizeof(Vertex)), indices.data(), (uint32_t)indices.size(), layout);

    // --- Shader ---
    std::string cubeVert = R"(
        #version 450 core

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;

        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNormal;
        layout(location=2) in vec2 aUV;

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 UV;

        void main() {
            vec4 worldPos = uModel * vec4(aPos, 1.0);
            FragPos = worldPos.xyz;
            Normal  = mat3(transpose(inverse(uModel))) * aNormal; // <-- safer
            UV      = aUV;
            gl_Position = uProj * uView * worldPos;
        }

    )";

    std::string cubeFrag = R"(
        #version 450 core
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 UV;
        out vec4 FragColor;
        uniform vec3 uLightDir;
        uniform vec3 uColor;
        uniform sampler2D uTexture;
        void main() {
            vec3 N = normalize(Normal);
            vec3 L = normalize(-uLightDir);
            float NdotL = max(dot(N,L), 0.0);
            
            // Sample the texture
            vec4 texColor = texture(uTexture, UV);
            
            // Combine texture color with lighting
            vec3 diffuse = NdotL * texColor.rgb;
            vec3 ambient = 0.15 * texColor.rgb;

            FragColor = vec4(diffuse + ambient, texColor.a);
        }
    )";

    auto cubeShader = VE::GEngine()->GetDevice()->CreateShader(cubeVert, cubeFrag);

    VE::VMaterial material(cubeShader.get());

    material.SetVector("uLightDir", VE::Math::VVector3(-1.0f,-1.0f,-1.0f));
    // material.SetVector("uColor", VE::Math::VVector3(0.0f, 1.0f, 0.0f));
    material.SetTexture("uTexture", texture->GetRHITexture().get());

    auto tStart = std::chrono::high_resolution_clock::now();

    inputDevice->SetMouseCursorState(false);

    while (!window->shouldWindowClose()) {
        VE::GEngine()->Update();

        window->pollEvents();
        auto tNow = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float>(tNow - tStart).count();

        if (VE::GEngine()->GetInputMngr()->IsPressed(
            static_cast<int>(VE::Input::EInputKey::KEY_W),
            VE::Input::EInputDeviceType::Keyboard))
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_FORWARD);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_S),
                VE::Input::EInputDeviceType::Keyboard))
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_BACK);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_A),
                VE::Input::EInputDeviceType::Keyboard))
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_LEFT);
        }
        else if (VE::GEngine()->GetInputMngr()->IsPressed(
                static_cast<int>(VE::Input::EInputKey::KEY_D),
                VE::Input::EInputDeviceType::Keyboard))
        {
            cam.InputKey(time, VE::Graphics::ECameraMovement::CAMERA_RIGHT);
        }

        auto delta = inputDevice->GetMouseDelta();

        cam.InputMouse(delta.x, -delta.y);

        cam.Update(time);

        // rotation around Y only so the cube always faces camera initially
        float yawDeg = time * 45.0f;
        VE::Math::VMat4 model = VE::Math::VMat4::RotationYaw(yawDeg);

        VE::GEngine()->GetDevice()->Clear(0.1f, 0.1f, 0.12f, 1.0f);
      
        RenderPath->PushRender(mesh.get(), &material, model);
        RenderPath->Render();

        window->swapBuffers();
    }
    window->close();
    return 0;
}
