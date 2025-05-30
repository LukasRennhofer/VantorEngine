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
 *  File: vantorPlatformLinux.cpp
 *  Last Change: Automatically updated
 */

#ifdef __LINUX__

#include "vantorPlatformLinux.hpp"
#include <chrono>
#include <thread>
#include <filesystem>
#include <unistd.h>
#include <limits.h>

namespace vantor::Platform
{
    std::string PlatformLinux::cachedPath;
    std::string PlatformLinux::cachedDir;

    void PlatformLinux::Initialize()
    {
        CacheExecutablePath();
        // TODO
    }

    void PlatformLinux::CacheExecutablePath()
    {
        char    buffer[PATH_MAX] = {0};
        ssize_t len              = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1)
        {
            cachedPath       = std::string(buffer, len);
            size_t lastSlash = cachedPath.find_last_of('/');
            cachedDir        = cachedPath.substr(0, lastSlash);
        }
    }

    double PlatformLinux::GetTimeSeconds()
    {
        using namespace std::chrono;
        static auto start = high_resolution_clock::now();
        auto        now   = high_resolution_clock::now();
        return duration<double>(now - start).count();
    }

    void PlatformLinux::SleepMilliseconds(unsigned int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

    std::string PlatformLinux::GetExecutablePath() { return cachedPath; }

    std::string PlatformLinux::GetExecutableDirectory() { return cachedDir; }

    bool PlatformLinux::FileExists(const std::string &path) { return std::filesystem::exists(path); }

    uint64_t PlatformLinux::GetFileSize(const std::string &path)
    {
        try
        {
            return static_cast<uint64_t>(std::filesystem::file_size(path));
        }
        catch (...)
        {
            return 0;
        }
    }
} // namespace vantor::Platform

#endif // __LINUX__