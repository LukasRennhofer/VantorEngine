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

namespace VE::Internal::Input {

class IMouseInputDevice : public IInputDevice {
public:
    virtual ~IMouseInputDevice() = default;
    virtual double GetX() const = 0;
    virtual double GetY() const = 0;
};

} // namespace VE::Internal::Input
