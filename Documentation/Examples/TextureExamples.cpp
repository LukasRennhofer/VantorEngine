/*
 *  ╔═══════════════════════════════════════════════════════════════╗
 *  ║                          ~ Vantor ~                           ║
 *  ║                                                               ║
 *  ║  This file is part of the Vantor Engine.                      ║
 *  ║  Automatically formatted by vtrgFormat.py                     ║
 *  ║                                                               ║
 *  ╚═══════════════════════════════════════════════════════════════╝
 *
 *  Copyright (c) 2025 Lukas Rennhofer
 *  Licensed under the GNU General Public License, Version 3.
 *  See LICENSE file for more details.
 *
 *  Author: Lukas Rennhofer
 *  Date: 2025-07-09
 *
 *  File: TextureExamples.cpp
 *  Last Change: Created texture usage examples
 */

#include "Vantor/Source/RenderDevice/OpenGL/VRDO_Texture.hpp"
#include <iostream>

namespace Vantor::Examples
{
    using namespace Vantor::RenderDevice;

    /**
     * @brief Example 1: Loading a texture from a file
     */
    void LoadTextureFromFile()
    {
        std::cout << "[Example 1] Loading texture from file..." << std::endl;

        // Configure sampling parameters
        VTextureSampler sampler;
        sampler.minFilter = VTextureFilter::LinearMipmap;
        sampler.magFilter = VTextureFilter::Linear;
        sampler.wrapS = VTextureWrap::Repeat;
        sampler.wrapT = VTextureWrap::Repeat;
        sampler.generateMipmaps = true;

        // Load texture from file
        auto texture = VOpenGLTexture2D::CreateFromFile("Assets/Textures/example.png", sampler, true);
        
        if (texture) {
            std::cout << "Successfully loaded texture: " 
                      << texture->GetWidth() << "x" << texture->GetHeight() 
                      << " (" << texture->GetMemoryUsage() << " bytes)" << std::endl;
            
            // Bind for rendering
            texture->Bind(0);
            
            // Use texture in shaders...
            // glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);
            
            texture->Unbind(0);
        } else {
            std::cerr << "Failed to load texture!" << std::endl;
        }
    }

