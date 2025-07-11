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
 *  File: VRD_Factory.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <memory>

// RenderDevice
#include "Interface/VRD_RenderDevice.hpp"

#if defined(VANTOR_API_OPENGL)
#include "OpenGL/VRDO_RenderDevice.hpp"
#endif

// Core
#include "../Core/Backlog/VCO_Backlog.hpp"

namespace Vantor::RenderDevice
{

    inline std::shared_ptr<VTexture> CreateTexture2DInstance(const std::string& filePath, 
                                                                const VTextureSampler& sampler,
                                                                bool generateMipmaps = true) {
        #if defined(VANTOR_API_OPENGL)
        return VOpenGLTexture2D::CreateFromFile(filePath, sampler, generateMipmaps);
        #endif
    }

    inline std::shared_ptr<VShader> CreateShaderInstance(const char *vertexCode, const char *fragmentCode) {
        #if defined(VANTOR_API_OPENGL)
        return std::make_shared<VOpenGLShader>(vertexCode, fragmentCode);
        #endif
        return nullptr;
    }

    // Compile-time selected instance
    inline std::unique_ptr<VRDevice> CreateInstance()
    {
#if defined(VANTOR_API_OPENGL)
        return std::make_unique<VRDeviceOpenGL>();
#else
        Vantor::Backlog::Log("RenderDevice::Factory", "Unable to find compiled RenderAPI to create Instance.", Vantor::Backlog::LogLevel::ERR);
        return nullptr;
#endif
    }

    // Runtime-selected instance
    inline std::unique_ptr<VRDevice> CreateInstance(VERenderAPI api)
    {
        switch (api)
        {
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
} // namespace Vantor::RenderDevice
