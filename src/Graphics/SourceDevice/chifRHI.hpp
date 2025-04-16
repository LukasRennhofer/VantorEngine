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
 * File: chifRHI.hpp
 * Last Change: 
*/


#pragma once

namespace chif::Graphics::SourceDevice {

    class RHI {
    public:
        virtual ~RHI() = default;

        // Initialization and cleanup
        virtual bool Init() = 0;
        virtual void Shutdown() = 0;

        // Frame control
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Present() = 0;

        // TODO: Other functions and RHI Device Handles
    };

} // namespace chif::Graphics::SourceDevice