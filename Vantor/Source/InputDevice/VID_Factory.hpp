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
 *  Date: 2025-07-09
 *
 *  File: VID_Factory.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VID_IDevice.hpp"

#ifdef VANTOR_WM_GLFW
#include "Backend/VID_GLFWDevice.hpp"
#endif

#include <memory>

namespace Vantor::Input
{
    enum VEInputBackend
    {
        GLFW
    };

    std::shared_ptr<VIInputDevice> CreateInputDevice(Vantor::Context::Window *nativeWindow)
    {
#ifdef VANTOR_WM_GLFW
        return std::make_shared<VGLFWInputDevice>(static_cast<Vantor::Context::Window *>(nativeWindow));
#endif
        return nullptr;
    }
} // namespace Vantor::Input