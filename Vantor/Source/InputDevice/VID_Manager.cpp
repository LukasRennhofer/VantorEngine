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
 *  File: VID_Manager.cpp
 *  Last Change: Automatically updated
 */

#include "VID_Manager.hpp"

namespace Vantor::Input
{
    void VInputManager::AddDevice(std::shared_ptr<VIInputDevice> device) { devices.push_back(device); }

    void VInputManager::MapAction(const std::string &actionName, const VInputButton &button) { actionMap[actionName].push_back(button); }

    void VInputManager::Update()
    {
        for (auto &device : devices)
            device->Update();
    }

    bool VInputManager::IsActionPressed(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return false;

        for (const auto &input : it->second)
        {
            for (const auto &device : devices)
            {
                if (device->IsPressed(input)) return true;
            }
        }
        return false;
    }

    bool VInputManager::IsPressed(const int &key, const VEInputDeviceType &deviceType) const
    {
        VInputButton it;
        it.id         = key;
        it.deviceType = deviceType;

        for (const auto &device : devices)
        {
            if (device->IsPressed(it)) return true;
        }

        return false;
    }

    bool VInputManager::WasActionPressed(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return false;

        for (const auto &input : it->second)
        {
            for (const auto &device : devices)
            {
                if (device->WasPressed(input)) return true;
            }
        }
        return false;
    }

    bool VInputManager::WasPressed(const int &key, const VEInputDeviceType &deviceType) const
    {
        VInputButton it;
        it.id         = key;
        it.deviceType = deviceType;

        for (const auto &device : devices)
        {
            if (device->WasPressed(it)) return true;
        }

        return false;
    }

    float VInputManager::GetActionAxis(const std::string &actionName) const
    {
        auto it = actionMap.find(actionName);
        if (it == actionMap.end()) return 0.f;

        for (const auto &input : it->second)
        {
            if (!input.isAxis) continue;

            for (const auto &device : devices)
            {
                float val = device->GetAxis(input);
                if (val != 0.f) return val;
            }
        }
        return 0.f;
    }
} // namespace Vantor::Input