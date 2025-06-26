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
            bool  IsActionPressed(const std::string &actionName) const;
            bool  WasActionPressed(const std::string &actionName) const;
            float GetActionAxis(const std::string &actionName) const;

            std::vector<std::shared_ptr<VIInputDevice>> devices;

            std::unordered_map<std::string, std::vector<VInputButton>> actionMap;
    };
} // namespace Vantor::Input