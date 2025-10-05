/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <InputDevice/Public/VID_IDevice.hpp>


namespace VE::Internal::InputDevice
{
    class VInputManager
    {
        public:
            void AddDevice(std::shared_ptr<VE::Input::VInputDevice> device);

            // Map a named action to physical input
            void MapAction(const std::string &actionName, const VE::Input::VInputButton &button);

            // Update all devices
            void Update();

            // Is Action or single Input at this moment pressed
            bool IsActionPressed(const std::string &actionName) const;
            bool IsPressed(const int &key, const VE::Input::EInputDeviceType &deviceType) const;

            // Was the Action or single Input pressed this frame
            bool WasActionPressed(const std::string &actionName) const;
            bool WasPressed(const int &key, const VE::Input::EInputDeviceType &deviceType) const;

            float GetActionAxis(const std::string &actionName) const;

            std::vector<std::shared_ptr<VE::Input::VInputDevice>> devices;

            std::unordered_map<std::string, std::vector<VE::Input::VInputButton>> actionMap;
    };
} // namespace VE::Input