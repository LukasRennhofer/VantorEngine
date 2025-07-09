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
 *  File: VID_Manager.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include "VID_IDevice.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

namespace Vantor::Input
{
    class VInputManager
    {
        public:
            void AddDevice(std::shared_ptr<VIInputDevice> device);

            // Map a named action to physical input
            void MapAction(const std::string &actionName, const VInputButton &button);

            // Update all devices
            void Update();

            // Query
            // Is Action or single Input at this moment pressed
            bool IsActionPressed(const std::string &actionName) const;
            bool IsPressed(const int &key, const VEInputDeviceType &deviceType) const;
            // Was the Action or single Input pressed this frame
            bool WasActionPressed(const std::string &actionName) const;
            bool WasPressed(const int &key, const VEInputDeviceType &deviceType) const;

            float GetActionAxis(const std::string &actionName) const;

            std::vector<std::shared_ptr<VIInputDevice>> devices;

            std::unordered_map<std::string, std::vector<VInputButton>> actionMap;
    };
} // namespace Vantor::Input