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
 * File: vantorOpenGLRHI.hpp 
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

#ifdef __WINDOWS__
    #define APIENTRY __stdcall
#else
    #define APIENTRY
#endif

#pragma once

#include <glad/glad.h>
#include "../../../Core/BackLog/vantorBacklog.h"
#include "vantorOpenGLRenderer.hpp"


namespace vantor::Graphics::RenderDevice::OpenGL {

    Renderer* renderer; // Main Renderer being declared, because of deleting during shutdown
    
    // DEBUG OUTPUT FUNCTION BY @JOEYDEVRIES
    inline void APIENTRY glDebugOutput(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        void *userParam)
    {

        if (
            id == 131169 || id == 131185 || id == 131218 || id == 131204 || id || // driver-specific non-significant error codes
            id == 2000 || id == 2001 || id == 2265 // shader compilation error codes; ignore as already managed from shader object
        )
        {
            return;
        }

        std::string logMessage = "Debug output: (" + std::to_string(id) + "): " + message + "\n";

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:             logMessage += "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   logMessage += "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: logMessage += "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     logMessage += "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     logMessage += "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           logMessage += "Source: Other"; break;
        } logMessage += "\n";

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:               logMessage += "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: logMessage += "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  logMessage += "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         logMessage += "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         logMessage += "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              logMessage += "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          logMessage += "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           logMessage += "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               logMessage += "Type: Other"; break;
        } logMessage += "\n";

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:         logMessage += "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       logMessage += "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          logMessage += "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: logMessage += "Severity: notification"; break;
        } logMessage += "\n";
        logMessage += "\n";

        static unsigned int msgCount = 0;
        if(msgCount++ < 3)
        {
            vantor::Backlog::Log("OpenGLRHIDebug", logMessage, type == GL_DEBUG_TYPE_ERROR ? vantor::Backlog::LogLevel::ERR : vantor::Backlog::LogLevel::WARNING);
        }
    }

    inline Renderer* Init() {
        ::vantor::Backlog::Log("OpenGLRHI", "Init of OpenGL Render Device", ::vantor::Backlog::LogLevel::INFO);
        ::vantor::Backlog::Log("OpenGLRHI", "Version - Major: " + std::to_string(GLVersion.major) + " Minor: " + std::to_string(GLVersion.minor), ::vantor::Backlog::LogLevel::INFO);
        ::vantor::Backlog::Log("OpenGLRHI", "Driver: " + std::string((char*)glGetString(GL_VENDOR)) + " Renderer: " + std::string((char*)glGetString(GL_RENDERER)), ::vantor::Backlog::LogLevel::INFO);

        ::vantor::Backlog::Log("OpenGLRHI", "Initializing debug Output.", ::vantor::Backlog::LogLevel::INFO);
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            // Enabling Debug Output for OpenGL Render Device
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        }
        else
        {
            ::vantor::Backlog::Log("OpenGLRHI", "Debug output not supported.", ::vantor::Backlog::LogLevel::WARNING);
        }
        ::vantor::Backlog::Log("OpenGLRHI", "Debug output initialized.", ::vantor::Backlog::LogLevel::INFO);
        
        renderer = new Renderer();
        renderer->Init();

        return renderer;
    }

    inline void Shutdown() {
        ::vantor::Backlog::Log("OpenGLRHI", "Shutdown of OpenGL Render Device", ::vantor::Backlog::LogLevel::INFO);
        delete renderer;
        // TODO: ?
    }

    inline void BeginFrame() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // TODO: Get ImGui in there?
    }

    inline void EndFrame() {
         // TODO: End Command Process and flush commands to GPU
    }

    inline void Present() {
        // TODO: Call Swapchain to change buffers with context and render
    }
} // namespace vantor::Graphics::RenderDevice::OpenGL