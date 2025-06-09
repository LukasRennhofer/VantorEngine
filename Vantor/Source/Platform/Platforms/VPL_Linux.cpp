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
 *  Date: 2025-06-09
 *
 *  File: VPL_Linux.cpp
 *  Last Change: Automatically updated
 */

#ifdef __LINUX__

#include "VPL_Linux.hpp"
#include <chrono>
#include <thread>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <fstream>

namespace Vantor::Platform
{
    std::string PlatformLinux::cachedPath;
    std::string PlatformLinux::cachedDir;

    void PlatformLinux::Initialize()
    {
        Vantor::Backlog::Log("Platform::Linux", "Initializing Platform Environment for Linux", Vantor::Backlog::LogLevel::INFO);
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

    std::vector<uint8_t> PlatformLinux::ReadBinary(const std::string &path)
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

    std::string PlatformLinux::ReadText(const std::string &path)
    {
        std::ifstream file(path);
        if (!file) return {};

        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
} // namespace Vantor::Platform

#endif // __LINUX__