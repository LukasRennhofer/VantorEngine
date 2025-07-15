/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-11
 *
 *  File: VRDO_RenderPath.cpp
 *  Last Change: Automatically updated
 */

#include <cstring>
#include <iostream>

#include "../../Resource/VRES_Manager.hpp"
#include "../Interface/VRD_RenderDevice.hpp"
#include "VRDO_RenderPath.hpp"
#include "VRDO_Shader.hpp"
#include "VRDO_Texture.hpp"
// Resource Manager
#include "../../Resource/VRES_Manager.hpp"

namespace Vantor::RenderDevice {
    // =============================================================================
    // VRenderPassGL Implementation
    // =============================================================================

    VRenderPassGL::VRenderPassGL(VERenderPassType type, const std::string &name) : m_Type(type), m_Name(name) {}

    // =============================================================================
    // VForwardRenderPassGL Implementation
    // =============================================================================

    VForwardRenderPassGL::VForwardRenderPassGL() : VRenderPassGL(VERenderPassType::Forward, "ForwardPass") {}

    void VForwardRenderPassGL::Initialize(VRenderPath3D* renderPath)
    {
       m_RenderPath = renderPath;
    }

    void VForwardRenderPassGL::Execute()
    {
        // Enable States
        VOpenGLStateChache::Instance().SetDepthTest(true);
        VOpenGLStateChache::Instance().SetDepthFunc(GL_LESS);

        VOpenGLStateChache::Instance().SetCull(true);
        VOpenGLStateChache::Instance().SetCullFace(GL_BACK);

        // Variables for Material
        bool MaterialUseTexture = false;

        // Go through all Meshes
        for (auto command : m_RenderPath->GetCommandBuffer()->GetForwardRenderCommands()) {
            command.Material->GetShader()->Use();

            // Should be the same in every Shader
            command.Material->GetShader()->setUniformMat4("model", command.Transform);

            // Handle Textures of Material
            auto* samplers = command.Material->GetSamplerUniforms();
            for (auto it = samplers->begin(); it != samplers->end(); ++it)
            {
                if (it->second.Type == Vantor::Renderer::UniformTypeSAMPLERCUBE) {
                    it->second.TextureCube->Bind(it->second.Unit);
                    MaterialUseTexture = true;
                }
                else {
                    it->second.Texture->Bind(it->second.Unit);
                    MaterialUseTexture = true;
                }
            }

            // Set all Uniforms of Material to its Shader : The error is here
            auto* uniforms = command.Material->GetUniforms();
            for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
            {
                std::cout << it->first << std::endl;

                switch (it->second.Type)
                {
                case Vantor::Renderer::UniformTypeBOOL:
                    command.Material->GetShader()->setUniformBool(it->first, it->second.Bool);
                    break;
                case Vantor::Renderer::UniformTypeINT:
                    command.Material->GetShader()->setUniformInt(it->first, it->second.Int);
                    break;
                case Vantor::Renderer::UniformTypeFLOAT:
                    command.Material->GetShader()->setUniformFloat(it->first, it->second.Float);
                    break;
                case Vantor::Renderer::UniformTypeVEC2:
                    command.Material->GetShader()->setUniformVec2(it->first, it->second.Vec2);
                    break;
                case Vantor::Renderer::UniformTypeVEC3:
                    command.Material->GetShader()->setUniformVec3(it->first, it->second.Vec3);
                    break;
                case Vantor::Renderer::UniformTypeVEC4:
                    command.Material->GetShader()->setUniformVec4(it->first, it->second.Vec4);
                    break;
                case Vantor::Renderer::UniformTypeMAT2:
                    command.Material->GetShader()->setUniformMat2(it->first, it->second.Mat2);
                    break;
                case Vantor::Renderer::UniformTypeMAT3:
                    command.Material->GetShader()->setUniformMat3(it->first, it->second.Mat3);
                    break;
                case Vantor::Renderer::UniformTypeMAT4:
                    command.Material->GetShader()->setUniformMat4(it->first, it->second.Mat4);
                    break;
                default:
                    Vantor::Backlog::Log("VRenderPath3DGL::ForwardPass", "Unrecognized Uniform type set.", Vantor::Backlog::LogLevel::ERR);
                    break;
                }}

            // TODO: Move this away, this was just to test

            // Directional light — soft and subtle
            command.Material->GetShader()->setUniformVec3("VdirLight.direction", { 0.0, -1.0, 0.0 }); // Above and behind
            command.Material->GetShader()->setUniformVec3("VdirLight.ambient",  { 0.01f, 0.01f, 0.01f });
            command.Material->GetShader()->setUniformVec3("VdirLight.diffuse",  { 0.08f, 0.08f, 0.08f });
            command.Material->GetShader()->setUniformVec3("VdirLight.specular", { 0.05f, 0.05f, 0.05f });

            // Spotlight — focused and realistic
            command.Material->GetShader()->setUniformVec3("VspotLight.position",  m_RenderPath->GetCamera()->Position); // Below and behind
            command.Material->GetShader()->setUniformVec3("VspotLight.direction", m_RenderPath->GetCamera()->Forward); // Facing toward object
            command.Material->GetShader()->setUniformVec3("VspotLight.ambient",   { 0.00f, 0.00f, 0.00f }); // No ambient (only direct light)
            command.Material->GetShader()->setUniformVec3("VspotLight.diffuse",   { 2.0f, 2.0f, 2.0f });    // Still bright, but not overkill
            command.Material->GetShader()->setUniformVec3("VspotLight.specular",  { 1.0f, 1.0f, 1.0f });    // Balanced specular

            // Attenuation — more realistic falloff
            command.Material->GetShader()->setUniformFloat("VspotLight.constant",  1.0f);
            command.Material->GetShader()->setUniformFloat("VspotLight.linear",    0.14f);
            command.Material->GetShader()->setUniformFloat("VspotLight.quadratic", 0.07f);

            // Cone cutoff
            command.Material->GetShader()->setUniformFloat("VspotLight.cutOff", 
                std::cos(Vantor::Math::DegToRad(10.0f))); // Tighter beam
            command.Material->GetShader()->setUniformFloat("VspotLight.outerCutOff", 
                std::cos(Vantor::Math::DegToRad(15.0f))); // Smooth falloff

            // Texture usage
            command.Material->GetShader()->setUniformBool("useDiffuseTexture", MaterialUseTexture);
            command.Material->GetShader()->setUniformBool("useSpecularTexture", MaterialUseTexture);
            
            // Only set this when false, to avoid pushes to GPU
            if (!MaterialUseTexture) {
                command.Material->GetShader()->setUniformVec3("diffuseColor", command.Material->color.toFloat3());
                command.Material->GetShader()->setUniformVec3("specularColor", command.Material->color.toFloat3());
            }

            // Material shininess
            command.Material->GetShader()->setUniformFloat("shininess", command.Material->shininess);

            // Draw the Raw Mesh
            command.Mesh->RenderRaw();

            // Set all Material Variables back to normal
            MaterialUseTexture = false;
        }
    }

