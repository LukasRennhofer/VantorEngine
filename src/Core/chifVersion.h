
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
 * File: chifVersion.h
 * Last Change: Updated projects docs
 */

#pragma once

#include <string>

namespace chif::Core::version {

    // return: const int major
    int GetMajor();
    // return: const int minor
    int GetMinor();
    // return: const int revision
    int GetRevision();

    std::string GetVersionString();
} // NAMESPACE CHIF::CORE::VERSION