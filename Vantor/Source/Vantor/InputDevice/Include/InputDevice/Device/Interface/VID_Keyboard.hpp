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

// TODO: Add more here, because this file sucks

namespace VE::Internal::Input {

class IKeyboardInputDevice : public IInputDevice {
public:
    virtual ~IKeyboardInputDevice() = default;
};
} // namespace VE::Internal::Input
