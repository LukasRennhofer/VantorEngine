/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <Core/Container/VCO_Vector.hpp>
#include <InputDevice/VID_IDevice.hpp>

#include <memory>
#include <string>
#include <unordered_map>

namespace Vantor::InputDevice
{
    class VInputManager
    {
        public:
            void AddDevice(std::shared_ptr<IInputDevice> device);

            // Map a named action to physical input
            void MapAction(const std::string &actionName, const VInputButton &button);

            // Update all devices
            void Update();

            // Query
            // Is Action or single Input at this moment pressed
            bool IsActionPressed(const std::string &actionName) const;
            bool IsPressed(const int &key, const EInputDeviceType &deviceType) const;
            // Was the Action or single Input pressed this frame
            bool WasActionPressed(const std::string &actionName) const;
            bool WasPressed(const int &key, const EInputDeviceType &deviceType) const;

            float GetActionAxis(const std::string &actionName) const;

            Vantor::Core::Container::TVector<std::shared_ptr<IInputDevice>> devices;

            std::unordered_map<std::string, Vantor::Core::Container::TVector<VInputButton>> actionMap;
    };
} // namespace Vantor::InputDevice