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
 *  File: VAP_Application.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Core
#include "../Core/VCO_Version.hpp"
#include "../Core/BackLog/VCO_Backlog.hpp"
#include "../Core/Global/VCO_ServiceRegistry.hpp"

// Context
#include "../Context/Interface/VCT_Window.hpp"

// RenderDevice
#include "../RenderDevice/VRD_Factory.hpp"

// Platform Env
#include "../Platform/VPL_Environment.hpp"

#include <format>
#include <memory>

namespace Vantor
{
    struct VApplicationCreateInfo {
        // Window Context
        int     windowWidth         = 1280;
        int     windowHeight        = 720;
        // bool    windowFullscreen    = false; TODO
        // bool    windowResizable     = true; TODO
        // bool    windowVSync         = true; TODO
        const char* windowTitle     = "Vantor Application";
    };

    class Application
    {
        private:
            std::unique_ptr<Vantor::Context::Window>        window;
            std::unique_ptr<Vantor::RenderDevice::VRDevice> RenderDevice;

        protected:
            // Base Stats
            bool initialized = false;

        public:
            virtual ~Application() = default;

            bool is_window_active = true;

            // Runs the main engine loop
            void         Run();
            void         Shutdown();
            virtual void Initialize(VApplicationCreateInfo* info);

            void SetWindow(Vantor::Context::Window window);

            bool IsRunning();

            // RenderDevice
            RenderDevice::VRDevice       *GetRenderDevice();       // mutable access
            const RenderDevice::VRDevice *GetRenderDevice() const; // readonly access

            // TODO:
            // void SetFullScreen(bool fullscreen);
    };
} // namespace Vantor
