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
 *  Date: 2025-07-01
 *
 *  File: VRD_RenderDevice.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <string>
#include <memory>

#include "VRD_Shader.hpp"
#include "VRD_Mesh.hpp"

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

    struct VMeshCreateInfo
    {
            std::vector<Vantor::Math::VVector3> Positions;
            std::vector<Vantor::Math::VVector2> UVs;
            std::vector<Vantor::Math::VVector3> Normals;
            std::vector<Vantor::Math::VVector3> Tangents;
            std::vector<Vantor::Math::VVector3> Bitangents;
            std::vector<unsigned int>           Indices;

            VE_RENDER_TOPOLOGY Topology    = VE_RENDER_TOPOLOGY::TRIANGLES;
            bool               Interleaved = true;

            // Optional SDF-based mesh generation (if used instead of raw vertex data)
            std::function<float(Vantor::Math::VVector3)> SDF            = nullptr;
            float                                        MaxDistance    = 1.0f;
            uint16_t                                     GridResolution = 32;

            bool HasSDF() const { return SDF != nullptr; }
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

            // Resource Factory
            // Shader
            virtual std::shared_ptr<VShader> CreateShader(const char *vertexCode, const char *fragmentCode) = 0;
            // Mesh : Use this with VMeshCreateInfo
            virtual std::shared_ptr<VMesh> CreateMesh(const VMeshCreateInfo &createInfo) = 0;
            // Helpers
            virtual VERenderAPI GetRenderDeviceAPI() const  = 0;
            virtual std::string GetRenderDeviceName() const = 0;

            // Device Info
            virtual std::string GetPhysicalDeviceVendor() const = 0;
            virtual std::string GetPhysicalDeviceName() const   = 0;
    };
} // namespace Vantor::RenderDevice