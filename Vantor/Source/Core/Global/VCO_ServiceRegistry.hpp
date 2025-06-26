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
 *  Date: 2025-06-26
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

            // TODO: Add more systems here
        private:
            static inline RenderDevice::VRDevice *renderDevice = nullptr;
    };
} // namespace Vantor
