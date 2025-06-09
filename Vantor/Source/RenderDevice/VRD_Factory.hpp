#pragma once

#include <memory>

// RenderDevice
#include "Interface/VRD_RenderDevice.hpp"

#if defined(VANTOR_API_OPENGL)
    #include "OpenGL/VRDO_RenderDevice.hpp"
#endif

// Core
#include "../Core/Backlog/VCO_Backlog.hpp"

namespace Vantor::RenderDevice {

    // Compile-time selected instance
    inline std::unique_ptr<VRDevice> CreateInstance() {
        #if defined(VANTOR_API_OPENGL)
            return std::make_unique<VRDeviceOpenGL>();
        #else
            Vantor::Backlog::Log("RenderDevice::Factory", "Unable to find compiled RenderAPI to create Instance.", Vantor::Backlog::LogLevel::ERR);
            return nullptr;
        #endif
    }

    // Runtime-selected instance
    inline std::unique_ptr<VRDevice> CreateInstance(VERenderAPI api) {
        switch (api) {
            case VERenderAPI::OPENGL:
                #if defined(VANTOR_API_OPENGL)
                    return std::make_unique<VRDeviceOpenGL>();
                #else
                    Vantor::Backlog::Log("RenderDevice::Factory", "OpenGL is not supported in this build.", Vantor::Backlog::LogLevel::ERR);
                #endif
            default:
                Vantor::Backlog::Log("RenderDevice::Factory", "Unsupported RenderAPI to create Instance.", Vantor::Backlog::LogLevel::ERR);
        }
    }
}
