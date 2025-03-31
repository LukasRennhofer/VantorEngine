/*
 *    				~ CHIFEngine ~
 *
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: chifBacklog.cpp
 * Last Change:
*/

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>

namespace chif::Backlog {

    // Different Log Levels
    enum class LogLevel { INFO, DEBUG, WARNING, ERR};

    struct LogEntry {
        std::string source;
        std::string text;
        LogLevel level;
    };

    // Function Declarations
    std::string LogLevelToString(LogLevel level);
    void Log(const std::string& source, const std::string& msg, LogLevel level = LogLevel::INFO);
    void SaveLogs(const std::string& filename = "CHIFEngine.log");

} // namespace chif::Backlog