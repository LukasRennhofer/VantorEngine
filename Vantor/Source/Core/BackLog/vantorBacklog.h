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
 *  File: vantorBacklog.h
 *  Last Change: Automatically updated
 */

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>

namespace vantor::Backlog
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

} // namespace vantor::Backlog