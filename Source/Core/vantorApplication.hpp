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
 *  Date: 2025-05-11
 *
 *  File: vantorApplication.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "../Helpers/vantorTimer.hpp"
#include "../Platform/vantorWindow.h"
#include "../Platform/vantorInput.hpp"

#include <SDL2/SDL.h>

namespace vantor
{
    class Application
    {
        protected:
            // Base Stats
            bool initialized = false;

            // Timer
            float                  deltaTime            = 0;
            float                  deltaTimeAccumulator = 0;
            vantor::Helpers::Timer timer;

        public:
            virtual ~Application() = default;

            bool is_window_active = true;
            // vantor::Platform::Window window;

            // Runs the main engine loop
            void         Run();
            virtual void Initialize();

            // void SetWindow(vantor::Platform::Window window);

            // TODO:
            // void SetFullScreen(bool fullscreen);
    };
} // namespace vantor
