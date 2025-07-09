# Vantor Texture System

The Vantor Texture System provides a comprehensive, modern interface for managing textures in the Vantor Engine. It supports 2D textures, cubemaps, various formats, filtering modes, and seamless integration with the rendering pipeline.

## Features

- **Multiple Texture Types**: 2D textures, cubemaps, with support for 3D textures planned
- **Comprehensive Format Support**: 8-bit, 16-bit float, 32-bit float, HDR, depth, and compressed formats
- **Advanced Sampling**: Multiple filtering modes, wrap modes, mipmapping, and LOD control
- **Memory Management**: RAII-based resource management with automatic cleanup
- **File Loading**: Built-in image loading with STB integration
- **Procedural Creation**: Create textures from memory or generate empty textures
- **OpenGL Backend**: Full OpenGL implementation with error checking and validation
- **Integration**: Seamless integration with RenderTarget system for off-screen rendering

## Basic Usage

### Loading a Texture from File

```cpp
#include "Vantor/Source/RenderDevice/OpenGL/VRDO_Texture.hpp"

// Configure sampling parameters
VTextureSampler sampler;
sampler.minFilter = VTextureFilter::LinearMipmap;
sampler.magFilter = VTextureFilter::Linear;
sampler.wrapS = VTextureWrap::Repeat;
sampler.wrapT = VTextureWrap::Repeat;
sampler.generateMipmaps = true;

// Load texture
auto texture = VOpenGLTexture2D::CreateFromFile("Assets/Textures/diffuse.png", sampler);

if (texture) {
    // Bind for rendering
    texture->Bind(0);
    
    // Use in shader
    // glUniform1i(glGetUniformLocation(program, "uDiffuseTexture"), 0);
    
    texture->Unbind(0);
}
```

### Creating a Render Target Texture

```cpp
// Configure for render target usage
VTextureSampler sampler;
sampler.minFilter = VTextureFilter::Linear;
sampler.magFilter = VTextureFilter::Linear;
sampler.wrapS = VTextureWrap::ClampToEdge;
sampler.wrapT = VTextureWrap::ClampToEdge;
sampler.generateMipmaps = false;

// Create color and depth textures
auto colorTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::RGBA8, sampler);
auto depthTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::Depth24, sampler);

// These can be attached to framebuffers for off-screen rendering
```

### Creating a Cubemap for Skybox

```cpp
std::string faceFiles[6] = {
    "Assets/Skybox/posx.jpg",  // +X (Right)
    "Assets/Skybox/negx.jpg",  // -X (Left)
    "Assets/Skybox/posy.jpg",  // +Y (Top)
    "Assets/Skybox/negy.jpg",  // -Y (Bottom)
    "Assets/Skybox/posz.jpg",  // +Z (Front)
    "Assets/Skybox/negz.jpg"   // -Z (Back)
};

VTextureSampler sampler;
sampler.minFilter = VTextureFilter::Linear;
sampler.magFilter = VTextureFilter::Linear;
sampler.wrapS = VTextureWrap::ClampToEdge;
sampler.wrapT = VTextureWrap::ClampToEdge;
sampler.wrapR = VTextureWrap::ClampToEdge;

auto cubemap = VOpenGLTextureCube::CreateFromFiles(faceFiles, sampler);
```

## Texture Formats

The system supports a wide range of texture formats:

### Color Formats
- `R8`, `RG8`, `RGB8`, `RGBA8` - Standard 8-bit per channel
- `RGB16F`, `RGBA16F` - 16-bit floating point per channel (HDR)
- `RGB32F`, `RGBA32F` - 32-bit floating point per channel (High precision HDR)

### Depth Formats
- `Depth16` - 16-bit depth
- `Depth24` - 24-bit depth
- `Depth32F` - 32-bit floating point depth
- `Depth24Stencil8` - 24-bit depth + 8-bit stencil

### Compressed Formats
- `CompressedRGB` - DXT1/BC1 compression
- `CompressedRGBA` - DXT5/BC3 compression

## Filtering and Sampling

### Filtering Modes
- `Nearest` - Pixelated, no interpolation
- `Linear` - Smooth linear interpolation
- `NearestMipmap` - Nearest with mipmapping
- `LinearMipmap` - Linear with mipmapping
- `MixedMipmap` - Mixed mipmap filtering

