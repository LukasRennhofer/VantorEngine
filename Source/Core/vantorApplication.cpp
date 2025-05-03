/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorApplication.cpp
 * Last Change: Added Version settings and vantor::core::version child namespace
*/

#include "vantorApplication.hpp"
#include "JobSystem/vantorJobSystem.h"
#include "../Graphics/RenderDevice/vantorRenderDevice.hpp"

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
        vantor::Backlog::Log("Application", "Using RenderDevice " + vantor::Graphics::RenderDevice::apiToString(vantor::Graphics::RenderDevice::RenderAPI::OPENGL), vantor::Backlog::LogLevel::DEBUG);

	}

    void Application::Run() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            vantor::Platform::Input::ProcessEvent(event);
        }

        vantor::Platform::Input::Update();
        // TODO
    }

    // void Application::SetWindow(vantor::Platform::Window window) {
    //     this->window = window;
    // }
} // namespace vantor
