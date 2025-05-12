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
 *  Date: 2025-05-12
 *
 *  File: vantorResourceLoader.hpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#pragma once

#include <string>
#include <vector>

#include "../../Graphics/RenderDevice/vantorRenderDevice.hpp"
#include "../Scene/vantorSceneNode.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiString;

namespace vantor
{
    /*

      Mesh loading

    */
    class MeshLoader
    {
        private:
            static std::vector<vantor::Graphics::RenderDevice::OpenGL::Mesh *> meshStore;

        public:
            static void       Clean();
            static SceneNode *LoadMesh(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, std::string path, bool setDefaultMaterial = true);

        private:
            static SceneNode                                    *processNode(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer,
                                                                             aiNode                                           *aNode,
                                                                             const aiScene                                    *aScene,
                                                                             std::string                                       directory,
                                                                             bool                                              setDefaultMaterial);
            static vantor::Graphics::RenderDevice::OpenGL::Mesh *parseMesh(aiMesh *aMesh, const aiScene *aScene, glm::vec3 &out_Min, glm::vec3 &out_Max);
            static vantor::Graphics::RenderDevice::OpenGL::Material *
            parseMaterial(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, aiMaterial *aMaterial, const aiScene *aScene, std::string directory);

            static std::string processPath(aiString *path, std::string directory);
    };

    /*

      Shader loading

    */
    class ShaderLoader
    {
        public:
            static vantor::Graphics::RenderDevice::OpenGL::Shader
            Load(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines = std::vector<std::string>());

        private:
            static std::string readShader(std::ifstream &file, const std::string &name, std::string path);
    };

    /*

      Texture loading

    */
    class TextureLoader
    {
        public:
            static vantor::Graphics::RenderDevice::OpenGL::Texture LoadTexture(std::string path, GLenum target, GLenum internalFormat, bool srgb = false);
            static vantor::Graphics::RenderDevice::OpenGL::Texture LoadHDRTexture(std::string path);
            static vantor::Graphics::RenderDevice::OpenGL::TextureCube
            LoadTextureCube(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back);
            static vantor::Graphics::RenderDevice::OpenGL::TextureCube LoadTextureCube(std::string folder);
    };
} // namespace vantor