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
 * File: chifOpenGLTexture.cpp
 * Last Change: 
*/

// #define STB_IMAGE_IMPLEMENTATION // Must be loaded in ResourceManager!!
// #include <stb_image.h>

#include "chifOpenGLTexture.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace chif::Graphics::RenderDevice::OpenGL {
        // --------------------------------------------------------------------------------------------
        Texture::Texture()
        {
            
        }
        // --------------------------------------------------------------------------------------------
        Texture::~Texture()
        {
    
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Generate(unsigned int width, GLenum internalFormat, GLenum format, GLenum type, void* data)
        {
            glGenTextures(1, &ID);
    
            Width          = width;
            Height         = 0;
            Depth          = 0;
            InternalFormat = internalFormat;
            Format         = format;
            Type           = type;
    
            assert(Target == GL_TEXTURE_1D);
            Bind();
                glTexImage1D(Target, 0, internalFormat, width, 0, format, type, data);
                glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, FilterMin);
                glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMax);
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
                if(Mipmapping)
                    glGenerateMipmap(Target);
            Unbind();
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Generate(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, void* data)
        {
            glGenTextures(1, &ID);
    
            Width          = width;
            Height         = height;
            Depth          = 0;
            InternalFormat = internalFormat;
            Format         = format;
            Type           = type;
    
            assert(Target == GL_TEXTURE_2D);
            Bind();
                glTexImage2D(Target, 0, internalFormat, width, height, 0, format, type, data);
                glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, FilterMin);
                glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMax);
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
                glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
                if (Mipmapping)
                    glGenerateMipmap(Target);
            Unbind();
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Generate(unsigned int width, unsigned int height, unsigned int depth, GLenum internalFormat, GLenum format, GLenum type, void* data)
        {
            glGenTextures(1, &ID);
    
            Width          = width;
            Height         = height;
            Depth          = depth;
            InternalFormat = internalFormat;
            Format         = format;
            Type           = type;
    
            assert(Target == GL_TEXTURE_3D);
            Bind();
                glTexImage3D(Target, 0, internalFormat, width, height, depth, 0, format, type, data);
                glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, FilterMin);
                glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, FilterMax);
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, WrapS);
                glTexParameteri(Target, GL_TEXTURE_WRAP_T, WrapT);
                glTexParameteri(Target, GL_TEXTURE_WRAP_R, WrapR);
                if (Mipmapping)
                    glGenerateMipmap(Target);
            Unbind();
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Resize(unsigned int width, unsigned int height, unsigned int depth)
        {
            Bind();
            if (Target == GL_TEXTURE_1D)
            {
                glTexImage1D(GL_TEXTURE_1D, 0, InternalFormat, width, 0, Format, Type, 0);
            }
            else if (Target == GL_TEXTURE_2D)
            {
                assert(height > 0);
                glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, width, height, 0, Format, Type, 0);
            }
            else if (Target == GL_TEXTURE_3D)
            {
                assert(height > 0 && depth > 0);
                glTexImage3D(GL_TEXTURE_3D, 0, InternalFormat, width, height, depth, 0, Format, Type, 0);
            }
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Bind(int unit)
        {
            if(unit >= 0)
                glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(Target, ID);
        }
        // --------------------------------------------------------------------------------------------
        void Texture::Unbind()
        {
            glBindTexture(Target, 0);
        }
        // --------------------------------------------------------------------------------------------
        void Texture::SetWrapMode(GLenum wrapMode, bool bind)
        {
            if(bind)
                Bind();
            if (Target == GL_TEXTURE_1D)
            {
                WrapS = wrapMode;
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, wrapMode);
            }
            else if (Target == GL_TEXTURE_2D)
            {
                WrapS = wrapMode;
                WrapT = wrapMode;
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, wrapMode);
                glTexParameteri(Target, GL_TEXTURE_WRAP_T, wrapMode);
            }
            else if (Target == GL_TEXTURE_3D)
            {
                WrapS = wrapMode;
                WrapT = wrapMode;
                WrapR = wrapMode;
                glTexParameteri(Target, GL_TEXTURE_WRAP_S, wrapMode);
                glTexParameteri(Target, GL_TEXTURE_WRAP_T, wrapMode);
                glTexParameteri(Target, GL_TEXTURE_WRAP_R, wrapMode);
            }
        }
        // --------------------------------------------------------------------------------------------
        void Texture::SetFilterMin(GLenum filter, bool bind)
        {
            if(bind)
                Bind();
            glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, filter);
        }
        // --------------------------------------------------------------------------------------------
        void Texture::SetFilterMax(GLenum filter, bool bind)
        {
            if (bind)
                Bind();
            glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, filter);
        }

        // ======== Texture Cube (originally chifTextureCube.cpp) ========
        TextureCube::TextureCube()
        {
    
        }
        // --------------------------------------------------------------------------------------------
        TextureCube::~TextureCube()
        {
    
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::DefaultInitialize(unsigned int width, unsigned int height, GLenum format, GLenum type, bool mipmap)
        {
            glGenTextures(1, &ID);
    
            FaceWidth = width;
            FaceHeight = height;
            Format = format;
            Type = type;
            Mipmapping = mipmap;
    
            if (type == GL_HALF_FLOAT && format == GL_RGB)
                InternalFormat = GL_RGB16F;
            else if (type == GL_FLOAT && format == GL_RGB)
                InternalFormat = GL_RGB32F;
            else if (type == GL_HALF_FLOAT && format == GL_RGBA)
                InternalFormat = GL_RGBA16F;
            else if (type == GL_FLOAT && format == GL_RGBA)
                InternalFormat = GL_RGBA32F;
    
            Bind();
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, FilterMin);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, FilterMax);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);
    
            for (unsigned int i = 0; i < 6; ++i)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, InternalFormat, width, height, 0, format, type, NULL);
            }
            if (mipmap)
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::GenerateFace(GLenum face, unsigned int width, unsigned int height, GLenum format, GLenum type, unsigned char* data)
        {
            if(FaceWidth == 0)
                glGenTextures(1, &ID);
    
            FaceWidth = width;
            FaceHeight = height;
            Format = format;
            Type = type;
    
            Bind();
    
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, FilterMin);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, FilterMax);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, WrapS);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, WrapT);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, WrapR);
    
            glTexImage2D(face, 0, format, width, height, 0, format, type, data);
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::SetMipFace(GLenum face, unsigned int width, unsigned int height, GLenum format, GLenum type, unsigned int mipLevel, unsigned char *data)
        {
            Bind();
            glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mipLevel, 0, 0, width, height, format, type, data);
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::Resize(unsigned int width, unsigned int height)
        {
            FaceWidth = width;
            FaceHeight = height;
    
            Bind();
            for (unsigned int i = 0; i < 6; ++i)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Format, width, height, 0, Format, Type, NULL);
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::Bind(int unit)
        {
            if (unit >= 0)
                glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        }
        // --------------------------------------------------------------------------------------------
        void TextureCube::Unbind()
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
} // namespace chif::Graphics::RenderDevice::OpenGL
