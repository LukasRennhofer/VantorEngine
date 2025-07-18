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
 *  File: VAP_Application.cpp
 *  Last Change: Automatically updated
 */

#include "VAP_Application.hpp"

namespace Vantor
{

    static Vantor::VApplication *s_appInstance = nullptr;

    static void ResizeCallbackStatic(int w, int h)
    {
        if (s_appInstance && Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()) Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->SetViewPort(w, h);
        VServiceLocator::SetContextWidth(w);
        VServiceLocator::SetContextHeight(h);
    }

    void VApplication::Initialize(VApplicationCreateInfo &info)
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
        window = std::make_unique<Vantor::Context::Window>(info.windowWidth, info.windowHeight, info.windowTitle);


        // Initializing the Resource Manager
        if (!Vantor::Resource::VResourceManager::Instance().Initialize())
        {
            Vantor::Backlog::Log("Application", "Resource Manager failed to initialize", Vantor::Backlog::LogLevel::ERR);
        }

        // Creating the InputManager
        InputManager = std::make_unique<Vantor::Input::VInputManager>();

        s_appInstance = this; // For ResizeCallback usage
        window->setResizeCallback(ResizeCallbackStatic);

        VServiceLocator::SetContextWidth(info.windowWidth);
        VServiceLocator::SetContextHeight(info.windowHeight);

        // Log RenderDevice Information
        // Vantor::Backlog::Log("Application", std::string("Using PhysicalDevice   : " + RenderDevice->GetPhysicalDeviceName()),
        // Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application", std::string("Using RenderDevice     : " + Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->GetRenderDeviceName()), Vantor::Backlog::LogLevel::INFO);
        Vantor::Backlog::Log("Application",
                             std::string("Using RenderAPI        : " + Vantor::RenderDevice::GetRenderAPIToString(Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->GetRenderDeviceAPI())),
                             Vantor::Backlog::LogLevel::INFO);

        // Create Context with window
        Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->CreateRenderDeviceContext(window.get());

        // Loading all Internal Resources (TODO: Work with Jobsystem)
        Vantor::Resource::VResourceManager::Instance().PreloadInternalResources();

        // Initializing the MaterialLibrary, so loading all the Default Materials (can be threaded later on)
        Vantor::Renderer::VMaterialLibrary::Instance().Initialize();

        initialized = true;

        // TODO: Initializing with vantorInitializer
    }

    void VApplication::BeginFrame()
    {
        // Record with Timer
        timer.record();

        // Update with DeltaTime
        static Vantor::Core::VTimeStamp lastTime    = timer.timestamp;
        auto                            currentTime = timer.timestamp;
        deltatime                                   = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime                                    = currentTime;

        // Update Window Events
        window->pollEvents();

        // Update the Inputs
        InputManager->Update();

        // Begin a new Frame with RenderDevice
        Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->BeginFrame();
    }

    void VApplication::EndFrame()
    {
        // End the RenderDevice frame
        Vantor::RenderDevice::VRenderCoordinator::Instance().GetRenderDevice()->EndFrame();

        // Swapchain
        window->swapBuffers();
    }

    void VApplication::Shutdown()
    {
        Vantor::Backlog::Log("Application", "Destroying VApplication Context", Vantor::Backlog::LogLevel::INFO);
        Vantor::Resource::VResourceManager::Instance().Shutdown();
        window->close();
    }

    bool VApplication::IsRunning()
    {
        if (!active)
        {
            return false;
        }
        else
        {
            return !window->shouldWindowClose();
        }
    }

    void VApplication::Break() { active = false; }

    // InputManager
    Input::VInputManager *VApplication::GetInputManager() { return InputManager.get(); }
} // namespace Vantor