/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vantorFormat.py                   ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-06-08
 *
 *  File: Vantor.h
 *  Last Change: Automatically updated
 */

// This file is intended for managing the Vantor Engine C++ API

/*
Vantors Preprocessor Variables:
     : Main Variables:
          VANTOR_API       = Used for the main header implementation
          VANTOR_STUDIO = Used to identify Studio Builds
          VANTOR_DEV = Used for identifying a debug build (Defined in CMake) (RESERVED, BUT NOT IN USE)
          VANTOR_PROFILE = Used for profiling the whole engine (Defined by user, or buildTool) (RESERVED, BUT NOT IN USE)

     : Platforms :
          __WINDOWS__ = Used for identifying a Window build (Defined in CMake)
          __LINUX__   = Used for identifying a Linux build (Defined in CMake)
          __NSX__  = Used for identifying a Nintendo Switch build (Defined in
CMake)
          __PS5__     = Used for identifying a PS5 build (Defined in CMake)
(RESERVED, BUT NOT IN USE)
          __XBOX__    = Used for identifying a Xbox Series X/S build (Defined in
CMake) (RESERVED, BUT NOT IN USE)

     : Platform API :
          VANTOR_API_COMPILE = Used to identify if the API is able to be
switched at runtime or if it is being chosen at compile time (Defined in CMake)
                             (If Defined: Compile with only one given API, else:
API is switchable based on platform) VANTOR_API_OPENGL  = Used for building with
RenderDevice OpenGL (Defined in CMake) VANTOR_API_VULKAN  = Used for building
with RenderDevice Vulkan (Defined in CMake) (RESERVED, BUT NOT IN USE)
          VANTOR_API_DIRECTX = Used for building with RenderDevice DirectX
(Defined in CMake) (RESERVED, BUT NOT IN USE)

     : Manager API :
          VANTOR_WM_GLFW = Used to identify if GLFW is being used as the WM
          VANTOR_WM_SDL2 = Used to identify if SDL2 is being used as the WM
          VANTOR_WM_EGL  = Used to identify if EGL is being used as the WM
     
     : Integrations :
          VANTOR_INTEGRATION_IMGUI = Used to identify if DEAR IMGUI is an integrated module of the current build (Defined in CMake)
*/

// VANTOR PREPROCESSOR DEFINITION
#ifndef VANTOR_API
#define VANTOR_API

// CORE
#include "../../Source/Core/VCO_Version.hpp"
#include "../../Source/Core/BackLog/VCO_Backlog.hpp"
#include "../../Source/Core/Global/VCO_ServiceRegistry.hpp"
#include "../../Source/Core/Debug/VCO_InlineDebugger.hpp"
#include "../../Source/Core/IO/VCO_FSUtils.hpp"
#include "../../Source/Core/Types/VCO_Color.hpp"
// CORE/CONTAINER
#include "../../Source/Core/Container/VCO_Vector.hpp"
#include "../../Source/Core/Container/VCO_SafeString.hpp"

// APPLICATION
#include "../../Source/Application/VAP_Application.hpp"

// RENDERDEVICE
#include "../../Source/RenderDevice/Interface/VRD_RenderDevice.hpp"
#include "../../Source/RenderDevice/VRD_Coordinator.hpp"

// RENDERER
#include "../../Source/Renderer/VRE_Material.hpp"
#include "../../Source/Renderer/VRE_Uniform.hpp"
#include "../../Source/Renderer/VRE_MaterialLibrary.hpp"
#include "../../Source/Renderer/Light/VRE_LightData.hpp"
// RENDERER/CAMERA
#include "../../Source/Renderer/Camera/VRE_Camera.hpp"
#include "../../Source/Renderer/Camera/VRE_Frustum.hpp"

// CONTEXT
#include "../../Source/Context/Interface/VCT_Window.hpp"

// Platform
#include "../../Source/Platform/VPL_Environment.hpp"

// Resource Manager
#include "../../Source/Resource/VRES_Manager.hpp"

// INPUT
#include "../../Source/InputDevice/VID_IDevice.hpp"
#include "../../Source/InputDevice/VID_Factory.hpp"
#include "../../Source/InputDevice/VID_Manager.hpp"

