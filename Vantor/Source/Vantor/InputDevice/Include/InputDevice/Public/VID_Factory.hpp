/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <InputDevice/Public/VID_IDevice.hpp>

#include <Context/Interface/VCT_Window.hpp>

#ifdef VANTOR_WM_GLFW
#include <InputDevice/Backend/VID_BackendGLFW.hpp>
#endif

#include <memory>

namespace VE::Input
{
    enum EInputBackend
    {
        GLFW
    };

    std::shared_ptr<VInputDevice> CreateInputDevice(VE::Internal::Context::VWindow *nativeWindow)
    {
#ifdef VANTOR_WM_GLFW
        return std::make_shared<VGLFWInputDevice>(static_cast<VE::Internal::Context::VWindow *>(nativeWindow));
#endif
        return nullptr;
    }
} // namespace Vantor::Input