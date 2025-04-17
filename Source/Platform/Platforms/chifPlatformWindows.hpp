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
 * File: chifPlatformWindows.hpp
 * Last Change: 
*/

#pragma once

#ifdef __WINDOWS__

#include <string>
#include <cstdint>

namespace chif::Platform
{
    class PlatformWindows
    {
    public:
        static void Initialize();                     // Call at app start

        static double GetTimeSeconds();
        static void SleepMilliseconds(unsigned int ms);

        static std::wstring GetExecutablePath();
        static std::wstring GetExecutableDirectory();

        static bool FileExists(const std::string& path);
        static uint64_t GetFileSize(const std::string& path);

    private:
        static void CacheExecutablePath();
        static std::wstring cachedPath;
        static std::wstring cachedDir;
    };
}

#endif // __WINDOWS__