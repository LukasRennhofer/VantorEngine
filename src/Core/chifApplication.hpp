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
 * File: chifApplication.hpp
 * Last Change: Added Version settings and chif::core::version child namespace
*/

#pragma once

#include "../Helpers/chifTimer.hpp"
#include "../Platform/chifWindow.h"
#include "../Platform/chifInput.hpp"

#include <SDL2/SDL.h>

namespace chif
{    class Application {
        protected:
            // Base Stats
            bool initialized = false;

            // Timer
            float deltaTime = 0;
            float deltaTimeAccumulator = 0;
            chif::Helpers::Timer timer;
        
        public:
            virtual ~Application() = default;

            bool is_window_active = true;
            // chif::Platform::Window window;

            // Runs the main engine loop
            void Run();
            virtual void Initialize();

            // void SetWindow(chif::Platform::Window window);

            // TODO:
            // void SetFullScreen(bool fullscreen);
    };
} // namespace chif
