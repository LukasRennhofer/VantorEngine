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
 *  File: VRD_RenderPath.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../../Math/Linear/VMA_Vector.hpp"
#include "../../Renderer/Camera/VRE_Camera.hpp"
#include "VRD_CommandBuffer.hpp"
#include "../../Renderer/Camera/VRE_Camera.hpp"

// Math
#include "../../Math/Linear/VMA_Vector.hpp"

namespace Vantor::RenderDevice
{
    // Forward declerations
    class VRDevice;
    class VRenderTarget;

    // RenderPass Types (In order)
    enum class VERenderPassType
    {
        Forward
    };

    // TODO: Profile RenderPasses
    // Render statistics for profiling (later on)
    struct VERenderStats
    {
            uint32_t DrawCalls      = 0;
            uint32_t Triangles      = 0;
            uint32_t Vertices       = 0;
            float    RenderTime     = 0.0f;
            uint32_t TextureBinds   = 0;
            uint32_t ShaderSwitches = 0;
    };

    // State Cache for chaching past render states
    class VStateCache
    {
        public:
            ~VStateCache() = default;

            // Wireframe state
            void SetWireframe(bool enabled) { m_isWireframeOn = enabled; }
            bool IsWireframeEnabled() const { return m_isWireframeOn; }

            // Culling state
            void SetCulling(bool enabled) { m_isCullingOn = enabled; }
            bool IsCullingEnabled() const { return m_isCullingOn; }

        private:
            bool m_isWireframeOn = false;
            bool m_isCullingOn   = false;
    };

    // Base render pass interface
    class VRenderPass
    {
        public:
            virtual ~VRenderPass() = default;

            virtual void Initialize()                           = 0;
            virtual void Execute(VCommandBuffer *commandBuffer) = 0;
            virtual void Cleanup()                              = 0;

            virtual VERenderPassType   GetType() const = 0;
            virtual const std::string &GetName() const = 0;

            // Enable/disable the pass
            virtual void SetEnabled(bool enabled) { m_Enabled = enabled; }
            virtual bool IsEnabled() const { return m_Enabled; }

        protected:
            bool m_Enabled = true;
    };

    // Base render path interface
    class VRenderPath
    {
        public:
            virtual ~VRenderPath() = default;

            // Core rendering methods
            virtual void Initialize(VRDevice *device) = 0;
            virtual void Render()                     = 0;
            virtual void Shutdown()                   = 0;

            // Render target management
            virtual void           SetRenderTarget(VRenderTarget *target) = 0;
            virtual VRenderTarget *GetRenderTarget() const                = 0;

            // Viewport configuration
            virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)           = 0;
            virtual void GetViewport(uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height) const = 0;

            // Render pass management
            virtual void         AddRenderPass(std::unique_ptr<VRenderPass> pass) = 0;
            virtual VRenderPass *GetRenderPass(VERenderPassType type) const       = 0;

            // Statistics and profiling : TODO
            virtual const VERenderStats &GetRenderStats() const = 0;
            virtual void                 ResetRenderStats()     = 0;

            bool IsActive() { return isActive; }
            void SetActive(bool active) { isActive = active; }

        protected:
            bool isActive = true;

            // State Cache for saving API states
            VStateCache m_StateCache;

            VRDevice      *m_Device       = nullptr;
            VRenderTarget *m_RenderTarget = nullptr;
            VERenderStats  m_Stats; // TODO

            uint32_t m_ViewportX      = 0;
            uint32_t m_ViewportY      = 0;
            uint32_t m_ViewportWidth  = 1280;
            uint32_t m_ViewportHeight = 720;

            std::shared_ptr<VCommandBuffer> m_CommandBuffer;
    };

    // 3D rendering path interface
    class VRenderPath3D : public VRenderPath
    {
        public:
            virtual ~VRenderPath3D() = default;

            // Camera management
            virtual void                      SetCamera(Vantor::Renderer::Camera *camera) = 0;
            virtual Vantor::Renderer::Camera *GetCamera() const                           = 0;

            // Lighting configuration
            virtual void                          SetAmbientLight(const Vantor::Math::VVector3 &color) = 0;
            virtual const Vantor::Math::VVector3 &GetAmbientLight() const                              = 0;

            // Shadow mapping : TODO
            // virtual void EnableShadows(bool enable) = 0;
            // virtual bool AreShadowsEnabled() const = 0;
            // virtual void SetShadowMapSize(uint32_t size) = 0;

            // // Post-processing effects : TODO
            // virtual void SetExposure(float exposure) = 0;
            // virtual void SetGamma(float gamma) = 0;
            // virtual void EnableHDR(bool enable) = 0;
            // virtual void EnableFXAA(bool enable) = 0;

            // Render modes : TODO
            virtual void SetWireframeMode(bool enable) = 0;
            virtual void SetCullingMode(bool enable)   = 0;

        protected:
            Vantor::Renderer::Camera *m_Camera       = nullptr;
            Vantor::Math::VVector3    m_AmbientLight = {0.1f, 0.1f, 0.1f};

            //         bool m_ShadowsEnabled = true;
            //         uint32_t m_ShadowMapSize = 2048;

            //         float m_Exposure = 1.0f;
            //         float m_Gamma = 2.2f;
            //         bool m_HDREnabled = true;
            //         bool m_FXAAEnabled = true;
    };
} // namespace Vantor::RenderDevice