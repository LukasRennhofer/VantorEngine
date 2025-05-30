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
 *  File: vantorPlatformWindows.cpp
 *  Last Change: Automatically updated
 */

#ifdef __WINDOWS__

#include "vantorPlatformWindows.hpp"
#define NOMINMAX
#include <Windows.h>
#include <filesystem>

namespace vantor::Platform
{
    std::wstring PlatformWindows::cachedPath;
    std::wstring PlatformWindows::cachedDir;

    void PlatformWindows::Initialize()
    {
        CacheExecutablePath();
        // TODO
    }

    void PlatformWindows::CacheExecutablePath()
    {
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        cachedPath = std::wstring(buffer);

        size_t lastSlash = cachedPath.find_last_of(L"\\/");
        cachedDir        = cachedPath.substr(0, lastSlash);
    }

    double PlatformWindows::GetTimeSeconds()
    {
        static LARGE_INTEGER freq;
        static bool          initialized = false;

        if (!initialized)
        {
            QueryPerformanceFrequency(&freq);
            initialized = true;
        }

        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return static_cast<double>(now.QuadPart) / freq.QuadPart;
    }

    void PlatformWindows::SleepMilliseconds(unsigned int ms) { ::Sleep(ms); }

    std::wstring PlatformWindows::GetExecutablePath() { return cachedPath; }

    std::wstring PlatformWindows::GetExecutableDirectory() { return cachedDir; }

    bool PlatformWindows::FileExists(const std::string &path) { return std::filesystem::exists(path); }

    uint64_t PlatformWindows::GetFileSize(const std::string &path)
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

#endif // __WINDOWS_
