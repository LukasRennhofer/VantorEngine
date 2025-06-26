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
 *  File: VRDO_RenderDevice.cpp
 *  Last Change: Automatically updated
 */

#include "VRDO_RenderDevice.hpp"

// Core/Backlog
#include "../../Core/BackLog/VCO_Backlog.hpp"

namespace Vantor::RenderDevice
{
    void VRDeviceOpenGL::Initialize() {} // TODO
    void VRDeviceOpenGL::SetViewPort(int w, int h) { glViewport(0, 0, w, h); }

    void VRDeviceOpenGL::CreateRenderDeviceContext(Vantor::Context::Window *window)
    {

        if (!gladLoadGLLoader((GLADloadproc) window->getLoadProc()))
        {
            Vantor::Backlog::Log("Application", "Failed to initialize OpenGL Window context", Vantor::Backlog::LogLevel::ERR);
        }
    }

    void VRDeviceOpenGL::BeginFrame() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void VRDeviceOpenGL::EndFrame()
    {
        // TODO: flush, prepare for present
    }

    void VRDeviceOpenGL::Present()
    {
        // TODO: Typically handled in the main loop with glfwSwapBuffers(window)
    }

    std::shared_ptr<VShader> VRDeviceOpenGL::CreateShader(const char *vertexCode, const char *fragmentCode)
    {
        return std::make_shared<VOpenGLShader>(vertexCode, fragmentCode);
    }

    VERenderAPI VRDeviceOpenGL::GetRenderDeviceAPI() const { return VERenderAPI::OPENGL; }

    std::string VRDeviceOpenGL::GetRenderDeviceName() const { return "VRDeviceOpenGL"; }

    // Device Info
    std::string VRDeviceOpenGL::GetPhysicalDeviceVendor() const
    {
        const GLubyte *vendor = glGetString(GL_VENDOR);
        return std::string(reinterpret_cast<const char *>(vendor));
    }

    std::string VRDeviceOpenGL::GetPhysicalDeviceName() const
    {
        const GLubyte *renderer = glGetString(GL_RENDERER);
        return std::string(reinterpret_cast<const char *>(renderer));
    }
} // namespace Vantor::RenderDevice