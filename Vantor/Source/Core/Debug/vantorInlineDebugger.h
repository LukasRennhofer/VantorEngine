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
 *  File: vantorInlineDebugger.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <format>

#include "../Backlog/vantorBacklog.h"

// TODO: Adding Final version of debugger
namespace vantor::Debug
{
    inline bool DebuggingEnabled       = false; // Debugging state
    inline bool EngineDebuggingEnabled = false; // Engine Debugging state

    void EnableDebugging(bool state);
    void EnableEngineDebugging(bool state);
    void Breakpoint(std::string file, int line, bool inEngine);
} // namespace vantor::Debug
