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
 *  File: VRE_Sphere.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VRE_GeomtetryObject.hpp"
#include "../../RenderDevice/Interface/VRD_RenderDevice.hpp"

namespace Vantor::Renderer
{
    class VSphere : public VGObject
    {
        public:
            VSphere(Vantor::RenderDevice::VRDevice *device, unsigned int xSegments, unsigned int ySegments);
    };
} // namespace Vantor::Renderer