    void VForwardRenderPassGL::Cleanup() {
        // TODO
    }

    // =============================================================================
    // VGeometryRenderPassGL Implementation
    // =============================================================================

    VGeometryRenderPassGL::VGeometryRenderPassGL() : VRenderPassGL(VERenderPassType::Geometry, "GeometryPass") {}

    void VGeometryRenderPassGL::Initialize(VRenderPath3D* renderPath)
    {
       m_RenderPath = renderPath;
    }

    void VGeometryRenderPassGL::Execute()
    {
        // Enable States
        VOpenGLStateChache::Instance().SetDepthTest(true);
        VOpenGLStateChache::Instance().SetDepthFunc(GL_LESS);

        VOpenGLStateChache::Instance().SetCull(true);
        VOpenGLStateChache::Instance().SetCullFace(GL_BACK);

        m_RenderPath->GetGBuffer()->Bind();
        
        // Clear the G-Buffer
        VClearValues clearValues;
        clearValues.ColorValue = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues.DepthValue = 1.0f;
        m_RenderPath->GetGBuffer()->Clear(clearValues);

        // Activate Common UBO
        m_RenderPath->ActivateStorage(VEStorageType::CommonUBO);

        // Material Settings
        bool MaterialUseTexture = false;

        // Go through all Meshes
        for (auto command : m_RenderPath->GetCommandBuffer()->GetDefferedRenderCommands()) {
            command.Material->GetShader()->Use();

            // Should be the same in every Shader
            command.Material->GetShader()->setUniformMat4("VModel", command.Transform);

            // Handle Textures of Material
            auto* samplers = command.Material->GetSamplerUniforms();
            for (auto it = samplers->begin(); it != samplers->end(); ++it)
            {
                if (it->second.Type == Vantor::Renderer::UniformTypeSAMPLERCUBE) {
                    it->second.TextureCube->Bind(it->second.Unit);
                    MaterialUseTexture = true;
                }
                else {
                    it->second.Texture->Bind(it->second.Unit);
                    MaterialUseTexture = true;
                }
            }

            // Set all Uniforms of Material to its Shader
            auto* uniforms = command.Material->GetUniforms();
            for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
            {

                switch (it->second.Type)
                {
                case Vantor::Renderer::UniformTypeBOOL:
                    command.Material->GetShader()->setUniformBool(it->first, it->second.Bool);
                    break;
                case Vantor::Renderer::UniformTypeINT:
                    command.Material->GetShader()->setUniformInt(it->first, it->second.Int);
                    break;
                case Vantor::Renderer::UniformTypeFLOAT:
                    command.Material->GetShader()->setUniformFloat(it->first, it->second.Float);
                    break;
                case Vantor::Renderer::UniformTypeVEC2:
                    command.Material->GetShader()->setUniformVec2(it->first, it->second.Vec2);
                    break;
                case Vantor::Renderer::UniformTypeVEC3:
                    command.Material->GetShader()->setUniformVec3(it->first, it->second.Vec3);
                    break;
                case Vantor::Renderer::UniformTypeVEC4:
                    command.Material->GetShader()->setUniformVec4(it->first, it->second.Vec4);
                    break;
                case Vantor::Renderer::UniformTypeMAT2:
                    command.Material->GetShader()->setUniformMat2(it->first, it->second.Mat2);
                    break;
                case Vantor::Renderer::UniformTypeMAT3:
                    command.Material->GetShader()->setUniformMat3(it->first, it->second.Mat3);
                    break;
                case Vantor::Renderer::UniformTypeMAT4:
                    command.Material->GetShader()->setUniformMat4(it->first, it->second.Mat4);
                    break;
                default:
                    Vantor::Backlog::Log("VRenderPath3DGL::GeometryPass", "Unrecognized Uniform type set.", Vantor::Backlog::LogLevel::ERR);
                    break;
                }
            }
            
            // Only set this when false, to avoid pushes to GPU
            // TODO: Write fallback Material
            // if (!MaterialUseTexture) {
            //     command.Material->GetShader()->setUniformVec3("diffuseColor", command.Material->color.toFloat3());
            //     command.Material->GetShader()->setUniformVec3("specularColor", command.Material->color.toFloat3());
            // }

            // Material shininess : TODO : Remove when doing PBR
            // command.Material->GetShader()->setUniformFloat("shininess", command.Material->shininess);

            // Draw the Raw Mesh
            command.Mesh->RenderRaw();

            // Set all Material Settings back to normal
            MaterialUseTexture = false;
        }

        m_RenderPath->GetGBuffer()->Unbind();
    }

