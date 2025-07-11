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

namespace Vantor::RenderDevice
{
    // =============================================================================
    // VRenderPassGL Implementation
    // =============================================================================

    VRenderPassGL::VRenderPassGL(VERenderPassType type, const std::string &name) : m_Type(type), m_Name(name) {}

    // =============================================================================
    // VForwardRenderPassGL Implementation
    // =============================================================================

    VForwardRenderPassGL::VForwardRenderPassGL() : VRenderPassGL(VERenderPassType::Forward, "ForwardPass") {}

    void VForwardRenderPassGL::Initialize()
    {
        float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                            -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

        auto m_ResDefShader
            = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("m_DefaultShader", "Shaders/Private/first.vs", "Shaders/Private/first.fs");

        m_DefaultShader = m_ResDefShader->GetShader();

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        VTextureSampler sampler;
        sampler.minFilter       = VTextureFilter::Linear;
        sampler.magFilter       = VTextureFilter::Linear;
        sampler.wrapS           = VTextureWrap::Repeat;
        sampler.wrapT           = VTextureWrap::Repeat;
        sampler.generateMipmaps = false;

        auto m_ResDeffered = Vantor::Resource::VResourceManager::Instance().LoadTexture2D("m_Deffered", "Resources/textures/container2.png", sampler, false);
        auto m_ResSpecular
            = Vantor::Resource::VResourceManager::Instance().LoadTexture2D("m_Specular", "Resources/textures/container2_specular.png", sampler, false);

        m_Deffered = VOpenGLTexture2D::CreateFromFile("Resources/textures/container2.png", sampler, false);
        m_Specular = VOpenGLTexture2D::CreateFromFile("Resources/textures/container2_specular.png", sampler, false);

        if (!m_Deffered)
        {
            std::cout << "m_Deffered is nullptr" << std::endl;
        }

        if (!m_Specular)
        {
            std::cout << "m_Specular is nullptr" << std::endl;
        }

        // REMOVED: These Create() calls might be overwriting the loaded texture data
        // m_Deffered->Create(m_Deffered->GetSpec());
        // m_Specular->Create(m_Specular->GetSpec());

        m_DefaultShader->use();

        m_DefaultShader->setUniformInt("texture1", 1);
        m_DefaultShader->setUniformInt("texture2", 2);
    }

    void VForwardRenderPassGL::Execute(VCommandBuffer *commandBuffer)
    {
        // Set OpenGL state for forward rendering : TODO : Work with StateChache of RenderPath
        // Enable these for proper 3D rendering
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);

        m_Deffered->Bind(1);
        m_Specular->Bind(2);

        m_DefaultShader->use();

        m_DefaultShader->setMat4("projection", m_Camera->Projection);
        m_DefaultShader->setMat4("view", m_Camera->View);

        glBindVertexArray(cubeVAO);

        Vantor::Math::VMat4 model = Vantor::Math::VMat4::Identity();                           // Setup the Identity Matrix
        model                     = model.Translate(Vantor::Math::VVector3(1.0f, 1.0f, 1.0f)); // Translate model

        m_DefaultShader->setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Basic implementation - would need proper command buffer iteration
        // commandBuffer->ExecuteAll();
    }

    void VForwardRenderPassGL::Cleanup()
    {
        // Just to test
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &VBO);

        m_DefaultShader.reset();
    }

    // =============================================================================
    // VRenderPath3DGL Implementation
    // =============================================================================

    VRenderPath3DGL::VRenderPath3DGL() {}

    void VRenderPath3DGL::Initialize(VRDevice *device)
    {
        // TODO: Make Command Buffer m_CommandBuffer =
        m_Device = device;
        SetupDefaultRenderPasses();

        // Initialize all render passes
        for (auto &[type, pass] : m_RenderPasses)
        {
            pass->Initialize();
        }

        ResetRenderStats();
    }

    void VRenderPath3DGL::Render()
    {
        // if (!m_CommandBuffer) return;

        ResetRenderStats();

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

        auto *forwardPass = GetRenderPass(VERenderPassType::Forward);
        if (forwardPass && forwardPass->IsEnabled())
        {
            forwardPass->Execute(m_CommandBuffer.get());
        }

        // UpdateRenderStats(m_CommandBuffer.get());
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

    void VRenderPath3DGL::SetAmbientLight(const Vantor::Math::VVector3 &color)
    {
        m_AmbientLight = color;

        // Update ambient light in forward pass
        auto *forwardPass = dynamic_cast<VForwardRenderPassGL *>(GetRenderPass(VERenderPassType::Forward));
        if (forwardPass)
        {
            forwardPass->SetAmbientLight(color);
        }
    }

    const Vantor::Math::VVector3 &VRenderPath3DGL::GetAmbientLight() const { return m_AmbientLight; }

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
    }

    // =============================================================================
    // VSpriteBatch Implementation : TODO
    // =============================================================================
} // namespace Vantor::RenderDevice