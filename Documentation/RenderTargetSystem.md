# Vantor RenderTarget System

The Vantor RenderTarget system provides a modern, flexible interface for off-screen rendering with full integration to the new Texture system. It supports various rendering techniques including HDR rendering, shadow mapping, multiple render targets (MRT), and deferred rendering.

## Overview

The RenderTarget system consists of:
- **Interface Layer**: `VRD_RenderTarget.hpp` - Platform-agnostic render target interface
- **OpenGL Implementation**: `VRDO_RenderTarget.hpp/cpp` - OpenGL-specific implementation
- **Utility Functions**: Helper functions for common render target configurations
- **Factory Functions**: Easy creation of specialized render targets

## Key Features

### ✅ Complete Texture Integration
- Full compatibility with the new VTexture system
- Automatic texture creation and management
- Support for 2D textures with various formats
- Proper memory management and cleanup

### ✅ Multiple Attachment Types
- Color attachments (up to 4 for MRT)
- Depth attachments
- Stencil attachments
- Combined depth-stencil attachments

### ✅ Flexible Configuration
- Configurable texture formats per attachment
- Custom sampler settings per attachment
- Resizable render targets
- Debug naming support

### ✅ Advanced Features
- Framebuffer blitting and copying
- Individual attachment clearing
- Memory usage tracking
- Comprehensive error handling and validation

## Basic Usage

### Simple Color Render Target

```cpp
#include "RenderDevice/OpenGL/VRDO_RenderTarget.hpp"

// Create a simple color render target
auto colorTarget = RenderTargetFactory::CreateColorTarget(
    1920, 1080,                    // Dimensions
    VTextureFormat::RGBA8,         // Color format
    "MainColorBuffer"              // Debug name
);

if (colorTarget && colorTarget->IsValid()) {
    colorTarget->Bind();
    
    // Clear with blue color
    VClearValues clearValues;
    clearValues.ColorValue = {0.0f, 0.0f, 1.0f, 1.0f};
    colorTarget->Clear(clearValues);
    
    // ... render your scene here ...
    
    colorTarget->Unbind();
    
    // Access the color texture
    auto texture = colorTarget->GetColorTexture(0);
    // Use texture for post-processing, UI, etc.
}
```

### Color + Depth Render Target

```cpp
// Create a color + depth render target for 3D scenes
auto sceneTarget = RenderTargetFactory::CreateColorDepthTarget(
    1920, 1080,
    VTextureFormat::RGBA8,      // Color format
    VTextureFormat::Depth24,    // Depth format
    "Scene3D"
);

if (sceneTarget && sceneTarget->IsValid()) {
    sceneTarget->Bind();
    
    // Clear both color and depth
    VClearValues clearValues;
    clearValues.ColorValue = {0.1f, 0.1f, 0.1f, 1.0f}; // Dark gray
    clearValues.DepthValue = 1.0f;                       // Far depth
    sceneTarget->Clear(clearValues);
    
    // ... render 3D scene with depth testing ...
    
    sceneTarget->Unbind();
    
    // Access textures
    auto colorTexture = sceneTarget->GetColorTexture(0);
    auto depthTexture = sceneTarget->GetDepthTexture();
}
```

## Advanced Usage

### HDR Rendering

```cpp
// Create HDR render target
auto hdrTarget = RenderTargetFactory::CreateHDRTarget(
    1920, 1080,
    VTextureFormat::RGBA16F,  // 16-bit floating point HDR
    "HDRBuffer"
);

// Render HDR scene
hdrTarget->Bind();
// ... render with HDR lighting ...
hdrTarget->Unbind();

// Use HDR texture for tone mapping
auto hdrTexture = hdrTarget->GetColorTexture(0);
```

### Shadow Mapping

```cpp
// Create shadow map
auto shadowMap = RenderTargetFactory::CreateShadowMapTarget(
    2048,                     // Shadow map resolution
    VTextureFormat::Depth24,  // Depth format
    "DirectionalShadowMap"
);

// Render shadow map
shadowMap->Bind();
VClearValues clearValues;
clearValues.DepthValue = 1.0f;
shadowMap->ClearAttachment(VAttachmentType::Depth, clearValues);
// ... render scene from light's perspective ...
shadowMap->Unbind();

// Use shadow map in main scene
auto depthTexture = shadowMap->GetDepthTexture();
depthTexture->Bind(1); // Bind to texture unit 1 for shadow sampling
```

