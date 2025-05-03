/*
 *    				~ Vantor ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the GNU General Public License, Version 3. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: vantorPlatformSwitch.hpp
 * Last Change: 
*/

// Only include this if its building for Nintendo Switch !

#pragma once

#ifdef __SWITCH__

#include <string>
#include <cstdint>

namespace vantor::Platform
{
    class PlatformSwitch
    {
    public:
        static void Initialize();

        static double GetTimeSeconds();
        static void SleepMilliseconds(unsigned int ms);

        static std::string GetExecutablePath();       // Not usually needed on Switch
        static std::string GetExecutableDirectory();  // Will be placeholder
        static bool FileExists(const std::string& path);
        static uint64_t GetFileSize(const std::string& path);

    private:
        static double startTime;
    };
}

#endif // __SWITCH__