#pragma once

#include "VRDO_Shader.hpp"
#include "../Interface/VRD_RenderDevice.hpp"

namespace Vantor::RenderDevice {
    class VRDeviceOpenGL : public VRDevice {
        public:
            VRDeviceOpenGL() = default;
            ~VRDeviceOpenGL() override = default;

            // Frame lifecycle
            void BeginFrame() override;
            void EndFrame()   override;
            void Present()    override;

            // Create Functions
            std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) override;

            // Helpers
            VERenderAPI GetRenderDeviceAPI() const override;
            std::string GetRenderDeviceName() const override;

            // Device Info
            std::string GetPhysicalDeviceVendor() const override;
            std::string GetPhysicalDeviceName() const override;
    };
}