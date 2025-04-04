/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-04-02
 *
 * File: chifInlineDebugger.h
 * Last Change:
 */
 

#pragma once

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <format>

#include "../Backlog/chifBacklog.h"

// TODO: Adding Final version of debugger
namespace chif::Debug {
    inline bool DebuggingEnabled = false; // Debugging state
    inline bool EngineDebuggingEnabled = false; // Engine Debugging state

    void EnableDebugging(bool state);
    void EnableEngineDebugging(bool state);
    void Breakpoint(std::string file, int line, bool inEngine);
} // namespace chif::Debug