    void VGeometryRenderPassGL::Cleanup() {
        // TODO
    }

    // =============================================================================
    // VLightingRenderPassGL Implementation
    // =============================================================================

    VLightingRenderPassGL::VLightingRenderPassGL() : VRenderPassGL(VERenderPassType::Lighting, "LightingPass") {}

    void VLightingRenderPassGL::Initialize(VRenderPath3D* renderPath)
    {
        m_RenderPath = renderPath;

        // Load Shaders : TODO: work with Material Library
        auto ResShaderLighting = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("VIShaderLighting", "Shaders/Private/Deffered/VLighting.vglsl", "Shaders/Private/Deffered/VLighting.fglsl");

        m_ShaderLighting = ResShaderLighting->GetShader();

        // m_OutBuffer = VRenderTargetFactory::CreateColorTarget(
        //     1920, 1080,                    // Dimensions
        //     VTextureFormat::RGBA8,         // Color format
        //     "OutBuffer"              // Debug name
        // );

        // Setting Texture Units
        m_ShaderLighting->Use();

        m_ShaderLighting->setUniformInt("gAlbedoSpec", 0);  // Albedo is at index 0
        m_ShaderLighting->setUniformInt("gNormal", 1);      // Normal is at index 1  
        m_ShaderLighting->setUniformInt("gPosition", 2);    // Position at texture unit 2 for now
    }

