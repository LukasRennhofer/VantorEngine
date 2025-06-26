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
 *  File: VRDO_RenderDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VRDO_Shader.hpp"
#include "../Interface/VRD_RenderDevice.hpp"

namespace Vantor::RenderDevice
{
    class VRDeviceOpenGL : public VRDevice
    {
        public:
            VRDeviceOpenGL()           = default;
            ~VRDeviceOpenGL() override = default;

            void Initialize() override;

            void SetViewPort(int w, int h) override;
            void CreateRenderDeviceContext(Vantor::Context::Window *window) override;

            // Frame lifecycle
            void BeginFrame() override;
            void EndFrame() override;
            void Present() override;

            // Create Functions
            std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) override;

            // Helpers
            VERenderAPI GetRenderDeviceAPI() const override;
            std::string GetRenderDeviceName() const override;

            // Device Info
            std::string GetPhysicalDeviceVendor() const override;
            std::string GetPhysicalDeviceName() const override;
    };
} // namespace Vantor::RenderDevice