// OBJECT SYSTEM
#include "../../Source/ObjectSystem/VOS_Object.hpp"
#include "../../Source/ObjectSystem/Component/VOS_Component.hpp"
#include "../../Source/ObjectSystem/Register/VOS_Registry.hpp"
#include "../../Source/ObjectSystem/Component/VOS_Base.hpp"  // Basic Components

// External API
//  ===== Objects =====
// Application
using VApplication = Vantor::VApplication;
using VApplicationCreateInfo = Vantor::VApplicationCreateInfo;
// Core
using VSafeString = Vantor::Core::SafeString<>;
template<typename T>
using VVector = Vantor::Core::Vector<T>;
using VTimer = Vantor::Core::Timer;
using VTimeStamp = Vantor::Core::VTimeStamp;
// TODO: Version Functions
using VColor = Vantor::Core::Types::VColor;
// TODO: Vantor IO Functions
// Backlog
using VBacklogEntry = Vantor::Backlog::LogEntry;
using VBacklogLevel = Vantor::Backlog::LogLevel;
// Context
using VWindow = Vantor::Context::Window;
// Debug : TODO
// Input
using VInputDevice =  Vantor::Input::VIInputDevice;
using VInputBackend = Vantor::Input::VEInputBackend;
using VInputDeviceType = Vantor::Input::VEInputDeviceType;
using VInputKey = Vantor::Input::VEInputKey;
using VInputButton = Vantor::Input::VInputButton;
using VInputManager = Vantor::Input::VInputManager;
// Object System
using VORegistry = Vantor::Object::VORegistry;
using VObject = Vantor::Object::VObject;
using VObjectID = Vantor::Object::VObjectID;
using VComponent = Vantor::Object::VComponent;
// Object System / Components
using VMaterialComponent = Vantor::Object::VMaterialComponent;
using VMeshComponent = Vantor::Object::VMeshComponent;
using VTransformComponent = Vantor::Object::VTransformComponent;
using VTagComponent = Vantor::Object::VTagComponent;
// TODO: Component and Object Handles
// Platform : TODO
// RenderDevice
using VRenderCoordinator = Vantor::RenderDevice::VRenderCoordinator;
using VRDevice = Vantor::RenderDevice::VRDevice;
using VRenderCommand = Vantor::RenderDevice::VRenderCommand;
using VRenderPass = Vantor::RenderDevice::VRenderPass;
using VRenderPath3D = Vantor::RenderDevice::VRenderPath3D;
using VShaderStorageType = Vantor::RenderDevice::VEStorageType;
using VRenderStats = Vantor::RenderDevice::VERenderStats; // TODO: With Profiler
using VRenderPassType = Vantor::RenderDevice::VERenderPassType;
using VRenderTopology = Vantor::RenderDevice::VE_RENDER_TOPOLOGY;
using VRenderAPI = Vantor::RenderDevice::VERenderAPI;
// RenderDevice / Main Types
using VRenderTarget = Vantor::RenderDevice::VRenderTarget;
using VShader = Vantor::RenderDevice::VShader;
using VTexture = Vantor::RenderDevice::VTexture;
using VTexture2D = Vantor::RenderDevice::VTexture2D;
using VTextureCube = Vantor::RenderDevice::VTextureCube;
using VTDataType = Vantor::RenderDevice::VTextureDataType;
using VTextureFilter = Vantor::RenderDevice::VTextureFilter;
using VTextureFormat = Vantor::RenderDevice::VTextureFormat;
using VTextureSampler = Vantor::RenderDevice::VTextureSampler;
using VTextureSpec = Vantor::RenderDevice::VTextureSpec;
using VTextureUsage = Vantor::RenderDevice::VTextureUsage;
using VTextureWrap = Vantor::RenderDevice::VTextureWrap;
using VMesh = Vantor::RenderDevice::VMesh;
using VMeshCreateInfo = Vantor::RenderDevice::VMeshCreateInfo;
// Renderer
using VCamera = Vantor::Renderer::Camera;
using VCameraFrustum = Vantor::Renderer::CameraFrustum;
using VCameraMovement = Vantor::Renderer::VECameraMovement;
using VMaterialType = Vantor::Renderer::VEMaterialType;
using VMaterial = Vantor::Renderer::VMaterial;
using VMaterialLibrary = Vantor::Renderer::VMaterialLibrary;
using VPointLightData = Vantor::Renderer::VPointLightData;
using VUniform = Vantor::Renderer::VUniform;
using VUniformValue = Vantor::Renderer::VUniformValue;
using VUniformSampler = Vantor::Renderer::VUniformValueSampler;
using VUniformType = Vantor::Renderer::VEUniformType;
using VSamplerType = Vantor::Renderer::VESamplerType;
using VVertexAttribute = Vantor::Renderer::VVertexAttribute;
// Resource
using VResourcetype = Vantor::Resource::VEResourceType;
using VResource = Vantor::Resource::VResource;
using VResourceHandle = Vantor::Resource::VResourceHandle;
using VResourceManager = Vantor::Resource::VResourceManager;
using VShaderProgramResource = Vantor::Resource::VShaderProgramResource;
using VTexture2DResource = Vantor::Resource::VTexture2DResource;

