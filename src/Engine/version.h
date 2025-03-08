
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
 * File: version.h
 * Last Change: Added Version settings and chif::version child namespace
 */

#pragma once

#include <string>

namespace chif::version {

    // return: const int major
    int GetMajor();
    // return: const int minor
    int GetMinor();
    // return: const int revision
    int GetRevision();

    std::string GetVersionString();
}