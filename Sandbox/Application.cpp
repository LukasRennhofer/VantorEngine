/****************************************************************************
 * Vantor Engine™ - Deferred PBR 3D Demo with Atmosphere (2025)
 ****************************************************************************/

#include <Vantor/Vantor.hpp>
#include <RHI/OpenGL/VRHI_OpenGLRenderPass.hpp>
#include <iostream>
#include <memory>

auto device = VE::Internal::RHI::VRDCoordinator::Instance().CreateDevice(VE::Internal::RHI::EGraphicsAPI::OPENGL);

struct Vertex {
    VE::Internal::Math::VVector3 position;
    VE::Internal::Math::VVector3 normal;
    VE::Internal::Math::VVector2 uv;
};

void ResizeCallback(int w, int h) {
    device->SetViewport(0, 0, w, h);
}

int main() {
    const uint32_t width = 1920, height = 1080;
    VE::Internal::Context::VWindow* window = new VE::Internal::Context::VWindow(width, height, "Deferred PBR 3D Demo");

    if(!device->Initialize()) return 1;
    window->setResizeCallback(ResizeCallback);
    device->SetViewport(0, 0, width, height);

    // --- Camera ---
    VE::Internal::RenderModules::AFlyCamera camera(
        VE::Internal::Math::VVector3(1.0f,0.0f,3.0f),
        VE::Internal::Math::VVector3(0.0f,0.0f,-1.0f),
        VE::Internal::Math::VVector3(0.0f,1.0f,0.0f)
    );
    camera.SetPerspective(60.0f, float(width)/float(height), 0.1f, 100.0f);
    camera.UpdateView();

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
    auto mesh = device->CreateMesh(vertices.data(),vertices.size()*sizeof(Vertex),indices.data(),indices.size(),layout);

    // --- G-buffer textures ---
    auto gPosition = device->CreateTexture(width,height,VE::Internal::RHI::ERHIFormat::R16G16B16A16_FLOAT);
    auto gNormal   = device->CreateTexture(width,height,VE::Internal::RHI::ERHIFormat::R16G16B16A16_FLOAT);
    auto gAlbedo   = device->CreateTexture(width,height,VE::Internal::RHI::ERHIFormat::R8G8B8A8_UNORM); // RGB = albedo, A = metallic
    auto gRoughness = device->CreateTexture(width,height,VE::Internal::RHI::ERHIFormat::R8_UNORM);
    auto gDepth    = device->CreateTexture(width,height,VE::Internal::RHI::ERHIFormat::D32_FLOAT);

    auto gBufferRT = device->CreateRenderTarget(width,height);
    gBufferRT->AttachTexture(VE::Internal::RHI::EAttachmentType::Color,gPosition,0,0);
    gBufferRT->AttachTexture(VE::Internal::RHI::EAttachmentType::Color,gNormal,0,1);
    gBufferRT->AttachTexture(VE::Internal::RHI::EAttachmentType::Color,gAlbedo,0,2);
    gBufferRT->AttachTexture(VE::Internal::RHI::EAttachmentType::Color,gRoughness,0,3);
    gBufferRT->AttachTexture(VE::Internal::RHI::EAttachmentType::Depth,gDepth);

    // --- G-buffer shader ---
    std::string gVert = R"(
        #version 450 core
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNormal;
        layout(location=2) in vec2 aUV;
        uniform mat4 uView;
        uniform mat4 uProj;
        out vec3 FragPos;
        out vec3 Normal;
        out vec2 UV;
        void main() {
            FragPos = aPos;
            Normal = normalize(aNormal);
            UV = aUV;
            gl_Position = uProj*uView*vec4(aPos,1.0);
        }
    )";

    std::string gFrag = R"(
        #version 450 core
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 UV;

        layout(location=0) out vec4 gPosition;
        layout(location=1) out vec4 gNormal;
        layout(location=2) out vec4 gAlbedo;
        layout(location=3) out float gRoughness;

        void main() {
            gPosition = vec4(FragPos,1.0);
            gNormal = vec4(normalize(Normal),1.0);
            gAlbedo = vec4(vec3(1.0,0.0,0.0),0.5);
            gRoughness = 0.5;
        }
    )";

    auto gBufferShader = device->CreateShader(gVert,gFrag);

    auto gBufferPass = std::make_shared<VE::Internal::RHI::OpenGLRenderPass>();
    gBufferPass->SetRenderDevice(device);
    gBufferPass->AttachOutput(gBufferRT);
    gBufferPass->SetExecuteCallback([&](std::shared_ptr<VE::Internal::RHI::IRHIDevice> dev){
        dev->BindShader(gBufferShader);
        gBufferShader->SetMat4("uView",camera.View);
        gBufferShader->SetMat4("uProj",camera.Projection);
        dev->DrawMesh(mesh);
    });

    // --- Fullscreen quad for final & atmosphere passes ---
    struct QuadVertex { VE::Internal::Math::VVector2 pos, uv; };
    QuadVertex quadVerts[] = { {{-1,-1},{0,0}},{{1,-1},{1,0}},{{1,1},{1,1}},{{-1,1},{0,1}} };
    uint32_t quadIndices[] = {0,1,2,2,3,0};
    VE::Internal::RHI::VVertexLayout quadLayout;
    quadLayout.stride = sizeof(QuadVertex);
    quadLayout.attributes = {
        {0,VE::Internal::RHI::ERHIFormat::R32G32_FLOAT,offsetof(QuadVertex,pos)},
        {1,VE::Internal::RHI::ERHIFormat::R32G32_FLOAT,offsetof(QuadVertex,uv)}
    };
    auto quadMesh = device->CreateMesh(quadVerts,4*sizeof(QuadVertex),quadIndices,6,quadLayout);

    // --- Atmosphere pass ---
    std::string atmosphereVert = R"(
        #version 450 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec2 aUV;
        out vec2 fragUV;
        void main() { fragUV = aUV; gl_Position = vec4(aPos,0,1); }
    )";

    std::string atmosphereFrag = R"(
        #version 450 core
        in vec2 fragUV;
        out vec4 FragColor;
        uniform float time;

        void main() {
            float t = fragUV.y;
            vec3 skyColor = mix(vec3(0.6,0.8,1.0), vec3(0.02,0.05,0.2), pow(t,1.5));
            float cloud = sin(fragUV.x*10.0 + time*0.1) * sin(fragUV.y*10.0 + time*0.1);
            skyColor += vec3(cloud*0.05);
            float fog = exp(-fragUV.y*3.0);
            skyColor = mix(vec3(0.1,0.1,0.12), skyColor, fog);
            FragColor = vec4(skyColor,1.0);
        }
    )";

    auto atmosphereShader = device->CreateShader(atmosphereVert,atmosphereFrag);

    auto atmospherePass = std::make_shared<VE::Internal::RHI::OpenGLRenderPass>();
    atmospherePass->SetRenderDevice(device);
    atmospherePass->AttachOutput(nullptr);
    atmospherePass->SetExecuteCallback([&](std::shared_ptr<VE::Internal::RHI::IRHIDevice> dev){
        dev->BindShader(atmosphereShader);
        atmosphereShader->SetFloat("time", static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count()) * 0.001f);
        dev->DrawMesh(quadMesh);
    });

    // --- Final PBR pass ---
    std::string finalVert = R"(
        #version 450 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec2 aUV;
        out vec2 fragUV;
        void main() { fragUV=aUV; gl_Position=vec4(aPos,0,1); }
    )";

    std::string finalFrag = R"(
        #version 450 core
        in vec2 fragUV;
        layout(binding = 0) uniform sampler2D gPosition;
        layout(binding = 1) uniform sampler2D gNormal;
        layout(binding = 2) uniform sampler2D gAlbedo;
        layout(binding = 3) uniform sampler2D gRoughness;
        out vec4 FragColor;

        const vec3 lightDir = normalize(vec3(-1.0,-1.0,-1.0));
        const vec3 lightColor = vec3(1.0);
        const vec3 camPos = vec3(0.0,0.0,3.0);
        const float PI = 3.14159265359;

        float DistributionGGX(vec3 N, vec3 H, float roughness){
            float a = roughness*roughness;
            float a2 = a*a;
            float NdotH = max(dot(N,H),0.0);
            float NdotH2 = NdotH*NdotH;
            float nom = a2;
            float denom = (NdotH2*(a2-1.0)+1.0);
            denom = PI*denom*denom;
            return nom/denom;
        }
        float GeometrySchlickGGX(float NdotV, float roughness){
            float r = roughness + 1.0;
            float k = (r*r)/8.0;
            return NdotV/(NdotV*(1.0-k)+k);
        }
        float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
            float NdotV = max(dot(N,V),0.0);
            float NdotL = max(dot(N,L),0.0);
            float ggx1 = GeometrySchlickGGX(NdotV,roughness);
            float ggx2 = GeometrySchlickGGX(NdotL,roughness);
            return ggx1*ggx2;
        }
        vec3 FresnelSchlick(float cosTheta, vec3 F0){ return F0 + (1.0-F0)*pow(1.0-cosTheta,5.0); }

        void main() {
            vec3 pos = texture(gPosition,fragUV).rgb;
            vec3 N = normalize(texture(gNormal,fragUV).rgb);
            vec3 albedo = texture(gAlbedo,fragUV).rgb;
            float metallic = texture(gAlbedo,fragUV).a;
            float roughness = texture(gRoughness,fragUV).r;

            vec3 V = normalize(camPos - pos);
            vec3 L = normalize(-lightDir);
            vec3 H = normalize(V+L);
            float distance = 1.0;
            float attenuation = 1.0 / (distance*distance);
            vec3 radiance = lightColor * attenuation;

            float NDF = DistributionGGX(N,H,roughness);
            float G   = GeometrySmith(N,V,L,roughness);
            vec3 F0   = mix(vec3(0.04), albedo, metallic);
            vec3 F    = FresnelSchlick(max(dot(H,V),0.0),F0);

            vec3 nominator = NDF * G * F;
            float denom = 4*max(dot(N,V),0.0)*max(dot(N,L),0.0) + 0.001;
            vec3 specular = nominator / denom;

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;

            float NdotL = max(dot(N,L),0.0);
            vec3 Lo = (kD*albedo/PI + specular) * radiance * NdotL;

            FragColor = vec4(Lo,1.0);
        }
    )";

    auto finalShader = device->CreateShader(finalVert,finalFrag);

    auto finalPass = std::make_shared<VE::Internal::RHI::OpenGLRenderPass>();
    finalPass->SetRenderDevice(device);
    finalPass->AttachOutput(nullptr);
    finalPass->AttachInput(gPosition,0);
    finalPass->AttachInput(gNormal,1);
    finalPass->AttachInput(gAlbedo,2);
    finalPass->AttachInput(gRoughness,3);
    finalPass->SetExecuteCallback([&](std::shared_ptr<VE::Internal::RHI::IRHIDevice> dev){
        dev->BindShader(finalShader);
        dev->BindTexture(gPosition,0);
        dev->BindTexture(gNormal,1);
        dev->BindTexture(gAlbedo,2);
        dev->BindTexture(gRoughness,3);
        dev->DrawMesh(quadMesh);
    });

    VE::Internal::Core::VTimer timerN;

    // --- Main loop ---
    while (!window->shouldWindowClose()) {
        window->pollEvents();
        camera.Update(0.016f);

        device->Clear(0,0,0,1);

        gBufferPass->Execute();
        atmospherePass->Execute(); // <-- draw sky first
        finalPass->Execute();

        window->swapBuffers();
    }

    device->Shutdown();
    window->close();
    return 0;
}
