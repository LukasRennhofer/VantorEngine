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
 * File: chifRenderDevice.hpp
 * Last Change: 
*/

#pragma once

#include <memory>

namespace chif::Graphics::RenderDevice {

    // Used for switching between and identifying different APIs
    enum RenderAPI {
        OpenGL,
        // Vulkan, // TODO
        // DirectX, // TODO
        None
    };

    inline std::string apiToString(RenderAPI api) {
        switch (api) {
            case RenderAPI::OpenGL: return "OpenGL";
            // case RenderAPI::Vulkan: return "Vulkan"; // TODO
            // case RenderAPI::DirectX: return "DirectX"; // TODO
            case RenderAPI::None: default: return "None";
        }
    }

    // ============= OPENGL ==============
    #if defined(CHIF_API_OPENGL)
    #include "DeviceOpenGL/chifOpenGLRHI.hpp"               // Core RHI
    // #include "DeviceOpenGL/chifOpenGLCommandBuffer.hpp"     // Command Buffer: TODO: CPP FILE
    #include "DeviceOpenGL/chifOpenGLShader.hpp"              // Mesh (depends on Material)
    #include "DeviceOpenGL/chifOpenGLMesh.hpp"              // Mesh (depends on Material)
    #include "DeviceOpenGL/PBR/chifOpenGLPBR.hpp"           // PBR (depends on Material and Shader)
    #include "DeviceOpenGL/chifOpenGLChache.hpp"            // Cache (depends on multiple components)
    #include "DeviceOpenGL/chifOpenGLMaterial.hpp"

    // // Main Functions
    inline RenderAPI getCurrentRenderingAPI() {return RenderAPI::OpenGL;}

    // Main Abstraction Functions
    inline void Init() {chif::Graphics::RenderDevice::OpenGL::Init();}
    inline void Shutdown() {chif::Graphics::RenderDevice::OpenGL::Shutdown();}
    inline void BeginFrame() {chif::Graphics::RenderDevice::OpenGL::BeginFrame();}
    inline void EndFrame() {chif::Graphics::RenderDevice::OpenGL::EndFrame();}

    // Abstraction Classes
    using Shader = chif::Graphics::RenderDevice::OpenGL::Shader;
    using Mesh = chif::Graphics::RenderDevice::OpenGL::Mesh;
    using Texture = chif::Graphics::RenderDevice::OpenGL::Texture;
    // using Material = chif::Graphics::RenderDevice::OpenGL::Material;

    // RenderDevice Functions etc.

    // ============= VULKAN: TODO (Later on) ==============
    // #elif defined(CHIF_API_VULKAN)
    //     #include "RenderDevice_Vulkan.hpp" // TODO
    //     inline RenderAPI getCurrentRenderingAPI() {return RenderAPI::Vulkan;}
    #endif


} // namespace chif::Graphics::RenderDevice


/*
Layout:

    #include <memory>

    namespace chif::Graphics {

        class RHISystem {
        public:
            static void SetRHI(std::unique_ptr<RHI> newRHI) {
                if (currentRHI) {
                    currentRHI->Shutdown();
                }
                currentRHI = std::move(newRHI);
                currentRHI->Init();
            }

            static RHI* Get() {
                return currentRHI.get();
            }

        private:
            static std::unique_ptr<RHI> currentRHI;
        };

    } // namespace chif::Graphics

*/