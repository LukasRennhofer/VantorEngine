/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <InputDevice/VID_IDevice.hpp>

#ifdef VANTOR_WM_GLFW
#include <InputDevice/Backend/VID_GLFWDevice.hpp>
#endif

#include <memory>

namespace Vantor::InputDevice
{
    enum EInputBackend
    {
        GLFW
    };

    std::shared_ptr<IInputDevice> CreateInputDevice(Vantor::Context::VWindow *nativeWindow)
    {
#ifdef VANTOR_WM_GLFW
        return std::make_shared<VGLFWInputDevice>(static_cast<Vantor::Context::VWindow *>(nativeWindow));
#endif
        return nullptr;
    }
} // namespace Vantor::InputDevice