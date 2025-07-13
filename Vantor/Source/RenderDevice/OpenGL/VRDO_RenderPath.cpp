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
            command.Material->GetShader()->use();

            // Should be the same in every Shader
            command.Material->GetShader()->setMat4("model", command.Transform);

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
                    command.Material->GetShader()->setVec2(it->first, it->second.Vec2);
                    break;
                case Vantor::Renderer::UniformTypeVEC3:
                    command.Material->GetShader()->setVec3(it->first, it->second.Vec3);
                    break;
                case Vantor::Renderer::UniformTypeVEC4:
                    command.Material->GetShader()->setVec4(it->first, it->second.Vec4);
                    break;
                case Vantor::Renderer::UniformTypeMAT2:
                    command.Material->GetShader()->setMat2(it->first, it->second.Mat2);
                    break;
                case Vantor::Renderer::UniformTypeMAT3:
                    command.Material->GetShader()->setMat3(it->first, it->second.Mat3);
                    break;
                case Vantor::Renderer::UniformTypeMAT4:
                    command.Material->GetShader()->setMat4(it->first, it->second.Mat4);
                    break;
                default:
                    Vantor::Backlog::Log("VRenderPath3DGL::ForwardPass", "Unrecognized Uniform type set.", Vantor::Backlog::LogLevel::ERR);
                    break;
                }}

            // TODO: Move this away, this was just to test

            // Directional light — soft and subtle
            command.Material->GetShader()->setVec3("VdirLight.direction", { 0.0, -1.0, 0.0 }); // Above and behind
            command.Material->GetShader()->setVec3("VdirLight.ambient",  { 0.01f, 0.01f, 0.01f });
            command.Material->GetShader()->setVec3("VdirLight.diffuse",  { 0.08f, 0.08f, 0.08f });
            command.Material->GetShader()->setVec3("VdirLight.specular", { 0.05f, 0.05f, 0.05f });

            // Spotlight — focused and realistic
            command.Material->GetShader()->setVec3("VspotLight.position",  m_RenderPath->GetCamera()->Position); // Below and behind
            command.Material->GetShader()->setVec3("VspotLight.direction", m_RenderPath->GetCamera()->Forward); // Facing toward object
            command.Material->GetShader()->setVec3("VspotLight.ambient",   { 0.00f, 0.00f, 0.00f }); // No ambient (only direct light)
            command.Material->GetShader()->setVec3("VspotLight.diffuse",   { 2.0f, 2.0f, 2.0f });    // Still bright, but not overkill
            command.Material->GetShader()->setVec3("VspotLight.specular",  { 1.0f, 1.0f, 1.0f });    // Balanced specular

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
                command.Material->GetShader()->setVec3("diffuseColor", command.Material->color.toFloat3());
                command.Material->GetShader()->setVec3("specularColor", command.Material->color.toFloat3());
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
    // VDefferedRenderPassGL Implementation
    // =============================================================================

    VDefferedRenderPassGL::VDefferedRenderPassGL() : VRenderPassGL(VERenderPassType::Deffered, "DefferedPass") {}

    void VDefferedRenderPassGL::Initialize(VRenderPath3D* renderPath)
    {
       m_RenderPath = renderPath;

       // Load Shaders
       auto ResShaderGBuffer = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("VIShaderGBuffer", "Shaders/Private/Deffered/VGBuffer.vglsl", "Shaders/Private/Deffered/VGBuffer.fglsl");

       m_ShaderGBuffer = ResShaderGBuffer->GetShader();

       // Initialize the GBuffer
       VRenderTargetDescriptor gBufferDesc = VRenderTargetUtils::CreateMRTTarget(
            1920, 1080,//  TODO: work with resizing
            {
                VTextureFormat::RGBA8,   // Albedo
                VTextureFormat::RGBA8,   // Normal (world space)
                VTextureFormat::RGBA8,   // Material properties (metallic, roughness, AO)
                VTextureFormat::RGBA16F  // World position
            },
            VTextureFormat::Depth24,     // Depth buffer
            "GBuffer"
        );

        m_GBuffer = std::make_unique<VOpenGLRenderTarget>();
        if (m_GBuffer->Create(gBufferDesc)) {
            Vantor::Backlog::Log("VRenderPath3DGL::DefferedPass", "Initialized GBuffer successfully.");
        }
    }

    void VDefferedRenderPassGL::Execute()
    {
        // Enable States
        VOpenGLStateChache::Instance().SetDepthTest(true);
        VOpenGLStateChache::Instance().SetDepthFunc(GL_LESS);

        VOpenGLStateChache::Instance().SetCull(true);
        VOpenGLStateChache::Instance().SetCullFace(GL_BACK);

        m_GBuffer->Bind();
        
        // Clear the G-Buffer
        VClearValues clearValues;
        clearValues.ColorValue = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues.DepthValue = 1.0f;
        m_GBuffer->Clear(clearValues);

        m_ShaderGBuffer->use();
        
        // We dont use materials yet, to test its overall ability

        // // Variables for Material
        // bool MaterialUseTexture = false;

        // Go through all Meshes
        for (auto command : m_RenderPath->GetCommandBuffer()->GetForwardRenderCommands()) {
            // command.Material->GetShader()->use();

            // Should be the same in every Shader
            m_ShaderGBuffer->setMat4("model", command.Transform);

            // Handle Textures of Material
            auto* samplers = command.Material->GetSamplerUniforms();
            for (auto it = samplers->begin(); it != samplers->end(); ++it)
            {
                if (it->second.Type == Vantor::Renderer::UniformTypeSAMPLERCUBE) {
                    it->second.TextureCube->Bind(it->second.Unit);
                    // MaterialUseTexture = true;
                }
                else {
                    it->second.Texture->Bind(it->second.Unit);
                    // MaterialUseTexture = true;
                }
            }

            m_ShaderGBuffer->setUniformInt("texture_diffuse1", 1);
            m_ShaderGBuffer->setUniformInt("texture_specular1", 2);

            // Set all Uniforms of Material to its Shader : The error is here
            // auto* uniforms = command.Material->GetUniforms();
            // for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
            // {
            //     std::cout << it->first << std::endl;

            //     switch (it->second.Type)
            //     {
            //     case Vantor::Renderer::UniformTypeBOOL:
            //         command.Material->GetShader()->setUniformBool(it->first, it->second.Bool);
            //         break;
            //     case Vantor::Renderer::UniformTypeINT:
            //         command.Material->GetShader()->setUniformInt(it->first, it->second.Int);
            //         break;
            //     case Vantor::Renderer::UniformTypeFLOAT:
            //         command.Material->GetShader()->setUniformFloat(it->first, it->second.Float);
            //         break;
            //     case Vantor::Renderer::UniformTypeVEC2:
            //         command.Material->GetShader()->setVec2(it->first, it->second.Vec2);
            //         break;
            //     case Vantor::Renderer::UniformTypeVEC3:
            //         command.Material->GetShader()->setVec3(it->first, it->second.Vec3);
            //         break;
            //     case Vantor::Renderer::UniformTypeVEC4:
            //         command.Material->GetShader()->setVec4(it->first, it->second.Vec4);
            //         break;
            //     case Vantor::Renderer::UniformTypeMAT2:
            //         command.Material->GetShader()->setMat2(it->first, it->second.Mat2);
            //         break;
            //     case Vantor::Renderer::UniformTypeMAT3:
            //         command.Material->GetShader()->setMat3(it->first, it->second.Mat3);
            //         break;
            //     case Vantor::Renderer::UniformTypeMAT4:
            //         command.Material->GetShader()->setMat4(it->first, it->second.Mat4);
            //         break;
            //     default:
            //         Vantor::Backlog::Log("VRenderPath3DGL::ForwardPass", "Unrecognized Uniform type set.", Vantor::Backlog::LogLevel::ERR);
            //         break;
            //     }}
            
            // // Only set this when false, to avoid pushes to GPU
            // if (!MaterialUseTexture) {
            //     command.Material->GetShader()->setVec3("diffuseColor", command.Material->color.toFloat3());
            //     command.Material->GetShader()->setVec3("specularColor", command.Material->color.toFloat3());
            // }

            // // Material shininess
            // command.Material->GetShader()->setUniformFloat("shininess", command.Material->shininess);

            // Draw the Raw Mesh
            command.Mesh->RenderRaw();

            // Set all Material Variables back to normal
            // MaterialUseTexture = false;
        }

        m_GBuffer->Unbind();
        
        auto albedoTexture = m_GBuffer->GetColorTexture(0);
        auto normalTexture = m_GBuffer->GetColorTexture(1);
        auto materialTexture = m_GBuffer->GetColorTexture(2);
        auto positionTexture = m_GBuffer->GetColorTexture(3);

        VOpenGLStateChache::Instance().SetDepthTest(false);

        normalTexture->Bind(1);

        m_ScreenQuad.Draw();
        // glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void VDefferedRenderPassGL::Cleanup() {
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

        // Initialize all render passes
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Initialize(this);
        }

        ResetRenderStats();
    }

    void VRenderPath3DGL::PushRender(VMesh *mesh, Vantor::Renderer::VMaterial *material, Vantor::Math::VMat4 transform) 
    {
        // Push Render to Command Buffer (RenderPath acts as Interface for developer)
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

        // Update Uniform Buffer
        m_UBO.Update(m_Camera, m_PointLights);

        // Bind the UBO at beginning
        m_UBO.Bind();

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

        auto *defferedPass = GetRenderPass(VERenderPassType::Deffered);
        if (defferedPass && defferedPass->IsEnabled())
        {
            defferedPass->Execute();
        }

        // Clear the Command Buffer at the end
        m_CommandBuffer->Clear();

        // Clear all lights
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

        // Update camera in forward pass
        auto *forwardPass = dynamic_cast<VForwardRenderPassGL *>(GetRenderPass(VERenderPassType::Forward));
        if (forwardPass)
        {
            forwardPass->SetCamera(camera);
        }
    }

    Vantor::Renderer::Camera *VRenderPath3DGL::GetCamera() const { return m_Camera; }

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
        forwardPass->SetCamera(m_Camera);
        AddRenderPass(std::move(forwardPass));

        auto defferedPass = std::make_unique<VDefferedRenderPassGL>();
        defferedPass->SetCamera(m_Camera);
        AddRenderPass(std::move(defferedPass));
    }

    // =============================================================================
    // VSpriteBatch Implementation : TODO
    // =============================================================================
} // namespace Vantor::RenderDevice