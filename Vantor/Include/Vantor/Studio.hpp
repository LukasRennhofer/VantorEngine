// This file is used for the Studio Mode Integration of Vantor for Applications

// Studio builds are being identified with the preproccessor VANTOR_STUDIO

#pragma once

#include "Vantor.hpp"

// Forward declarations for studio functions
void vStudioInitialize();
void vStudioRender(VRenderTarget* viewportBuffer = nullptr);
void vStudioShutdown();

#ifdef VANTOR_STUDIO

#include "../../Studio/VSTD_StudioManager.hpp"

// Studio API Implementation
namespace Vantor::Studio::API {
    // Initialize the studio system
    inline void Initialize() {
        VStudioManager::Instance().Initialize();
    }

    // Render the studio interface
    inline void Render(VRenderTarget* viewportBuffer = nullptr) {
        VStudioManager::Instance().RenderInterface(viewportBuffer);
    }

    // Shutdown the studio system
    inline void Shutdown() {
        VStudioManager::Instance().Shutdown();
    }

    // Get studio manager instance
    inline VStudioManager& GetStudioManager() {
        return VStudioManager::Instance();
    }
}

// C-style API
inline void vStudioInitialize() {
    Vantor::Studio::API::Initialize();
}

inline void vStudioRender(VRenderTarget* viewportBuffer) {
    Vantor::Studio::API::Render(viewportBuffer);
}

inline void vStudioShutdown() {
    Vantor::Studio::API::Shutdown();
}

// Additional studio utilities
inline Vantor::Studio::VStudioManager& vGetStudioManager() {
    return Vantor::Studio::API::GetStudioManager();
}

#else

inline void vStudioInitialize() {}

inline void vStudioRender(VRenderTarget* viewportBuffer = nullptr) {}

inline void vStudioShutdown() {}

#endif // VANTOR_STUDIO
