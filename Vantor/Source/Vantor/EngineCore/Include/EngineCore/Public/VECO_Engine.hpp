/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

#include <memory>

namespace VE::Internal::RHI {
    class IRHIDevice;
}

namespace VE::Internal::InputDevice {
    class VInputManager;
}

namespace VE::Internal::AssetManager {
    class VAssetManager;
}

namespace VE {
    
    class VEngine {
    public:
        VEngine();
        ~VEngine();

        void Initialize();    // sets up subsystems
        void Update();        // Update all Subsystems
        void Shutdown();      // cleans up

        // RenderDevice, that gets created with the Coordinator
        VE::Internal::RHI::IRHIDevice* GetDevice() const { return m_Device.get(); }
        VE::Internal::InputDevice::VInputManager* GetInputMngr() const { return m_InputManager.get(); }
        VE::Internal::AssetManager::VAssetManager* GetAssetMngr() const { return m_AssetManager.get(); }

        // TODO: Get Subsystems
    private:
        std::shared_ptr<Internal::RHI::IRHIDevice> m_Device = nullptr;

        std::shared_ptr<Internal::InputDevice::VInputManager> m_InputManager = nullptr;
        std::shared_ptr<Internal::AssetManager::VAssetManager> m_AssetManager = nullptr;
    };

    inline VEngine* GEngine() {
        static VEngine instance; // constructed on first use
        return &instance;
    }
}