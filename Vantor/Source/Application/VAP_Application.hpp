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
 *  Date: 2025-07-01
 *
 *  File: VAP_Application.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Core
#include "../Core/VCO_Version.hpp"
#include "../Core/BackLog/VCO_Backlog.hpp"
#include "../Core/Global/VCO_ServiceRegistry.hpp"

// RenderDevice
#include "../RenderDevice/VRD_Factory.hpp"

// Context
#include "../Context/Interface/VCT_Window.hpp"

// Platform Env
#include "../Platform/VPL_Environment.hpp"

// Input
#include "../InputDevice/VID_Manager.hpp"

#include <format>
#include <memory>
#include <functional>

namespace Vantor
{
    struct VApplicationCreateInfo
    {
            // Window Context
            int windowWidth  = 1280;
            int windowHeight = 720;
            // bool    windowFullscreen    = false; TODO
            // bool    windowResizable     = true; TODO
            // bool    windowVSync         = true; TODO
            const char *windowTitle = "Vantor Application";
    };

    class Application
    {
        private:
            std::unique_ptr<Vantor::Context::Window>        window;
            std::unique_ptr<Vantor::RenderDevice::VRDevice> RenderDevice;
            std::unique_ptr<Vantor::Input::VInputManager>   InputManager;

        protected:
            // Base Stats
            bool initialized = false;
            bool active      = true;

        public:
            virtual ~Application() = default;

            bool is_window_active = true;

            // Runs the main engine loop
            void         Run(const std::function<void()> &updateFunc);
            void         Break();
            void         Shutdown();
            virtual void Initialize(VApplicationCreateInfo &info);

            void                     SetWindow(Vantor::Context::Window window);
            Vantor::Context::Window *GetWindow() { return window.get(); }

            bool IsRunning();

            // RenderDevice
            RenderDevice::VRDevice       *GetRenderDevice();       // mutable access
            const RenderDevice::VRDevice *GetRenderDevice() const; // readonly access

            // Input
            Input::VInputManager *GetInputManager();
            // TODO:
            // void SetFullScreen(bool fullscreen);
    };
} // namespace Vantor
