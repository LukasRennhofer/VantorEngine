/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

#include <Core/Container/VCO_Vector.hpp>

namespace VE::Internal::Core::Backlog
{

    // Different Log Levels
    enum class ELogLevel
    {
        INFO,
        DEBUG,
        WARNING,
        ERR
    };

    struct VLogEntry
    {
            std::string source;
            std::string text;
            ELogLevel    level;
    };

    // Function Declarations
    std::string LogLevelToString(ELogLevel level);
    void        Log(const std::string &source, const std::string &msg, ELogLevel level = ELogLevel::INFO);
    // void        SaveLogs(const std::string &filename = "Vantor.log");

    extern std::mutex logMutex;
    extern VE::Internal::Core::Container::TVector<VLogEntry> logEntries;


} // namespace VE::Internal::Backlog