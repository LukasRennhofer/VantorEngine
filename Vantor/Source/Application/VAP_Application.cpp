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
 *  Date: 2025-06-09
 *
 *  File: VAP_Application.cpp
 *  Last Change: Automatically updated
 */

#include "VAP_Application.hpp"
#include <Shared/glad/glad.h> // TODO: MOVE AWAY

namespace Vantor
{

    static Vantor::Application* s_appInstance = nullptr;

    static void ResizeCallbackStatic(int w, int h) {
    if (s_appInstance && s_appInstance->GetRenderDevice())
        s_appInstance->GetRenderDevice()->SetViewPort(w, h);
    }


    void Application::Initialize(VApplicationCreateInfo* info)
    {
        // Guard for Initializing
        if (initialized)
        {
            return;
        }

        Vantor::Backlog::Log("Application", std::format("Using Vantor Core      : {}", Vantor::Core::Version::GetVersionString()),
                             Vantor::Backlog::LogLevel::INFO);

        // Initializing the Platform Environment (FS, Time, ...)
        Vantor::Platform::Environment::Initialize();

        // Context
        Vantor::Backlog::Log("Application", "Creating Vantor Window Context", Vantor::Backlog::LogLevel::INFO);
        window = std::make_unique<Vantor::Context::Window>(info->windowWidth, info->windowHeight, info->windowTitle);

        // Creating the RenderDevice
        RenderDevice = Vantor::RenderDevice::CreateInstance();

        s_appInstance = this; // For ResizeCallback usage
        window->setResizeCallback(ResizeCallbackStatic);

        // put the pointer of the RenderDevice into the Service Registry for access across the Engine
        VServiceLocator::SetRenderDevice(RenderDevice.get());

        // Log RenderDevice Information
        // Vantor::Backlog::Log("Application", std::string("Using PhysicalDevice   : " + RenderDevice->GetPhysicalDeviceName()), Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application", std::string("Using RenderDevice     : " + RenderDevice->GetRenderDeviceName()), Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application",
                             std::string("Using RenderAPI        : " + Vantor::RenderDevice::GetRenderAPIToString(RenderDevice->GetRenderDeviceAPI())),
                             Vantor::Backlog::LogLevel::INFO);

        // Create Context with window
        RenderDevice->CreateRenderDeviceContext(window.get());

        initialized = true;

        // TODO: Initializing with vantorInitializer
    }

    void Application::Run()
    {
        window->swapBuffers();
        window->pollEvents();

        // Begin a new Frame
        RenderDevice->BeginFrame();
        RenderDevice->EndFrame(); // TODO: Not working rn
    }

    void Application::Shutdown()
    {
        Vantor::Backlog::Log("Application", "Destroying Application Context", Vantor::Backlog::LogLevel::INFO);
        window->close();
    }

    bool Application::IsRunning()
    {
        // TODO: Write with new Running Context
        return !window->shouldWindowClose();
    }

    // RenderDevice: mutable access
    RenderDevice::VRDevice *Application::GetRenderDevice() { return RenderDevice.get(); }

    // RenderDevice: readonly access
    const RenderDevice::VRDevice *Application::GetRenderDevice() const { return RenderDevice.get(); }
} // namespace Vantor
