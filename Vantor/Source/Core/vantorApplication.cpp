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
 *  Date: 2025-05-12
 *
 *  File: vantorApplication.cpp
 *  Last Change: Automatically updated
 */

#include "vantorApplication.hpp"
#include "JobSystem/vantorJobSystem.h"
#include "../Graphics/RenderDevice/vantorRenderDevice.hpp"

#include "Resource/vantorResource.hpp"

// TODO: vantor::Application::Shutdown(); function

namespace vantor
{
    void Application::Initialize()
    {
        if (initialized)
        {
            return;
        }
        vantor::Backlog::Log("Application", std::format("Using Vantor Core: {}", vantor::Core::version::GetVersionString()), vantor::Backlog::LogLevel::INFO);

        initialized = true;

        // TODO: Initializing with vantorInitializer
        vantor::Platform::Input::Initialize();
        vantor::Backlog::Log("Application", "Using RenderDevice " + vantor::Graphics::RenderDevice::apiToString(vantor::Graphics::RenderDevice::OPENGL),
                             vantor::Backlog::LogLevel::DEBUG);
        vantor::Resources::Init();
    }

    void Application::Run()
    {
        vantor::Platform::Input::Update();
        // TODO
    }

    // void Application::SetWindow(vantor::Platform::Window window) {
    //     this->window = window;
    // }
} // namespace vantor
