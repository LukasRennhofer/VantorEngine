/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <EngineCore/Public/VECO_Engine.hpp>

#include <RHI/Interface/VRHI_Device.hpp>
#include <RHI/VRHI_Coordinator.hpp>

#include <InputDevice/Manager/VID_SubSystem.hpp>

#include <AssetManager/Manager/VAM_AssetManager.hpp>

namespace VE {

    VEngine::VEngine() {
        Initialize();
    }

    VEngine::~VEngine() {
        Shutdown();
    }

    void VEngine::Initialize() {
        // TODO: Make Render API Choice automatically
        m_Device = VE::Internal::RHI::VRDCoordinator::Instance().CreateDevice(VE::Internal::RHI::EGraphicsAPI::OPENGL);
        m_InputManager = std::make_shared<VE::Internal::InputDevice::VInputManager>();
        m_AssetManager = std::make_shared<VE::Internal::AssetManager::VAssetManager>();

        // TODO: Initialize subsystems here
        m_Device->Initialize();
        m_AssetManager->Initialize();
    }

    void VEngine::Update() {
        m_InputManager->Update();
    }

    void VEngine::Shutdown() {
        m_Device->Shutdown();

        m_AssetManager->Shutdown();
    }
}