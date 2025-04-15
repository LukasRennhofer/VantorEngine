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
 * File: chifResourceManager.hpp
 * Last Change: Added Version settings and chif::core::version child namespace
*/

#pragma once

namespace chif::ResourceManager
{
    void Initialize(); // TODO
    void Clear(); // TODO

    // TODO?
    enum ResourceTypes {
        TEXTURE,
        SHADER,
        AUDIO,
        VIDEO,
        MODEl,
        UNIDENTIFIED
    }

    struct Resource {
        ResourceTypes type = UNIDENTIFIED;

        // TODO
    }
} // namespace chif::ResourceManager
