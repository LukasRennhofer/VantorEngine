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
 * File: vantorRenderDevice.hpp
 * Last Change: 
*/

#pragma once

#include <memory>

#if defined(VANTOR_API_OPENGL)
    #include "DeviceOpenGL/vantorOpenGLRHI.hpp"               // Core RHI
    // #include "DeviceOpenGL/vantorOpenGLCommandBuffer.hpp"     // Command Buffer: TODO: CPP FILE
    #include "DeviceOpenGL/vantorOpenGLShader.hpp"              // Mesh (depends on Material)
    #include "DeviceOpenGL/vantorOpenGLMesh.hpp"              // Mesh (depends on Material)
    #include "DeviceOpenGL/PBR/vantorOpenGLPBR.hpp"           // PBR (depends on Material and Shader)
    #include "DeviceOpenGL/vantorOpenGLChache.hpp"            // Cache (depends on multiple components)
    #include "DeviceOpenGL/vantorOpenGLMaterial.hpp"
    #include "DeviceOpenGL/vantorOpenGLRenderer.hpp"
    #include "DeviceOpenGL/vantorOpenGLRenderTarget.hpp"
    #include "DeviceOpenGL/vantorOpenGLMaterialLibrary.hpp"
    #include "DeviceOpenGL/PBR/vantorOpenGLPBR.hpp"
#endif

namespace vantor::Graphics::RenderDevice {

    // Used for switching between and identifying different APIs
    enum RenderAPI {
        OPENGL,
        // Vulkan, // TODO
        // DirectX, // TODO
        NONE
    };

    inline std::string apiToString(RenderAPI api) {
        switch (api) {
            case RenderAPI::OPENGL: return "OpenGL";
            // case RenderAPI::Vulkan: return "Vulkan"; // TODO
            // case RenderAPI::DirectX: return "DirectX"; // TODO
            case RenderAPI::NONE: default: return "None";
        }
    }

    // ============= OPENGL ==============
    #if defined(VANTOR_API_OPENGL)
    // // Main Functions
    inline RenderAPI getCurrentRenderingAPI() {return RenderAPI::OPENGL;}

    // Main Abstraction Functions
    inline void Init() {vantor::Graphics::RenderDevice::OpenGL::Init();}
    inline void Shutdown() {vantor::Graphics::RenderDevice::OpenGL::Shutdown();}
    inline void BeginFrame() {vantor::Graphics::RenderDevice::OpenGL::BeginFrame();}
    inline void EndFrame() {vantor::Graphics::RenderDevice::OpenGL::EndFrame();}

    // Abstraction Classes
    // MESH
    using Mesh = vantor::Graphics::RenderDevice::OpenGL::Mesh;
    using TOPOLOGY = vantor::Graphics::RenderDevice::OpenGL::TOPOLOGY;
    // Rendering
    using Renderer = vantor::Graphics::RenderDevice::OpenGL::Renderer;
    using RenderTarget = vantor::Graphics::RenderDevice::OpenGL::RenderTarget;
    // Resources
    using Shader = vantor::Graphics::RenderDevice::OpenGL::Shader;
    using Texture = vantor::Graphics::RenderDevice::OpenGL::Texture;
    using Material = vantor::Graphics::RenderDevice::OpenGL::Material;
    using TextureCube = vantor::Graphics::RenderDevice::OpenGL::TextureCube;
    
    #endif


} // namespace vantor::Graphics::RenderDevice


/*
Layout:

    #include <memory>

    namespace vantor::Graphics {

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

    } // namespace vantor::Graphics

*/