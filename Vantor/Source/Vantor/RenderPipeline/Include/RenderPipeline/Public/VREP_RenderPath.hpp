/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Graphics/Public/Camera/VGFX_Camera.hpp>
#include <RenderPipeline/Public/VREP_RenderPass.hpp>

#include <Math/Linear/VMA_Matrix.hpp>

#include <RenderPipeline/Pipeline/VREP_CommandBuffer.hpp>

#include <memory>
#include <cstdint>

namespace VE::Internal::RHI {
    class IRHIRenderTarget;
    class IRHIMesh;
    class IRHIShader;
    class IRHIDevice;
}

namespace VE {
    class VMaterial;
}

namespace VE::Render {

    // Base render path interface
    class VRenderPath
    {
        public:
            virtual ~VRenderPath() = default;

            // Core rendering methods
            virtual void Initialize(VE::Internal::RHI::IRHIDevice* device) = 0;
            virtual void Render() = 0;
            virtual void Shutdown() = 0;

            // Render target management
            void           SetInputRenderTarget(VE::Internal::RHI::IRHIRenderTarget *target) { m_IN = target; };
            VE::Internal::RHI::IRHIRenderTarget *GetOutputRenderTarget() const { return m_OUT; };

            // Viewport configuration
            virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)           = 0;
            // virtual void GetViewport(uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height) const = 0;

            // Render pass management
            virtual void         AddRenderPass(std::unique_ptr<VRenderPass> pass) = 0;
            virtual VRenderPass *GetRenderPass(ERenderPassType type) const        = 0;

            void SetCamera(VE::Graphics::ACamera* camera) { m_Camera = camera; };
            VE::Graphics::ACamera* GetCamera() const { return m_Camera; };

            VE::Internal::RenderPipeline::VCommandBuffer* GetCommandBuffer() const { return m_CommandBuffer.get(); }
            VE::Internal::RHI::IRHIDevice* GetDevice() const { return m_Device; }


        protected:
            VE::Graphics::ACamera* m_Camera;

            uint32_t m_ViewportX      = 0;
            uint32_t m_ViewportY      = 0;
            uint32_t m_ViewportWidth  = 1280;
            uint32_t m_ViewportHeight = 720;

            VE::Internal::RHI::IRHIRenderTarget* m_IN;
            VE::Internal::RHI::IRHIRenderTarget* m_OUT;

            VE::Internal::RHI::IRHIDevice* m_Device;

            std::unique_ptr<VE::Internal::RenderPipeline::VCommandBuffer> m_CommandBuffer;
    };

    class VRenderPath3D : public VRenderPath
    {
        public:
            VRenderPath3D();
            virtual ~VRenderPath3D() = default;

            // Core rendering methods
            void Initialize(VE::Internal::RHI::IRHIDevice *device) override;
            void Render() override;
            void Shutdown() override;

            // Push to CommandBufferr
            void PushRender(VE::Graphics::VModel* model, VMaterial* material, const VE::Math::VMat4& transform);
            // void PushRender(Vantor::Object::VObject *object) override;

            // void PushPointLight(const Vantor::Renderer::VPointLightData &pointLightData) override;

            // Viewport configuration
            void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
            // void GetViewport(uint32_t &x, uint32_t &y, uint32_t &width, uint32_t &height) const override;

            // Render pass management
            void         AddRenderPass(std::unique_ptr<VRenderPass> pass) override;
            VRenderPass *GetRenderPass(ERenderPassType type) const override;

        private:
            void SetupDefaultRenderPasses();

            std::unordered_map<ERenderPassType, std::unique_ptr<VRenderPass>> m_RenderPasses;
    };

}

