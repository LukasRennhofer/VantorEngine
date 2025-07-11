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
 *  Date: 2025-07-11
 *
 *  File: VRDO_RenderDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../Interface/VRD_RenderDevice.hpp"
#include "VRDO_Mesh.hpp"
#include "VRDO_RenderPath.hpp"
#include "VRDO_Shader.hpp"
#include "VRDO_Texture.hpp"

#include "VRDO_RenderPath.hpp"

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

            // RenderPaths
            std::shared_ptr<VRenderPath3D> CreateRenderPath3D() override
            {
                // Initialize the RenderPath with the RenderDevice
                std::shared_ptr<VRenderPath3DGL> rPath = std::make_shared<VRenderPath3DGL>();
                rPath->Initialize(this);
                m_RenderPaths.push_back(rPath);
                return rPath;
            }

            // Frame lifecycle
            void BeginFrame() override;
            void EndFrame() override;

            // Resource Factory
            // Shader
            // std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) override;
            // Mesh : Use this with VMeshCreateInfo
            std::shared_ptr<VMesh> CreateMesh(const VMeshCreateInfo &createInfo) override;

            // Helpers
            VERenderAPI GetRenderDeviceAPI() const override;
            std::string GetRenderDeviceName() const override;

            // Device Info
            std::string GetPhysicalDeviceVendor() const override;
            std::string GetPhysicalDeviceName() const override;

        private:
            std::vector<std::shared_ptr<VRenderPath>> m_RenderPaths;
    };
} // namespace Vantor::RenderDevice