    /**
     * @brief Example 2: Creating an empty texture for render targets
     */
    void CreateRenderTargetTexture()
    {
        std::cout << "[Example 2] Creating render target texture..." << std::endl;

        // Configure for render target usage
        VTextureSampler sampler;
        sampler.minFilter = VTextureFilter::Linear;
        sampler.magFilter = VTextureFilter::Linear;
        sampler.wrapS = VTextureWrap::ClampToEdge;
        sampler.wrapT = VTextureWrap::ClampToEdge;
        sampler.generateMipmaps = false; // Render targets typically don't need mipmaps

        // Create empty texture for rendering
        auto colorTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::RGBA8, sampler);
        auto depthTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::Depth24, sampler);

        if (colorTexture && depthTexture) {
            std::cout << "Created render target textures successfully" << std::endl;
            std::cout << "Color texture memory: " << colorTexture->GetMemoryUsage() << " bytes" << std::endl;
            std::cout << "Depth texture memory: " << depthTexture->GetMemoryUsage() << " bytes" << std::endl;

            // These textures can now be attached to framebuffers for off-screen rendering
        }
    }

    /**
     * @brief Example 3: Creating a cubemap for skybox
     */
    void CreateCubemapTexture()
    {
        std::cout << "[Example 3] Creating cubemap texture..." << std::endl;

        // Define the 6 face files (+X, -X, +Y, -Y, +Z, -Z)
        std::string faceFiles[6] = {
            "Assets/Skybox/posx.jpg",  // Right
            "Assets/Skybox/negx.jpg",  // Left
            "Assets/Skybox/posy.jpg",  // Top
            "Assets/Skybox/negy.jpg",  // Bottom
            "Assets/Skybox/posz.jpg",  // Front
            "Assets/Skybox/negz.jpg"   // Back
        };

        VTextureSampler sampler;
        sampler.minFilter = VTextureFilter::Linear;
        sampler.magFilter = VTextureFilter::Linear;
        sampler.wrapS = VTextureWrap::ClampToEdge;
        sampler.wrapT = VTextureWrap::ClampToEdge;
        sampler.wrapR = VTextureWrap::ClampToEdge;
        sampler.generateMipmaps = false; // Cubemaps often don't need mipmaps

        auto cubemap = VOpenGLTextureCube::CreateFromFiles(faceFiles, sampler);

        if (cubemap) {
            std::cout << "Cubemap created successfully: " 
                      << cubemap->GetWidth() << "x" << cubemap->GetHeight() 
                      << " (" << cubemap->GetMemoryUsage() << " bytes)" << std::endl;
            
            // Bind for skybox rendering
            cubemap->Bind(0);
            
            // Use in skybox shader...
            // glUniform1i(glGetUniformLocation(skyboxShader, "uSkybox"), 0);
            
            cubemap->Unbind(0);
        } else {
            std::cerr << "Failed to create cubemap!" << std::endl;
        }
    }

    /**
     * @brief Example 4: Creating a texture from procedural data
     */
    void CreateProceduralTexture()
    {
        std::cout << "[Example 4] Creating procedural texture..." << std::endl;

        const uint32_t width = 256;
        const uint32_t height = 256;
        
        // Generate a simple checkerboard pattern
        std::vector<uint8_t> pixels(width * height * 4); // RGBA
        
        for (uint32_t y = 0; y < height; ++y) {
            for (uint32_t x = 0; x < width; ++x) {
                uint32_t index = (y * width + x) * 4;
                
                // Create checkerboard pattern
                bool checker = ((x / 32) + (y / 32)) % 2 == 0;
                uint8_t color = checker ? 255 : 64;
                
                pixels[index + 0] = color;     // Red
                pixels[index + 1] = color;     // Green
                pixels[index + 2] = color;     // Blue
                pixels[index + 3] = 255;       // Alpha
            }
        }

        VTextureSampler sampler;
        sampler.minFilter = VTextureFilter::Nearest; // Keep sharp edges for checkerboard
        sampler.magFilter = VTextureFilter::Nearest;
        sampler.wrapS = VTextureWrap::Repeat;
        sampler.wrapT = VTextureWrap::Repeat;
        sampler.generateMipmaps = false;

        auto texture = VOpenGLTexture2D::CreateFromMemory(pixels.data(), width, height, 
                                                         VTextureFormat::RGBA8, sampler);

        if (texture) {
            std::cout << "Procedural texture created: " << width << "x" << height << std::endl;
            
            // This texture can now be used for testing, placeholders, or debugging
            texture->Bind(0);
            texture->Unbind(0);
        }
    }

    /**
     * @brief Example 5: HDR texture for advanced rendering
     */
    void CreateHDRTexture()
    {
        std::cout << "[Example 5] Creating HDR texture..." << std::endl;

        VTextureSampler sampler;
        sampler.minFilter = VTextureFilter::Linear;
        sampler.magFilter = VTextureFilter::Linear;
        sampler.wrapS = VTextureWrap::ClampToEdge;
        sampler.wrapT = VTextureWrap::ClampToEdge;
        sampler.generateMipmaps = false;

        // Create HDR render target for tone mapping and post-processing
        auto hdrTexture = VOpenGLTexture2D::CreateEmpty(1920, 1080, VTextureFormat::RGBA16F, sampler);

        if (hdrTexture) {
            std::cout << "HDR texture created: " << hdrTexture->GetWidth() << "x" << hdrTexture->GetHeight() 
                      << " (Format: RGBA16F, Memory: " << hdrTexture->GetMemoryUsage() << " bytes)" << std::endl;
            
            // This texture supports values > 1.0 for HDR rendering
        }
    }

    /**
     * @brief Run all texture examples
     */
    void RunAllExamples()
    {
        std::cout << "=== Vantor Texture System Examples ===" << std::endl;
        
        LoadTextureFromFile();
        std::cout << std::endl;
        
        CreateRenderTargetTexture();
        std::cout << std::endl;
        
        CreateCubemapTexture();
        std::cout << std::endl;
        
        CreateProceduralTexture();
        std::cout << std::endl;
        
        CreateHDRTexture();
        std::cout << std::endl;
        
        std::cout << "=== Examples Complete ===" << std::endl;
    }

} // namespace Vantor::Examples
