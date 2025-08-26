/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#pragma once

// =============================================================================
// Core Engine Components
// =============================================================================

// Version and Core Types
#include "../../Source/Vantor/Core/Include/Core/VCO_Version.hpp"
#include "../../Source/Vantor/Core/Include/Core/VCO_Timer.hpp"

// Core Types and Containers
#include "../../Source/Vantor/Core/Include/Core/Container/VCO_SafeString.hpp"
#include "../../Source/Vantor/Core/Include/Core/Container/VCO_Vector.hpp"

// Memory Management
// TODO : #include "../../Source/Vantor/Core/Include/Core/Memory/VCO_Allocator.hpp"

// Backlog
#include "../../Source/Vantor/Core/Include/Core/BackLog/VCO_Backlog.hpp"

// =============================================================================
// Math Library
// =============================================================================
#include "../../Source/Vantor/Math/Include/Math/VMA_Common.hpp"
#include "../../Source/Vantor/Math/Include/Math/Linear/VMA_Vector.hpp"
#include "../../Source/Vantor/Math/Include/Math/Linear/VMA_Matrix.hpp"
#include "../../Source/Vantor/Math/Include/Math/Linear/VMA_Quaternation.hpp"

// =============================================================================
// Render Hardware Interface (RHI)
// =============================================================================
#include "../../Source/Vantor/RHI/Include/RHI/VRHI_Coordinator.hpp"

// RHI Interface
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_Device.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_Buffer.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_Shader.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_Texture.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_Mesh.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_RenderTarget.hpp"
#include "../../Source/Vantor/RHI/Include/RHI/Interface/VRHI_RenderPass.hpp"

// =============================================================================
// Context and Window Management
// =============================================================================
#include "../../Source/Vantor/Context/Include/Context/Interface/VCT_Window.hpp"

// =============================================================================
// Input Device Management
// =============================================================================
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/VID_Factory.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/VID_System.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/VID_MappingContext.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/VID_Common.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/Device/Interface/VID_Device.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/Device/Interface/VID_Gamepad.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/Device/Interface/VID_Keyboard.hpp"
// #include "../../Source/Vantor/InputDevice/Include/InputDevice/Device/Interface/VID_Mouse.hpp"

// =============================================================================
// Actor Runtime System
// =============================================================================
#include "../../Source/Vantor/ActorRuntime/Include/ActorRuntime/VAR_Actor.hpp"
#include "../../Source/Vantor/ActorRuntime/Include/ActorRuntime/VAR_Component.hpp"
#include "../../Source/Vantor/ActorRuntime/Include/ActorRuntime/Register/VAR_Registry.hpp"
#include "../../Source/Vantor/ActorRuntime/Include/ActorRuntime/Components/VAR_Base.hpp"

// =============================================================================
// Graphics System
// =============================================================================
#include "../../Source/Vantor/Graphics/Include/Graphics/VGFX_Material.hpp"
#include "../../Source/Vantor/Graphics/Include/Graphics/VGFX_Uniform.hpp"

// Camera System
#include "../../Source/Vantor/Graphics/Include/Graphics/Camera/VGFX_Camera.hpp"
#include "../../Source/Vantor/Graphics/Include/Graphics/Camera/VGFX_Frustum.hpp"

// Lighting
#include "../../Source/Vantor/Graphics/Include/Graphics/Light/VGFX_LightData.hpp"

// =============================================================================
// Render Modules
// =============================================================================
#include "../../Source/Vantor/RenderModules/Include/RenderModules/Camera/VRM_FlyCamera.hpp"

// =============================================================================
// Integration Layer
// =============================================================================
#include "../../Source/Vantor/Integration/Include/Integration/VIN_ImGui.hpp"

// =============================================================================
// Studio Components (Editor)
// =============================================================================
#ifdef VANTOR_STUDIO_BUILD
    #include "../../Studio/VSTD_StudioManager.hpp"
    #include "../../Studio/Interface/VSTD_Panel.hpp"
    #include "../../Studio/Interface/VSTD_PanelManager.hpp"
    #include "../../Studio/Panels/VSTD_Viewport.hpp"
    #include "../../Studio/Panels/VSTD_Scene.hpp"
    #include "../../Studio/Panels/VSTD_Backlog.hpp"
#endif

// =============================================================================
// Convenience Macros for Common Operations
// =============================================================================

// Quick access to commonly used components
#define VANTOR_VERSION() VE::Internal::Core::Version::GetVersionString()