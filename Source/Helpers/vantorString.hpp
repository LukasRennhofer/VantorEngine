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
 *  File: vantorString.hpp
 *  Last Change: Automatically updated
 */

// TODO: Own String Type or hash functions

#pragma once

#include <string>

#define SID(string) vantor::Helpers::hashString(string)

namespace vantor::Helpers
{
    inline unsigned int hashString(const std::string &str)
    {
        unsigned int hash = 0;

        for (auto &it : str)
        {
            hash = 37 * hash + 17 * static_cast<char>(it);
        }

        return hash;
    }

    inline unsigned int hashString(const char *cStr)
    {
        std::string str(cStr);
        return hashString(str);
    }
} // namespace vantor::Helpers