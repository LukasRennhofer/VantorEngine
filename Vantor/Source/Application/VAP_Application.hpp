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
 *  File: VAP_Application.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// Core
#include <format>
#include <format>
#include <functional>
#include <functional>
#include <memory>
#include <memory>

#include "../Context/Interface/VCT_Window.hpp"
#include "../Core/BackLog/VCO_Backlog.hpp"
#include "../Core/Global/VCO_ServiceRegistry.hpp"
#include "../Core/VCO_Timer.hpp"
#include "../Core/VCO_Version.hpp"
#include "../InputDevice/VID_Manager.hpp"
#include "../Platform/VPL_Environment.hpp"
#include "../RenderDevice/VRD_Factory.hpp"
#include "../Resource/VRES_Manager.hpp"
#include "../Renderer/VRE_MaterialLibrary.hpp"

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

    class VApplication
    {
        private:
            std::unique_ptr<Vantor::Context::Window>        window;
            std::unique_ptr<Vantor::RenderDevice::VRDevice> RenderDevice;
            std::unique_ptr<Vantor::Input::VInputManager>   InputManager;

        protected:
            // Base Stats
            bool  initialized = false;
            bool  active      = true;
            float deltatime;

            Vantor::Core::Timer timer;

        public:
            virtual ~VApplication() = default;

            bool is_window_active = true;

            // Main Functions
            void         Break();
            void         Shutdown();
            virtual void Initialize(VApplicationCreateInfo &info);

            // Inside the loop
            void BeginFrame();
            void EndFrame();

            void                     SetWindow(Vantor::Context::Window window);
            Vantor::Context::Window *GetWindow() { return window.get(); }

            bool IsRunning();

            // Deltatime
            float GetDeltaTime() { return deltatime; }

            // RenderDevice
            RenderDevice::VRDevice       *GetRenderDevice();       // mutable access
            const RenderDevice::VRDevice *GetRenderDevice() const; // readonly access

            // Input
            Input::VInputManager *GetInputManager();

            // TODO:
            // void SetFullScreen(bool fullscreen);
    };
} // namespace Vantor