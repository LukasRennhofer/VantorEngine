#pragma once

#include <string>
#include <memory>

#include "VRD_Shader.hpp"

namespace Vantor::RenderDevice {
    enum VERenderAPI {
        OPENGL
        // VULKAN,
        // DIRECTX
    };

    inline std::string GetRenderAPIToString(VERenderAPI api) {
        switch (api) {
            case VERenderAPI::OPENGL:
                return std::string("OpenGL");
            default:
                return std::string("Unknown");
        }
    }

    class VRDevice {
        public:
            virtual ~VRDevice() = default;

            // Frame lifecycle
            virtual void BeginFrame() = 0;
            virtual void EndFrame() = 0;
            virtual void Present() = 0;
            
            // Create Functions
            virtual std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) = 0;

            // Helpers
            virtual VERenderAPI GetRenderDeviceAPI() const = 0;
            virtual std::string GetRenderDeviceName() const = 0;

            // Device Info
            virtual std::string GetPhysicalDeviceVendor() const = 0;
            virtual std::string GetPhysicalDeviceName() const = 0;
    };
}