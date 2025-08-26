/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <InputDevice/Device/Interface/VID_Device.hpp>

// TODO: Implment Enums for multiple Controller backends
// TODO: Implement Rumble States and Backend for Controller Gamepads

namespace VE::Internal::Input {

class IGamepadInputDevice : public IInputDevice {
public:
    virtual ~IGamepadInputDevice() = default;
};

} // namespace VE::Internal::Input
