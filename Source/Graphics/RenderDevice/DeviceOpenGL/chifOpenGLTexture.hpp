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

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace chif::Graphics::RenderDevice::OpenGL {
    // ============ Base OpenGL Texture ===========
    class Texture
    {
    public:
        unsigned int ID;

        GLenum Target         = GL_TEXTURE_2D;           // what type of texture we're dealing with
        GLenum InternalFormat = GL_RGBA;                 // number of color components
        GLenum Format         = GL_RGBA;                 // the format each texel is stored in
        GLenum Type           = GL_UNSIGNED_BYTE;
        GLenum FilterMin      = GL_LINEAR_MIPMAP_LINEAR; // what filter method to use during minification
        GLenum FilterMax      = GL_LINEAR;               // what filter method to use during magnification
        GLenum WrapS          = GL_REPEAT;               // wrapping method of the S coordinate
        GLenum WrapT          = GL_REPEAT;               // wrapping method of the T coordinate
        GLenum WrapR          = GL_REPEAT;               // wrapping method of the R coordinate
        bool Mipmapping       = true;

        unsigned int Width  = 0;
        unsigned int Height = 0;
        unsigned int Depth  = 0;
    private:
    public:
         Texture();
        ~Texture();

        // 1D texture generation
        void Generate(unsigned int width, GLenum internalFormat, GLenum format, GLenum type, void* data);
        // 2D texture generation
        void Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data);
        // 3D texture generation
        void Generate(unsigned int width, unsigned int height, unsigned int depth, GLenum internalFormat, GLenum format, GLenum type, void* data);

        // resizes the texture; allocates new (empty) texture memory
        void Resize(unsigned int width, unsigned int height = 0, unsigned int depth = 0);

        void Bind(int unit = -1);
        void Unbind();

        // update relevant texture state
        void SetWrapMode(GLenum wrapMode, bool bind = false);
        void SetFilterMin(GLenum filter, bool bind = false);
        void SetFilterMax(GLenum filter, bool bind = false);
    };

    // ======== Texture Cube (originally chifTextureCube.hpp) ========
    class TextureCube
    {
    public:
        unsigned int ID;

        GLenum InternalFormat = GL_RGBA;            // the format each texel is stored in
        GLenum Format         = GL_RGBA;            // the format each texel is stored in
        GLenum Type           = GL_UNSIGNED_BYTE;
        GLenum FilterMin      = GL_LINEAR;          // what filter method to use during minification
        GLenum FilterMax      = GL_LINEAR;          // what filter method to use during magnification
        GLenum WrapS          = GL_CLAMP_TO_EDGE;   // wrapping method of the S coordinate
        GLenum WrapT          = GL_CLAMP_TO_EDGE;   // wrapping method of the T coordinate
        GLenum WrapR          = GL_CLAMP_TO_EDGE;   // wrapping method of the T coordinate
        bool   Mipmapping = false;

        unsigned int FaceWidth  = 0;
        unsigned int FaceHeight = 0;
    private:
    public:
        TextureCube();
        ~TextureCube();

        void DefaultInitialize(unsigned int width, unsigned int height, GLenum format, GLenum type, bool mipmap = false);

        // cubemap texture generation per face
        void GenerateFace(GLenum face, unsigned int width, unsigned int height, GLenum format, GLenum type, unsigned char* data);
        void SetMipFace(GLenum face, unsigned int width, unsigned int height, GLenum format, GLenum type, unsigned int mipLevel, unsigned char* data);

        void Resize(unsigned int width, unsigned int height);

        void Bind(int unit = -1);
        void Unbind();
    };
} // namespace chif::Graphics::RenderDevice::OpenGL