    void VLightingRenderPassGL::Execute()
    {
        VOpenGLStateChache::Instance().SetDepthTest(false);
        // m_OutBuffer->Bind();

        // VClearValues clearValues;
        // clearValues.ColorValue = {0.0f, 0.0f, 0.0f, 1.0f};
        // clearValues.DepthValue = 1.0f;

        // m_OutBuffer->Clear(clearValues);

        m_ShaderLighting->Use();

        // TODO: Implement a StateCache for each Storage, to minimize GPU calls
        // UBOs
        m_RenderPath->ActivateStorage(VEStorageType::CommonUBO);
        m_RenderPath->ActivateStorage(VEStorageType::LightDataUBO);
        // Light Data Shader Storage Buffers
        m_RenderPath->ActivateStorage(VEStorageType::PointLightSSBO);

        // Bind G-Buffer textures to correct texture units
        auto albedoTexture =   m_RenderPath->GetGBuffer()->GetColorTexture(0);  // Albedo is at index 0
        auto normalTexture =   m_RenderPath->GetGBuffer()->GetColorTexture(1);  // Normal is at index 1
        auto materialTexture = m_RenderPath->GetGBuffer()->GetColorTexture(2); //  TODO
        auto positionTexture = m_RenderPath->GetGBuffer()->GetColorTexture(3);  // Position is at index 3


        albedoTexture->Bind(0);
        normalTexture->Bind(1);
        positionTexture->Bind(2);

        m_ScreenQuad.DrawRaw();

        // m_OutBuffer->Unbind(); TODO

        // auto outTexture = m_OutBuffer->GetColorTexture(0);

        // outTexture->Bind(1);

        // m_ScreenQuad.Draw();
    }

    void VLightingRenderPassGL::Cleanup() {
        // TODO
    }

    // =============================================================================
    // VRenderPath3DGL Implementation
    // =============================================================================

    VRenderPath3DGL::VRenderPath3DGL() {}

    void VRenderPath3DGL::Initialize(VRDevice *device)
    {
        m_CommandBuffer = std::make_shared<VOpenGLCommandBuffer>();

        m_Device = device;
        SetupDefaultRenderPasses();
       
        // Initialize the GBuffer
        VRenderTargetDescriptor gBufferDesc = VRenderTargetUtils::CreateMRTTarget(
            m_ViewportWidth, m_ViewportHeight,//  TODO: work with resizing
            {
                VTextureFormat::RGBA8,   // Albedo
                VTextureFormat::RGBA8,   // Normal (world space)
                VTextureFormat::RGBA8,   // Material properties (metallic, roughness, AO)
                VTextureFormat::RGBA16F  // World position
            },
            VTextureFormat::Depth24,     // Depth buffer
            "GBuffer"
        );

        m_GBuffer = std::make_shared<VOpenGLRenderTarget>();
        if (m_GBuffer->Create(gBufferDesc)) {
            Vantor::Backlog::Log("VRenderPath3DGL", "Initialized GBuffer successfully.");
        }

        // Initialize all render passes
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Initialize(this);
        }

        // // Debbuging:
        // GLint maxUBOs = 0;
        // glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUBOs);

        // GLint maxSSBOs = 0;
        // glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxSSBOs);

        // std::cout << "Max UBOs : " << maxUBOs  << std::endl;
        // std::cout << "Max SSBOs: " << maxSSBOs << std::endl;

