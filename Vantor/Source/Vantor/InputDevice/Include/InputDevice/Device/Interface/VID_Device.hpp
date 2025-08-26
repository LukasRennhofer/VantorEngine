/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <cstdint>
#include <vector>
#include <functional>

namespace VE::Internal::Input {

enum class EInputEventType {
    ButtonDown,
    ButtonUp,
    AxisChanged
};

struct VInputEvent {
    EInputEventType type;
    int code; // Key/button/axis code
    float value; // 1.0 for down, 0.0 for up, axis value otherwise
};

class IInputDevice {
public:
    virtual ~IInputDevice() = default;
    // Update Device
    virtual void Poll() = 0;

    // Process Event internally
    virtual void ProcessEvent(const VInputEvent& event) = 0;
    // For fast input
    virtual bool IsButtonDown(int code) const = 0;

    virtual bool WasButtonJustPressed(int code) const = 0;
    virtual bool WasButtonJustReleased(int code) const = 0;

    // Returns 0 on Keyboard / Mouse
    virtual float GetAxis(int code) const = 0;
};

} // namespace VE::Internal::Input
