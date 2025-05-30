/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-05-12
 *
 *  File: valib.hpp
 *  Last Change: Automatically updated
 */

namespace valib {

    // TimeStep
    struct TimeStep {
        float deltaTime = 0.0f;
        float totalTime = 0.0f;
        int frameCount = 0;
    };

    // Sizes of different Data in bits
    enum class DataSize : uint64_t {
        BIT     = 1,
        BYTE    = 8 * BIT,
        KILOBIT = 1024 * BIT,
        KILOBYTE = 1024 * BYTE,
        MEGABYTE = 1024 * KILOBYTE,
        GIGABYTE = 1024 * MEGABYTE
    };
}