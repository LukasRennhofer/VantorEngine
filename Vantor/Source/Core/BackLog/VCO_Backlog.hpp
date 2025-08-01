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
 *  File: VCO_Backlog.hpp
 *  Last Change: Automatically updated
 */

#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

namespace Vantor::Backlog
{

    // Different Log Levels
    enum class LogLevel
    {
        INFO,
        DEBUG,
        WARNING,
        ERR
    };

    struct LogEntry
    {
            std::string source;
            std::string text;
            LogLevel    level;
    };

    // Function Declarations
    std::string LogLevelToString(LogLevel level);
    void        Log(const std::string &source, const std::string &msg, LogLevel level = LogLevel::INFO);
    void        SaveLogs(const std::string &filename = "Vantor.log");

    extern std::mutex logMutex;
    extern std::vector<LogEntry> logEntries;


} // namespace Vantor::Backlog