### Wrap Modes
- `Repeat` - Tile the texture
- `MirroredRepeat` - Mirror and tile
- `ClampToEdge` - Clamp to edge pixels
- `ClampToBorder` - Clamp to border color

### Sampler Configuration

```cpp
VTextureSampler sampler;
sampler.minFilter = VTextureFilter::LinearMipmap;
sampler.magFilter = VTextureFilter::Linear;
sampler.wrapS = VTextureWrap::Repeat;
sampler.wrapT = VTextureWrap::Repeat;
sampler.borderColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red border
sampler.lodBias = 0.0f;
sampler.minLod = 0.0f;
sampler.maxLod = 1000.0f;
sampler.generateMipmaps = true;
```

## Advanced Features

### HDR Rendering

```cpp
// Create HDR render target
auto hdrTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::RGBA16F, sampler);

// Use for HDR rendering pipeline
// 1. Render scene to HDR texture
// 2. Apply tone mapping
// 3. Output to LDR display
```

### Procedural Textures

```cpp
// Generate procedural data
std::vector<uint8_t> pixels(width * height * 4);
// ... fill pixels with procedural data ...

// Create texture from memory
auto texture = VOpenGLTexture2D::CreateFromMemory(pixels.data(), width, height, 
                                                 VTextureFormat::RGBA8, sampler);
```

### Memory Management

```cpp
// Check memory usage
size_t memoryUsage = texture->GetMemoryUsage();
std::cout << "Texture uses " << memoryUsage << " bytes" << std::endl;

// Textures are automatically destroyed when shared_ptr goes out of scope
// Manual destruction is also possible:
texture->Destroy();
```

## Integration with RenderTargets

The texture system integrates seamlessly with the RenderTarget system:

```cpp
// Create render target with texture attachments
VRenderTargetDescriptor rtDesc;
rtDesc.Width = 1920;
rtDesc.Height = 1080;

VAttachmentDescriptor colorAttachment;
colorAttachment.Type = VAttachmentType::Color0;
colorAttachment.Format = VTextureFormat::RGBA8;
colorAttachment.Sampler.minFilter = VTextureFilter::Linear;
colorAttachment.Sampler.magFilter = VTextureFilter::Linear;

VAttachmentDescriptor depthAttachment;
depthAttachment.Type = VAttachmentType::Depth;
depthAttachment.Format = VTextureFormat::Depth24;

rtDesc.Attachments = {colorAttachment, depthAttachment};

auto renderTarget = CreateRenderTarget(rtDesc);

// Access textures from render target
auto colorTexture = renderTarget->GetColorTexture(0);
auto depthTexture = renderTarget->GetDepthTexture();
```

## Error Handling

The texture system includes comprehensive error checking:

```cpp
auto texture = VOpenGLTexture2D::CreateFromFile("missing_file.png");
if (!texture) {
    std::cerr << "Failed to load texture!" << std::endl;
    // Handle error appropriately
}

// Check if texture is valid before use
if (texture && texture->IsValid()) {
    texture->Bind(0);
    // Safe to use
}
```

## Performance Tips

1. **Use appropriate formats**: Don't use RGBA32F if RGBA8 is sufficient
2. **Generate mipmaps wisely**: Only generate mipmaps for textures that need them
3. **Reuse textures**: Cache loaded textures to avoid redundant loading
4. **Use texture atlases**: Combine small textures into larger atlases to reduce state changes
5. **Monitor memory usage**: Use `GetMemoryUsage()` to track texture memory consumption

## File Format Support

The system uses STB_image for loading, supporting:
- **PNG** - Lossless with alpha support
- **JPEG** - Compressed, good for photos
- **TGA** - Uncompressed with alpha
- **BMP** - Basic bitmap format
- **HDR** - High dynamic range images

## Future Enhancements

- **3D Texture Support**: Volume textures for effects and voxel rendering
- **Texture Arrays**: Efficient storage of multiple related textures
- **Compressed Texture Loading**: Direct loading of DDS/KTX files
- **Async Loading**: Background texture loading for better performance
- **Texture Streaming**: Dynamic loading/unloading based on distance

## Examples

See `Documentation/Examples/TextureExamples.cpp` for comprehensive usage examples including:
- File loading with various formats
- Render target creation
- Cubemap setup
- Procedural texture generation
- HDR texture usage
