/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <Core/BackLog/VCO_Backlog.hpp>
#include <Core/Container/VCO_Vector.hpp>

namespace VE::Internal::Core::Backlog
{

    std::mutex            logMutex;
    VE::Internal::Core::Container::TVector<VLogEntry> logEntries;

    // Convert ELogLevel to String
    std::string ELogLevelToString(ELogLevel level)
    {
        switch (level)
        {
            case ELogLevel::INFO:
                return "INFO";
            case ELogLevel::DEBUG:
                return "DEBUG";
            case ELogLevel::WARNING:
                return "WARNING";
            case ELogLevel::ERR:
                return "ERROR";
        }
        return "UNKNOWN";
    }

    // Logging function
    void Log(const std::string &source, const std::string &msg, ELogLevel level)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        VLogEntry entry = {source, msg, level};
        logEntries.push_back(entry);

        // Print to console
        std::cout << "[" << ELogLevelToString(level) << "::" << source << "] " << msg << std::endl;
    }

    // // Save Logs to File
    // // TODO_PLATFORM_SWITCH: Get it working with Nintendo Switch File Saving
    // void SaveLogs(const std::string &filename)
    // {
    //     std::lock_guard<std::mutex> lock(logMutex);
    //     std::ofstream               logFile(filename, std::ios::app);

    //     if (!logFile)
    //     {
    //         Log("Backlog", "Failed to open log file", ELogLevel::ERR);
    //         return;
    //     }

    //     for (const auto &entry : logEntries)
    //     {
    //         logFile << "[" << ELogLevelToString(entry.level) << "::" << entry.source << "] " << entry.text << std::endl;
    //     }
    //     logFile.close();
    // }

} // namespace VE::Internal::Backlog