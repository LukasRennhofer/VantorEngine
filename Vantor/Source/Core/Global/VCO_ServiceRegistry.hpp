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
 *  Date: 2025-07-09
 *
 *  File: VCO_ServiceRegistry.hpp
 *  Last Change: Automatically updated
 */

#pragma once

// RenderDevice
#include "../../RenderDevice/Interface/VRD_RenderDevice.hpp"

namespace Vantor
{
    class VServiceLocator
    {
        public:
            static void                    SetRenderDevice(RenderDevice::VRDevice *dev) { renderDevice = dev; }
            static RenderDevice::VRDevice *GetRenderDevice() { return renderDevice; }

            static uint32_t GetContextWidth() {return ctxW;}
            static uint32_t GetContextHeight() {return ctxH;}

            static void SetContextWidth(uint32_t w) {ctxW = w;}
            static void SetContextHeight(uint32_t h) {ctxH = h;}
            // TODO: Add more systems here
        private:
            static inline RenderDevice::VRDevice *renderDevice = nullptr;

            // Context Infos for public
            // Only working after Context got initialized
            // NOTE (Lukas): Should implement a version with Singleton for Service Locator
            static uint32_t ctxW;
            static uint32_t ctxH;
    };
} // namespace Vantor
