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
 * File: chifOpenGLTexture.hpp
 * Last Change: 
*/

#pragma once
#include <string>
#include <vector>

namespace chif::Graphics::RenderDevice::OpenGL {

    enum class TextureFormat {
        RGB,
        RGBA,
        SRGB,
        SRGBA,
        DEPTH,
        // TODO
    };

    class OpenGLTexture {
    public:
        static unsigned int Load2D(const std::string& path, bool useSRGB = false);
        static unsigned int LoadCubemap(const std::vector<std::string>& faces);
        static unsigned int Create2D(int width, int height, TextureFormat format = TextureFormat::RGBA);
        static unsigned int Create3D(int width, int height, int depth, TextureFormat format = TextureFormat::RGBA);
        static void Bind2D(unsigned int textureID, int unit = 0);
        static void BindCubemap(unsigned int textureID, int unit = 0);
    };

} // namespace chif::Graphics::RenderDevice::OpenGL