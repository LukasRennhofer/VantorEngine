#pragma once

#include <cmath>

namespace Vantor::Math {
    // Convert degrees to radians
    constexpr float DegToRad(float degrees) {
        return degrees * 3.14159265358979323846f / 180.0f;
    }

    // Convert radians to degrees
    constexpr float RadToDeg(float radians) {
        return radians * 180.0f / 3.14159265358979323846f;
    }
}