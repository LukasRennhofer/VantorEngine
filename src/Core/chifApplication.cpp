/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifApplication.cpp
 * Last Change: Added Version settings and chif::core::version child namespace
*/

#include "chifApplication.hpp"
#include "JobSystem/chifJobSystem.h"

namespace chif
{
    void Application::Initialize()
	{
		if (initialized)
		{
			return;
		}
        chif::Backlog::Log("Application", std::format("Using CHIFEngine Core: {}", chif::Core::version::GetVersionString()), chif::Backlog::LogLevel::INFO);

		initialized = true;

        // TODO: Initializing with chifInitializer
        chif::Platform::Input::Initialize();
	}

    void Application::Run() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            chif::Platform::Input::ProcessEvent(event);
        }

        chif::Platform::Input::Update();
        // TODO
    }

    // void Application::SetWindow(chif::Platform::Window window) {
    //     this->window = window;
    // }
} // namespace chif
