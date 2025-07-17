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
 *  File: VCO_InlineDebugger.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <string>

#include "../BackLog/VCO_Backlog.hpp"

// TODO: Adding Final version of debugger
namespace Vantor::Debug
{
    inline bool DebuggingEnabled       = false; // Debugging state
    inline bool EngineDebuggingEnabled = false; // Engine Debugging state

    void EnableDebugging(bool state);
    void EnableEngineDebugging(bool state);
    void Breakpoint(std::string file, int line, bool inEngine);
} // namespace Vantor::Debug