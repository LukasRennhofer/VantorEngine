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
 * File: chifOpenGLRHI.hpp 
 * Last Change: 
*/

/*
             Graphics Renderer Device 
                ====== OpenGL ======

    Implemented:
        : Factory : 
            - OpenGL API implementation: TODO:

        : Shader :
            - OpenGL Shader: Finished
            - OpenGL BaseShader: Finished

        : Render Context : 
            - OpenGL Context: TODO: Window Creation / Binding with SDL2 so just context binding and the swap chain
        
        : Buffer :
            - OpenGL Buffers: TODO:
            - OpengL Uniform/Framebuffer: TODO:
*/

#pragma once

#include "../../SourceDevice/chifRHI.hpp"

namespace chif::Graphics::RenderDevice::OpenGL {

    class OpenGLRHI : public chif::Graphics::SourceDevice::RHI {
    public:
        bool Init() override; // TODO
        void Shutdown() override; // TODO

        void BeginFrame() override; // TODO
        void EndFrame() override; // TODO
        void Present() override; // TODO

        // TODO: Using Shaders and Buffers
    } 
}// namespace chif::Graphics::RenderDevice::OpenGL