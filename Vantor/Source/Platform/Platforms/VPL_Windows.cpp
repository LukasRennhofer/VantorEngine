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
 *  File: VPL_Windows.cpp
 *  Last Change: Automatically updated
 */

#ifdef __WINDOWS__

#include "VPL_Windows.hpp"
#define NOMINMAX
#include <filesystem>
#include <fstream>
#include <windows.h>

namespace Vantor::Platform
{
    std::wstring PlatformWindows::cachedPath;
    std::wstring PlatformWindows::cachedDir;

    void PlatformWindows::Initialize()
    {
        Vantor::Backlog::Log("Platform::Windows", "Initializing Platform Environment for Windows", Vantor::Backlog::LogLevel::INFO);
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

    std::vector<uint8_t> PlatformWindows::ReadBinary(const std::string &path)
    {
        std::vector<uint8_t> buffer;

        std::ifstream file(path, std::ios::binary);
        if (!file) return buffer;

        file.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        buffer.resize(size);
        file.read(reinterpret_cast<char *>(buffer.data()), size);

        return buffer;
    }

    std::string PlatformWindows::ReadText(const std::string &path)
    {
        std::ifstream file(path);
        if (!file) return {};

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
} // namespace Vantor::Platform

#endif // __WINDOWS_