### Multiple Render Targets (MRT) - Deferred Rendering

```cpp
// Create G-Buffer for deferred rendering
VRenderTargetDescriptor gBufferDesc = RenderTargetUtils::CreateMRTTarget(
    1920, 1080,
    {
        VTextureFormat::RGBA8,   // Albedo
        VTextureFormat::RGBA8,   // Normal (world space)
        VTextureFormat::RGBA8,   // Material properties (metallic, roughness, AO)
        VTextureFormat::RGBA16F  // World position
    },
    VTextureFormat::Depth24,     // Depth buffer
    "GBuffer"
);

auto gBuffer = std::make_unique<VOpenGLRenderTarget>();
if (gBuffer->Create(gBufferDesc)) {
    // Geometry pass
    gBuffer->Bind();
    gBuffer->Clear();
    // ... render geometry, output to multiple render targets ...
    gBuffer->Unbind();
    
    // Lighting pass - use G-Buffer textures
    auto albedoTexture = gBuffer->GetColorTexture(0);
    auto normalTexture = gBuffer->GetColorTexture(1);
    auto materialTexture = gBuffer->GetColorTexture(2);
    auto positionTexture = gBuffer->GetColorTexture(3);
    
    // Bind all textures for lighting calculations
    albedoTexture->Bind(0);
    normalTexture->Bind(1);
    materialTexture->Bind(2);
    positionTexture->Bind(3);
    
    // ... perform deferred lighting ...
}
```

## Custom Configuration

### Manual Render Target Creation

```cpp
// Create custom render target descriptor
VRenderTargetDescriptor desc;
desc.Width = 1024;
desc.Height = 1024;
desc.DebugName = "CustomTarget";
desc.Resizable = true;

// Add color attachment
VAttachmentDescriptor colorAttachment;
colorAttachment.Type = VAttachmentType::Color0;
colorAttachment.Format = VTextureFormat::RGBA16F;
colorAttachment.GenerateMipmaps = true;

// Configure sampler
colorAttachment.Sampler.minFilter = VTextureFilter::LinearMipmapLinear;
colorAttachment.Sampler.magFilter = VTextureFilter::Linear;
colorAttachment.Sampler.wrapS = VTextureWrap::ClampToEdge;
colorAttachment.Sampler.wrapT = VTextureWrap::ClampToEdge;

desc.Attachments.push_back(colorAttachment);

// Add depth attachment
VAttachmentDescriptor depthAttachment;
depthAttachment.Type = VAttachmentType::Depth;
depthAttachment.Format = VTextureFormat::Depth32F;
depthAttachment.Sampler.minFilter = VTextureFilter::Nearest;
depthAttachment.Sampler.magFilter = VTextureFilter::Nearest;

desc.Attachments.push_back(depthAttachment);

// Create render target
auto customTarget = std::make_unique<VOpenGLRenderTarget>();
if (customTarget->Create(desc)) {
    // Use custom render target
}
```

## Utility Functions

### Framebuffer Operations

```cpp
auto sourceTarget = RenderTargetFactory::CreateColorTarget(1920, 1080);
auto destTarget = RenderTargetFactory::CreateColorTarget(960, 540);

// Blit from source to destination with scaling
sourceTarget->BlitTo(
    destTarget.get(),
    0, 0, 1920, 1080,    // Source region
    0, 0, 960, 540,      // Destination region  
    true                 // Linear filtering
);

// Copy entire target (maintains aspect ratio)
sourceTarget->CopyTo(destTarget.get());
```

### Resizing

```cpp
auto renderTarget = RenderTargetFactory::CreateColorTarget(800, 600);

// Resize if supported
if (renderTarget->IsResizable()) {
    renderTarget->Resize(1920, 1080);
}
```

### Memory Management

