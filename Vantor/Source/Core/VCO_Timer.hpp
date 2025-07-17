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
 *  Date: 2025-07-16
 *
 *  File: VCO_Timer.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <chrono>

namespace Vantor::Core
{
    using VTimeStamp = std::chrono::high_resolution_clock::time_point;

    struct Timer
    {
            VTimeStamp timestamp = std::chrono::high_resolution_clock::now();

            // Record a reference timestamp
            inline void record() { timestamp = std::chrono::high_resolution_clock::now(); }

            // Elapsed time in seconds between the Vantor::Core::Timer creation or last recording and "timestamp2"
            inline double elapsed_seconds_since(VTimeStamp timestamp2)
            {
                std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp2 - timestamp);
                return time_span.count();
            }

            // Elapsed time in seconds since the Vantor::Core::Timer creation or last recording
            inline double elapsed_seconds() { return elapsed_seconds_since(std::chrono::high_resolution_clock::now()); }

            // Elapsed time in milliseconds since the Vantor::Core::Timer creation or last recording
            inline double elapsed_milliseconds() { return elapsed_seconds() * 1000.0; }

            // Elapsed time in milliseconds since the Vantor::Core::Timer creation or last recording
            inline double elapsed() { return elapsed_milliseconds(); }

            // Record a reference timestamp and return elapsed time in seconds since the Vantor::Core::Timer creation or last recording
            inline double record_elapsed_seconds()
            {
                auto timestamp2 = std::chrono::high_resolution_clock::now();
                auto elapsed    = elapsed_seconds_since(timestamp2);
                timestamp       = timestamp2;
                return elapsed;
            }
    };
} // namespace Vantor::Core