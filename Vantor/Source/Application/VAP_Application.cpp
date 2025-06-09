/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-08
 *
 *  File: vantorApplication.cpp
 *  Last Change: Automatically updated
 */

#include "VAP_Application.hpp"
#include <Shared/glad/glad.h> // TODO: MOVE AWAY

namespace Vantor
{
    void Application::Initialize()
    {   
        // Guard for Initializing
        if (initialized)
        {
            return;
        }

        Vantor::Backlog::Log("Application", std::format("Using Vantor Core      : {}", Vantor::Core::version::GetVersionString()), Vantor::Backlog::LogLevel::INFO);

        // Initializing the Platform Environment (FS, Time, ...)
        Vantor::Platform::Environment::Initialize();

        RenderDevice = Vantor::RenderDevice::CreateInstance();

        // put the pointer of the RenderDevice into the Service Registry for access across the Engine
        VServiceLocator::SetRenderDevice(RenderDevice.get());

        // Log RenderDevice Information
        Vantor::Backlog::Log("Application", std::string("Using RenderDevice     : " + RenderDevice->GetRenderDeviceName()), Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application", std::string("Using PhysicalDevice   : " + RenderDevice->GetPhysicalDeviceName()), Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application", std::string("Using RenderAPI        : " + Vantor::RenderDevice::GetRenderAPIToString(RenderDevice->GetRenderDeviceAPI())), Vantor::Backlog::LogLevel::INFO);
        
        initialized = true;

        // TODO: Initializing with vantorInitializer
    }

    void Application::Run()
    {
        window->swapBuffers();
        window->pollEvents();
    }

    void Application::Shutdown()
    {
        Vantor::Backlog::Log("Application", "Destroying Application Context", Vantor::Backlog::LogLevel::INFO);
        window->close();
    }

    void Application::createWindow(int width, int height, const char *title)
    {
        Vantor::Backlog::Log("Application", "Creating Vantor Window Context", Vantor::Backlog::LogLevel::INFO);
        window = std::make_unique<Vantor::Context::Window>(width, height, title);

        window->setResizeCallback([](int w, int h) { glViewport(0, 0, w, h); });

        // TODO_RENDERDEVICE_OPENGL: Move away, Initialize OpenGL loader if needed (e.g., glad)
        if (!gladLoadGLLoader((GLADloadproc) window->getLoadProc()))
        {

            Vantor::Backlog::Log("Application", "Failed to initialize OpenGL Window context", Vantor::Backlog::LogLevel::ERR);
        }
    }

    bool Application::IsRunning()
    {
        // TODO: Write with new Running Context
        return !window->shouldWindowClose();
    }

    // RenderDevice: mutable access
    RenderDevice::VRDevice* Application::GetRenderDevice() {
        return RenderDevice.get();
    }

    // RenderDevice: readonly access
    const RenderDevice::VRDevice* Application::GetRenderDevice() const {
        return RenderDevice.get();
    }
} // namespace Vantor
