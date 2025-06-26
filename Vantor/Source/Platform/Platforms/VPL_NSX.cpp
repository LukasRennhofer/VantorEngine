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
 *  Date: 2025-06-26
 *
 *  File: VPL_NSX.cpp
 *  Last Change: Automatically updated
 */

#ifdef __NSX__

#include "VPL_NSX.hpp"
#include <switch.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cstring>

namespace Vantor::Platform
{
    double PlatformNSX::startTime = 0.0;

    void PlatformNSX::Initialize()
    {
        Vantor::Backlog::Log("Platform::NSX", "Initializing Platform Environment for Nintendo Switch", Vantor::Backlog::LogLevel::INFO);
        socketInitializeDefault();
        romfsInit();
        startTime = GetTimeSeconds();
    }

    double PlatformNSX::GetTimeSeconds()
    {
        TimeSpan ts;
        timeGetCurrentTime(TimeType_UserSystemClock, &ts);
        return static_cast<double>(ts.nanoseconds) / 1'000'000'000.0;
    }

    void PlatformNSX::SleepMilliseconds(unsigned int ms) { svcSleepThread(static_cast<int64_t>(ms) * 1'000'000); }

    std::string PlatformNSX::GetExecutablePath()
    {
        return "romfs:/"; // Default on homebrew
    }

    std::string PlatformNSX::GetExecutableDirectory()
    {
        return "romfs:/"; // Safe fallback
    }

    bool PlatformNSX::FileExists(const std::string &path)
    {
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
    }

    uint64_t PlatformNSX::GetFileSize(const std::string &path)
    {
        struct stat st;
        if (stat(path.c_str(), &st) == 0) return static_cast<uint64_t>(st.st_size);
        return 0;
    }

    std::vector<uint8_t> PlatformNSX::ReadBinary(const std::string &path)
    {
        std::vector<uint8_t> buffer;

        FILE *file = fopen(path.c_str(), "rb");
        if (!file) return buffer;

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        buffer.resize(size);
        fread(buffer.data(), 1, size, file);
        fclose(file);

        return buffer;
    }

    std::string PlatformNSX::ReadText(const std::string &path)
    {
        FILE *file = fopen(path.c_str(), "r");
        if (!file) return {};

        std::ostringstream ss;
        char               buf[512];
        while (size_t len = fread(buf, 1, sizeof(buf), file))
        {
            ss.write(buf, len);
        }

        fclose(file);
        return ss.str();
    }
} // namespace Vantor::Platform

#endif // __NSX__
