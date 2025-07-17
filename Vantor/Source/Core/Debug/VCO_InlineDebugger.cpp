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
 *  File: VCO_InlineDebugger.cpp
 *  Last Change: Automatically updated
 */

#include "VCO_InlineDebugger.hpp"

// TODO: Adding Final version of debugger
namespace Vantor::Debug
{

    void EnableDebugging(bool state) { DebuggingEnabled = state; }

    void EnableEngineDebugging(bool state) { EngineDebuggingEnabled = state; }

    void Breakpoint(std::string file, int line, bool inEngine = false)
    {
        if (inEngine && EngineDebuggingEnabled)
        {
            Vantor::Backlog::Log(file, std::format("[Source::InEngine] Breakpoint hit at line {}", line), Vantor::Backlog::LogLevel::DEBUG);
        }
        else
        {
            if (DebuggingEnabled)
            {
                Vantor::Backlog::Log(file, std::format("Breakpoint hit at line {}", line), Vantor::Backlog::LogLevel::DEBUG);
            }
        }
    }
} // namespace Vantor::Debug