// ===== C++ API Functions ======

// ===== Core Functions =====
// Version
std::string vGetVersion() { return Vantor::Core::Version::GetVersionString(); }

// Core / IO
bool vFileExists(const std::string &path) { return Vantor::Core::IO::FileExists(path); }

// Backlog/Logging
void vBacklog(const std::string &source, const std::string &msg, VBacklogLevel level = VBacklogLevel::INFO) { 
    return Vantor::Backlog::Log(source, msg, level); 
}
void vLog(const std::string &source, const std::string &msg) { 
    return Vantor::Backlog::Log(source, msg, VBacklogLevel::INFO); 
}
void vLogInfo(const std::string &source, const std::string &msg) { 
    return Vantor::Backlog::Log(source, msg, VBacklogLevel::INFO); 
}
void vLogError(const std::string &source, const std::string &msg) { 
    return Vantor::Backlog::Log(source, msg, VBacklogLevel::ERR); 
}

// ===== Application Functions =====
VApplication* vCreateApplication() { return new VApplication(); }

// ===== RenderDevice Functions =====
VRenderCoordinator& vGetRenderCoordinator() { return VRenderCoordinator::Instance(); }
std::shared_ptr<VRDevice> vCreateRenderDevice(VRenderAPI backend) { 
    return VRenderCoordinator::Instance().CreateRenderDevice(backend); 
}
void vRenderDeviceInitialize(VRDevice* device) { device->Initialize(); }
void vRenderDeviceBeginFrame(VRDevice* device) { device->BeginFrame(); }
void vRenderDeviceEndFrame(VRDevice* device) { device->EndFrame(); }
void vRenderDeviceSetViewPort(VRDevice* device, int width, int height) { device->SetViewPort(width, height); }
void vRenderDeviceCreateContext(VRDevice* device, VWindow* window) { device->CreateRenderDeviceContext(window); }
std::shared_ptr<VRenderPath3D> vRenderDeviceCreateRenderPath3D(VRDevice* device) { return device->CreateRenderPath3D(); }
// TODO: Make better functions
std::shared_ptr<VMesh> vRenderDeviceCreateMesh(VRDevice* device, const VMeshCreateInfo& createInfo) { 
    return device->CreateMesh(createInfo); 
}

// RenderCoordinator texture/shader creation helpers
// std::shared_ptr<VTexture> vCreateTexture2DInstance(const std::string& filePath, const VTextureSampler& sampler, bool generateMipmaps = true) {
//     return VRenderCoordinator::Instance().CreateTexture2DInstance(filePath, sampler, generateMipmaps);
// }
// std::shared_ptr<VShader> vCreateShaderInstance(const char* vertexCode, const char* fragmentCode, 
//     const char* fileNameVertex, const char* fileNameFragment) {
//     return VRenderCoordinator::Instance().CreateShaderInstance(vertexCode, fragmentCode, fileNameVertex, fileNameFragment);
// }

// ===== Resource Manager Functions =====
VResourceManager& vGetResourceManager() { return VResourceManager::Instance(); }
bool vResourceManagerInitialize() { return VResourceManager::Instance().Initialize(); }
void vResourceManagerShutdown() { VResourceManager::Instance().Shutdown(); }
bool vResourceManagerIsInitialized() { return VResourceManager::Instance().IsInitialized(); }

