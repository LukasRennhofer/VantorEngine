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
 *  Date: 2025-06-26
 *
 *  File: VRD_RenderDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>
#include <memory>

#include "VRD_Shader.hpp"

// Context
#include "../../Context/Interface/VCT_Window.hpp"

namespace Vantor::RenderDevice
{
    enum VERenderAPI
    {
        OPENGL
        // VULKAN,
        // DIRECTX
    };

    inline std::string GetRenderAPIToString(VERenderAPI api)
    {
        switch (api)
        {
            case VERenderAPI::OPENGL:
                return std::string("OpenGL");
            default:
                return std::string("Unknown");
        }
    }

    class VRDevice
    {
        public:
            virtual ~VRDevice() = default;

            // Frame lifecycle
            virtual void Initialize() = 0;

            virtual void BeginFrame() = 0;
            virtual void EndFrame()   = 0;
            virtual void Present()    = 0;

            virtual void SetViewPort(int w, int h)                                  = 0;
            virtual void CreateRenderDeviceContext(Vantor::Context::Window *window) = 0;

            // Create Functions
            virtual std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) = 0;

            // Helpers
            virtual VERenderAPI GetRenderDeviceAPI() const  = 0;
            virtual std::string GetRenderDeviceName() const = 0;

            // Device Info
            virtual std::string GetPhysicalDeviceVendor() const = 0;
            virtual std::string GetPhysicalDeviceName() const   = 0;
    };
} // namespace Vantor::RenderDevice