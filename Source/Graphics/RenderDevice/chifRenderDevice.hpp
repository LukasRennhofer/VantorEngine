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

#include <string>
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


    // TODO
    // ============= OPENGL ==============
    // #include "DeviceOpenGL/chifDeviceOpenGL.hpp"

    // // Main Functions
    // inline RenderAPI getCurrentRenderingAPI() {return RenderAPI::OpenGL;}
    // inline void Init() {chif::Graphics::RenderDevice::OpenGL::Init();}
    // inline void Shutdown() {chif::Graphics::RenderDevice::OpenGL::Shutdown();}
    // inline void BeginFrame() {chif::Graphics::RenderDevice::OpenGL::BeginFrame();}
    // inline void EndFrame() {chif::Graphics::RenderDevice::OpenGL::EndFrame();}
    // RenderDevice Functions etc.
    // TODO

    // ============= VULKAN ==============
    // #elif defined(CHIF_API_VULKAN)
    //     #include "RenderDevice_Vulkan.hpp" // TODO
    //     inline RenderAPI getCurrentRenderingAPI() {return RenderAPI::Vulkan;}


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