        ResetRenderStats();
    }

    void VRenderPath3DGL::PushRender(VMesh *mesh, Vantor::Renderer::VMaterial *material, Vantor::Math::VMat4 transform) 
    {
        // Push Render to Command Buffer (RenderPath acts as Interface for developer)
        m_CommandBuffer->Push(mesh, material, transform);
    }

    void VRenderPath3DGL::PushRender(Vantor::Object::VObject* object) 
    {   
        // The RenderPath will only push the VObject to the CommandBuffer
        // if it has these 3 Components avalible, else it will not be rendered
        
        // TODO: Implement Fallbacks for Material and Transform
        Vantor::Math::VMat4 transform;
        Vantor::Renderer::VMaterial* material;
        VMesh* mesh;

        // Push Render to Command Buffer

        // TransformComponent
        if (object->HasComponent<Vantor::Object::VTransformComponent>()) {
            transform = object->GetComponent<Vantor::Object::VTransformComponent>()->GetTransform();
        } else { return; }

        // Material Component
        if (object->HasComponent<Vantor::Object::VMaterialComponent>()) {
            material = object->GetComponent<Vantor::Object::VMaterialComponent>()->GetMaterial();
        } else { return; }

        // Mesh Component
        if (object->HasComponent<Vantor::Object::VMeshComponent>()) {
            mesh = object->GetComponent<Vantor::Object::VMeshComponent>()->GetMesh().get();
        }

        m_CommandBuffer->Push(mesh, material, transform);
    }

    void VRenderPath3DGL::PushPointLight(const Vantor::Renderer::VPointLightData& pointLightData)
    {
        m_PointLights.push_back(pointLightData);
    }

    void VRenderPath3DGL::Render()
    {
        if (!m_CommandBuffer) return;

        ResetRenderStats();
        // Sorting Elements in CommandBuffer
        m_CommandBuffer->Sort();
        // UBO
        m_CommonUBO.Update(m_Camera);        
        // Update the Lightdata Uniform Buffer
        m_LightDataUBO.UploadLightData(m_PointLights.size());
        m_LightDataUBO.Update();
        
        // Update PointLightData
        m_PointLightSSBO.Update(m_PointLights);

        // Set viewport
        glViewport(m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Set global OpenGL state
        if (m_WireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (m_CullingEnabled)
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        // Execute render passes in order
        // TODO:
        // if (m_ShadowsEnabled)
        // {
        //     auto* shadowPass = GetRenderPass(VRenderPassType::ShadowMap);
        //     if (shadowPass && shadowPass->IsEnabled())
        //     {
        //         shadowPass->Execute(m_CommandBuffer.get());
        //     }
        // }

        // auto *forwardPass = GetRenderPass(VERenderPassType::Forward);
        // if (forwardPass && forwardPass->IsEnabled())
        // {
        //     forwardPass->Execute();
        // }

        // The Geometry Pass
        auto *geometryPass = GetRenderPass(VERenderPassType::Geometry);
        if (geometryPass && geometryPass->IsEnabled())
        {
            geometryPass->Execute();
        }

        // The Lighting Pass
        auto *lightingPass = GetRenderPass(VERenderPassType::Lighting);
        if (lightingPass && lightingPass->IsEnabled())
        {
            lightingPass->Execute();
        }

        // Clear the Command Buffer at the end
        m_CommandBuffer->Clear();

        // Clear all lights: TODO: Cache them to stay
        m_PointLights.clear();
    }

    void VRenderPath3DGL::Shutdown()
    {
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Cleanup();
        }
        m_RenderPasses.clear();
    }

    void VRenderPath3DGL::SetRenderTarget(VRenderTarget *target)
    {
        m_RenderTarget = target;
        // TODO: Bind render target framebuffer
    }

    VRenderTarget *VRenderPath3DGL::GetRenderTarget() const { return m_RenderTarget; }

    void VRenderPath3DGL::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        m_ViewportX      = x;
        m_ViewportY      = y;
        m_ViewportWidth  = width;
        m_ViewportHeight = height;

        m_GBuffer->Resize(width, height); // Resize the GBuffer
    }

    void VRenderPath3DGL::GetViewport(uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height) const
    {
        x      = m_ViewportX;
        y      = m_ViewportY;
        width  = m_ViewportWidth;
        height = m_ViewportHeight;
    }

    void VRenderPath3DGL::AddRenderPass(std::unique_ptr<VRenderPass> pass)
    {
        if (pass)
        {
            VERenderPassType type = pass->GetType();
            m_RenderPasses[type]  = std::move(pass);
        }
    }

    // void VRenderPath3DGL::RemoveRenderPass(VERenderPassType type)
    // {
    //     // TODO
    //     // auto it = m_RenderPasses.find(type);
    //     // if (it != m_RenderPasses.end())
    //     // {
    //     //     it->second->Cleanup();
    //     //     m_RenderPasses.erase(it);
    //     // }
    // }

    VRenderPass *VRenderPath3DGL::GetRenderPass(VERenderPassType type) const
    {
        auto it = m_RenderPasses.find(type);
        return (it != m_RenderPasses.end()) ? it->second.get() : nullptr;
    }

    const VERenderStats &VRenderPath3DGL::GetRenderStats() const { return m_Stats; }

    void VRenderPath3DGL::ResetRenderStats() { m_Stats = VERenderStats(); }

    void VRenderPath3DGL::SetCamera(Vantor::Renderer::Camera *camera)
    {
        m_Camera = camera;
    }

    Vantor::Renderer::Camera *VRenderPath3DGL::GetCamera() const { return m_Camera; }

    void VRenderPath3DGL::ActivateStorage(VEStorageType storage) const {
        switch (storage) {
            // UBOs
            case VEStorageType::CommonUBO: m_CommonUBO.Bind(static_cast<GLuint>(VEStorageType::CommonUBO));
            case VEStorageType::LightDataUBO: m_LightDataUBO.Bind(static_cast<GLuint>(VEStorageType::LightDataUBO));
            // SSBOs
            case VEStorageType::PointLightSSBO: m_PointLightSSBO.Bind(static_cast<GLuint>(VEStorageType::PointLightSSBO));
            default: return;
        }
    }

    VCommandBuffer* VRenderPath3DGL::GetCommandBuffer() { return m_CommandBuffer.get(); }

    // void VRenderPath3DGL::EnableShadows(bool enable)
    // {
    //     m_ShadowsEnabled = enable;
    // }

    // bool VRenderPath3DGL::AreShadowsEnabled() const
    // {
    //     return m_ShadowsEnabled;
    // }

    // void VRenderPath3DGL::SetShadowMapSize(uint32_t size)
    // {
    //     m_ShadowMapSize = size;

    //     // Recreate shadow map with new size
    //     auto* shadowPass = GetRenderPass(VRenderPassType::ShadowMap);
    //     if (shadowPass)
    //     {
    //         shadowPass->Cleanup();
    //         shadowPass->Initialize();
    //     }
    // }

    // void VRenderPath3DGL::SetExposure(float exposure)
    // {
    //     m_Exposure = exposure;
    // }

    // void VRenderPath3DGL::SetGamma(float gamma)
    // {
    //     m_Gamma = gamma;
    // }

    // void VRenderPath3DGL::EnableHDR(bool enable)
    // {
    //     m_HDREnabled = enable;
    // }

    // void VRenderPath3DGL::EnableFXAA(bool enable)
    // {
    //     m_FXAAEnabled = enable;
    // }

    void VRenderPath3DGL::SetWireframeMode(bool enable)
    {
        // m_WireframeMode = enable;
    }

    void VRenderPath3DGL::SetCullingMode(bool enable) { m_CullingEnabled = enable; }

    void VRenderPath3DGL::SetupDefaultRenderPasses()
    {
        // Add shadow map pass : TODO
        // auto shadowPass = std::make_unique<VShadowMapPassGL>(m_ShadowMapSize);
        // AddRenderPass(std::move(shadowPass));

        // Add forward rendering pass
        auto forwardPass = std::make_unique<VForwardRenderPassGL>();
        AddRenderPass(std::move(forwardPass));

        // Add Geometry Pass
        auto geometryPass = std::make_unique<VGeometryRenderPassGL>();
        AddRenderPass(std::move(geometryPass));

        // Add Lighting Pass
        auto lightingPass = std::make_unique<VLightingRenderPassGL>();
        AddRenderPass(std::move(lightingPass));
    }

    // =============================================================================
    // VSpriteBatch Implementation : TODO
    // =============================================================================
} // namespace Vantor::RenderDevice