// Texture Loading
std::shared_ptr<VTexture2DResource> vLoadTexture2D(const std::string& handle, const std::string& filePath, 
    const VTextureSampler& sampler = {}, bool generateMipmaps = true) {
    return VResourceManager::Instance().LoadTexture2D(handle, filePath, sampler, generateMipmaps);
}
void vPreloadTexture2D(const std::string& handle, const std::string& filePath, 
    const VTextureSampler& sampler = {}, bool generateMipmaps = true) {
    VResourceManager::Instance().PreloadTexture2D(handle, filePath, sampler, generateMipmaps);
}

// Shader Loading
std::shared_ptr<VShaderProgramResource> vLoadShaderProgram(const std::string& handle, 
    const std::string& vertexPath, const std::string& fragmentPath) {
    return VResourceManager::Instance().LoadShaderProgram(handle, vertexPath, fragmentPath);
}
void vPreloadShaderProgram(const std::string& handle, const std::string& vertexPath, const std::string& fragmentPath) {
    VResourceManager::Instance().PreloadShaderProgram(handle, vertexPath, fragmentPath);
}

// Resource Access
std::shared_ptr<VResource> vGetResource(const std::string& handle) {
    return VResourceManager::Instance().GetResource(handle);
}
std::shared_ptr<VTexture2DResource> vGetTexture2D(const std::string& handle) {
    return VResourceManager::Instance().GetTexture2D(handle);
}
std::shared_ptr<VShaderProgramResource> vGetShaderProgram(const std::string& handle) {
    return VResourceManager::Instance().GetShaderProgram(handle);
}
std::shared_ptr<VTexture> vGetTexture2DData(const std::string& handle) {
    return VResourceManager::Instance().GetTexture2DData(handle);
}
std::shared_ptr<VShader> vGetShaderProgramData(const std::string& handle) {
    return VResourceManager::Instance().GetShaderProgramData(handle);
}

// Resource Management
bool vUnloadResource(const std::string& handle) {
    return VResourceManager::Instance().UnloadResource(handle);
}
void vUnloadAllResources() {
    VResourceManager::Instance().UnloadAllResources();
}
bool vReloadResource(const std::string& handle) {
    return VResourceManager::Instance().ReloadResource(handle);
}
void vPreloadInternalResources() {
    VResourceManager::Instance().PreloadInternalResources();
}

// Resource Statistics
size_t vGetResourceCount() {
    return VResourceManager::Instance().GetResourceCount();
}
size_t vGetTotalMemoryUsage() {
    return VResourceManager::Instance().GetTotalMemoryUsage();
}
std::vector<std::string> vGetResourceHandles() {
    return VResourceManager::Instance().GetResourceHandles();
}

// ===== Object System Functions =====
// VORegistry Helper functions
template <typename T, typename... Args>
std::shared_ptr<T> vCreateActor(Args&&... args) { 
    return Vantor::Object::VORegistry::CreateEntity<T>(std::forward<Args>(args)...); 
}
std::shared_ptr<VObject> vGetActor(VObjectID id) { 
    return Vantor::Object::VORegistry::GetEntity(id); 
}
void vDestroyActor(VObjectID id) { 
    Vantor::Object::VORegistry::DestroyEntity(id); 
}
std::vector<std::shared_ptr<VObject>> vGetAllActors() { 
    return Vantor::Object::VORegistry::GetAllEntitiesList(); 
}

// ===== Material System Functions =====
VMaterialLibrary& vGetMaterialLibrary() { return VMaterialLibrary::Instance(); }
void vMaterialLibraryInitialize() { VMaterialLibrary::Instance().Initialize(); }
// std::shared_ptr<VMaterial> vCreateMaterial(VShader* shader) { 
//     return std::make_shared<VMaterial>(type); 
// }

// ===== Renderer Functions =====
std::string vGetRenderAPIString(VRenderAPI api) { 
    return Vantor::RenderDevice::GetRenderAPIToString(api); 
}

#endif // VANTOR_API