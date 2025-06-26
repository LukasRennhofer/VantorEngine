#pragma once

#include "VID_IDevice.hpp"

#ifdef VANTOR_WM_GLFW
#include "Backend/VID_GLFWDevice.hpp"
#endif

#include <memory>

namespace Vantor::Input {
    enum VEInputBackend {
        GLFW
    };

    std::shared_ptr<VIInputDevice> CreateInputDevice(Vantor::Context::Window* nativeWindow)
    {
        #ifdef VANTOR_WM_GLFW
            return std::make_shared<VGLFWInputDevice>(static_cast<Vantor::Context::Window*>(nativeWindow));
        #endif
        return nullptr;
    }
}