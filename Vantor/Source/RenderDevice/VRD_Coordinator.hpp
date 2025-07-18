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
 *  Date: 2025-07-16
 *
 *  File: VRD_Factory.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <memory>

#include "Interface/VRD_RenderDevice.hpp"

#include "../Core/Types/VCO_ClassTypes.hpp" // VSingleton

#if defined(VANTOR_API_OPENGL)
#include "OpenGL/VRDO_RenderDevice.hpp"
#endif

// Core
#include "../Core/Backlog/VCO_Backlog.hpp"

namespace Vantor::RenderDevice
{

    class VRenderCoordinator : public Vantor::Core::Types::VSingleton<VRenderCoordinator> {
        friend class Vantor::Core::Types::VSingleton<VRenderCoordinator>;

        public:
            std::shared_ptr<VRDevice> CreateRenderDevice(VERenderAPI backend) {
                std::shared_ptr<VRDevice> renderDevice = nullptr;

                switch (backend) {
                    #if defined(VANTOR_API_OPENGL)
                    case VERenderAPI::OPENGL : 
                        renderDevice = std::make_shared<VRDeviceOpenGL>();
                        break;
                    #endif
                    default: 
                        Vantor::Backlog::Log("VRenderCoordinator", "GPU backend not avalible or invalid", Vantor::Backlog::LogLevel::ERR);
                        return nullptr;
                    }

                    if (renderDevice) {
                        m_RenderDevice = renderDevice;
                        m_ActiveBackend = backend;
                    }

                    return renderDevice;
            }

            VRDevice* GetRenderDevice() { return m_RenderDevice.get(); }

        private:
            VERenderAPI m_ActiveBackend;

            // RenderDevice
            std::shared_ptr<VRDevice> m_RenderDevice = nullptr;
    };


    inline std::shared_ptr<VTexture> CreateTexture2DInstance(const std::string &filePath, const VTextureSampler &sampler, bool generateMipmaps = true)
    {
#if defined(VANTOR_API_OPENGL)
        return VOpenGLTexture2D::CreateFromFile(filePath, sampler, generateMipmaps);
#endif
    }

    inline std::shared_ptr<VShader>
    CreateShaderInstance(const char *vertexCode, const char *fragmentCode, const char *fileNameVertex, const char *fileNameFragment)
    {
#if defined(VANTOR_API_OPENGL)
        return std::make_shared<VOpenGLShader>(vertexCode, fragmentCode, fileNameVertex, fileNameFragment);
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
} // namespace Vantor::RenderDevice