```cpp
// Check memory usage
size_t memoryUsage = renderTarget->GetMemoryUsage();

// Get debug information
std::string name = renderTarget->GetDebugName();
uint32_t width = renderTarget->GetWidth();
uint32_t height = renderTarget->GetHeight();
float aspectRatio = renderTarget->GetAspectRatio();
```

## Integration with Game Engine

### Example Renderer Class

```cpp
class GameRenderer {
private:
    std::unique_ptr<VOpenGLRenderTarget> m_SceneTarget;
    std::unique_ptr<VOpenGLRenderTarget> m_ShadowMap;
    std::unique_ptr<VOpenGLRenderTarget> m_PostProcessTarget;
    
public:
    void Initialize(uint32_t width, uint32_t height) {
        m_SceneTarget = RenderTargetFactory::CreateColorDepthTarget(
            width, height, VTextureFormat::RGBA16F, VTextureFormat::Depth24
        );
        
        m_ShadowMap = RenderTargetFactory::CreateShadowMapTarget(2048);
        
        m_PostProcessTarget = RenderTargetFactory::CreateColorTarget(
            width, height, VTextureFormat::RGBA8
        );
    }
    
    void RenderFrame() {
        // 1. Shadow pass
        m_ShadowMap->Bind();
        m_ShadowMap->Clear();
        // ... render shadow casters ...
        m_ShadowMap->Unbind();
        
        // 2. Main scene pass
        m_SceneTarget->Bind();
        m_SceneTarget->Clear();
        
        // Bind shadow map
        auto shadowTexture = m_ShadowMap->GetDepthTexture();
        shadowTexture->Bind(1);
        
        // ... render scene with shadows ...
        m_SceneTarget->Unbind();
        
        // 3. Post-processing
        m_PostProcessTarget->Bind();
        auto sceneTexture = m_SceneTarget->GetColorTexture(0);
        sceneTexture->Bind(0);
        // ... apply post-processing effects ...
        m_PostProcessTarget->Unbind();
        
        // 4. Present to screen
        auto finalTexture = m_PostProcessTarget->GetColorTexture(0);
        // ... present final image ...
    }
};
```

## Error Handling

### Validation

```cpp
// Always validate render targets before use
if (renderTarget && renderTarget->IsValid() && renderTarget->IsComplete()) {
    // Safe to use
    renderTarget->Bind();
    // ... render ...
    renderTarget->Unbind();
} else {
    // Handle error - check logs for details
}
```

### Debug Output

The system provides comprehensive logging through the Vantor Backlog system:
- Creation success/failure messages
- Framebuffer completeness validation
- Memory usage information
- Detailed error descriptions

## Performance Considerations

### Memory Usage
- Use appropriate texture formats for your needs
- Consider using smaller shadow map resolutions for distant lights
- Monitor memory usage with `GetMemoryUsage()`

### Optimization Tips
- Reuse render targets when possible instead of creating new ones
- Use `Resize()` instead of recreating targets for dynamic resolution
- Clear only the attachments you need with `ClearAttachment()`
- Use appropriate filtering modes (Nearest for shadow maps, Linear for color)

## Texture Format Guide

### Color Formats
- `RGBA8`: Standard 8-bit color (most compatible)
- `RGBA16F`: HDR color (floating point)
- `RGBA32F`: High precision HDR
- `RGB8`: 8-bit color without alpha
- `R8`: Single channel (grayscale, masks)

### Depth Formats
- `Depth16`: 16-bit depth (basic)
- `Depth24`: 24-bit depth (standard)
- `Depth32F`: 32-bit floating point depth (high precision)
- `Depth24Stencil8`: Combined depth-stencil

## Limitations

- Maximum of 4 color attachments (OpenGL MRT limit)
- No multisampling support yet (planned for future)
- OpenGL-specific implementation only
- Requires OpenGL 3.3+ for full functionality

## Future Enhancements

- [ ] Multisampling (MSAA) support
- [ ] Texture arrays and 3D textures
- [ ] Vulkan/DirectX backends
- [ ] Compute shader integration
- [ ] Advanced formats (compressed textures)
