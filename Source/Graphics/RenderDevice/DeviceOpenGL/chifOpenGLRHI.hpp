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

#include <glad/glad.h>
#include "../../../Core/BackLog/chifBacklog.h"

namespace chif::Graphics::RenderDevice::OpenGL {
 
    inline void Init() {
        ::chif::Backlog::Log("OpenGLRHI", "Init of OpenGL Render Device", ::chif::Backlog::LogLevel::INFO);
        // TODO: Load extensions (glad), setup state, etc.
    }

    inline void Shutdown() {
        ::chif::Backlog::Log("OpenGLRHI", "Shutdown of OpenGL Render Device", ::chif::Backlog::LogLevel::INFO);
        // TODO: ?
    }

    inline void BeginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void EndFrame() {
         // TODO: End Command Process and flush commands to GPU
    }

    inline void Present() {
        // TODO: Call Swapchain to change buffers with context and render
    }
} // namespace chif::Graphics::RenderDevice::OpenGL