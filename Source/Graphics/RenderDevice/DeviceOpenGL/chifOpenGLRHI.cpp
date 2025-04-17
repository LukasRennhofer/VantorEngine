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
 * File: chifOpenGLRHI.cpp 
 * Last Change: 
*/

#include <glad/glad.h>

#include "chifOpenGLRHI.hpp"
#include "../../../Core/BackLog/chifBacklog.h"

namespace chif::Graphics::RenderDevice::OpenGL {
    // ======= OpenGL RHI ======
    OpenGLRHI::~OpenGLRHI {
        // TODO
    }
    // ====== Base Functions =====
    bool OpenGLRHI::Init() {
        chif::Backlog::Log("OpenGLRHI", "Init of OpenGL Render Device", chif::Backlog::LogLevel::INFO);

        return true;
        // TODO: Load extensions (glad), setup state, etc.
    }

    void OpenGLRHI::Shutdown() {
        chif::Backlog::Log("OpenGLRHI", "Shutdown of OpenGL Render Device", chif::Backlog::LogLevel::INFO);
        // TODO: ?
    }

    void OpenGLRHI::BeginFrame() {
        // TODO: Handle with Render Context
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRHI::EndFrame() {
        // TODO: End Command Process and flush commands to GPU
    }
    
    void OpenGLRHI::Present() {
        // TODO: Call Swapchain to change buffers with context and render
    }

}// namespace chif::Graphics::RenderDevice::OpenGL