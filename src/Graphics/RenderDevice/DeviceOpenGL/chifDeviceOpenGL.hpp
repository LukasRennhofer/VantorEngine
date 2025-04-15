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
 * File: chifDeviceOpenGL.hpp 
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


namespace chif::Graphics::RenderDevice::OpenGL {
    inline void Init() {
        std::cout << "[OpenGL] Init Render Device\n"; // TODO: BackLog
        // TODO: Load extensions (glad), setup state, etc.
    }

    inline void Shutdown() {
        std::cout << "[OpenGL] Shutdown Render Device\n"; // TODO: BackLog
        // TODO: ?
    }

    inline void BeginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline void EndFrame() {
        // TODO: Present swapchain maybe (depends swapchain lives)
    }
} // namespace chif::Graphics::RenderDevice::OpenGL