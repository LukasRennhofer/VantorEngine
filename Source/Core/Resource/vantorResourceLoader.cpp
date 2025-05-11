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
 *  Date: 2025-05-11
 *
 *  File: vantorResourceLoader.cpp
 *  Last Change: Automatically updated
 */

// !!! Warning: Needs to be changed, when implementing Vulkan, because of GLEnum
// and other OpenGL usage

#include <stb_image.h>

#include "vantorResourceLoader.hpp"
#include "vantorResource.hpp"
#include "../Scene/vantorSceneNode.hpp"
#include "../BackLog/vantorBacklog.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace vantor
{
    /*

      Mesh loading

    */
    std::vector<vantor::Graphics::RenderDevice::OpenGL::Mesh *> MeshLoader::meshStore = std::vector<vantor::Graphics::RenderDevice::OpenGL::Mesh *>();
    // --------------------------------------------------------------------------------------------
    void MeshLoader::Clean()
    {
        for (unsigned int i = 0; i < MeshLoader::meshStore.size(); ++i)
        {
            delete MeshLoader::meshStore[i];
        }
    }
    // --------------------------------------------------------------------------------------------
    vantor::SceneNode *MeshLoader::LoadMesh(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, std::string path, bool setDefaultMaterial)
    {
        vantor::Backlog::Log("ResourceLoader", "Loading mesh file at: " + path + ".", vantor::Backlog::LogLevel::INFO);

        Assimp::Importer importer;
        const aiScene   *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            vantor::Backlog::Log("ResourceLoader", "Assimp failed to load model at path: " + path, vantor::Backlog::LogLevel::ERR);
            return nullptr;
        }

        std::string directory = path.substr(0, path.find_last_of("/"));

        vantor::Backlog::Log("ResourceLoader", "Succesfully loaded: " + path + ".", vantor::Backlog::LogLevel::INFO);

        return MeshLoader::processNode(renderer, scene->mRootNode, scene, directory, setDefaultMaterial);
    }
    // --------------------------------------------------------------------------------------------
    SceneNode *MeshLoader::processNode(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer,
                                       aiNode                                           *aNode,
                                       const aiScene                                    *aScene,
                                       std::string                                       directory,
                                       bool                                              setDefaultMaterial)
    {
        SceneNode *node = new SceneNode(0);

        for (unsigned int i = 0; i < aNode->mNumMeshes; ++i)
        {
            glm::vec3                                         boxMin, boxMax;
            aiMesh                                           *assimpMesh = aScene->mMeshes[aNode->mMeshes[i]];
            aiMaterial                                       *assimpMat  = aScene->mMaterials[assimpMesh->mMaterialIndex];
            vantor::Graphics::RenderDevice::OpenGL::Mesh     *mesh       = MeshLoader::parseMesh(assimpMesh, aScene, boxMin, boxMax);
            vantor::Graphics::RenderDevice::OpenGL::Material *material   = nullptr;
            if (setDefaultMaterial)
            {
                material = MeshLoader::parseMaterial(renderer, assimpMat, aScene, directory);
            }

            if (aNode->mNumMeshes == 1)
            {
                node->Mesh = mesh;
                if (setDefaultMaterial)
                {
                    node->Material = material;
                }
                node->BoxMin = boxMin;
                node->BoxMax = boxMax;
            }

            else
            {
                SceneNode *child = new SceneNode(0);
                child->Mesh      = mesh;
                child->Material  = material;
                child->BoxMin    = boxMin;
                child->BoxMax    = boxMax;
                node->AddChild(child);
            }
        }

        for (unsigned int i = 0; i < aNode->mNumChildren; ++i)
        {
            node->AddChild(MeshLoader::processNode(renderer, aNode->mChildren[i], aScene, directory, setDefaultMaterial));
        }

        return node;
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Mesh *MeshLoader::parseMesh(aiMesh *aMesh, const aiScene *aScene, glm::vec3 &out_Min, glm::vec3 &out_Max)
    {
        std::vector<glm::vec3>    positions;
        std::vector<glm::vec2>    uv;
        std::vector<glm::vec3>    normals;
        std::vector<glm::vec3>    tangents;
        std::vector<glm::vec3>    bitangents;
        std::vector<unsigned int> indices;

        positions.resize(aMesh->mNumVertices);
        normals.resize(aMesh->mNumVertices);
        if (aMesh->mNumUVComponents[0] > 0) // MAYBE CAUSING ERRORS!!!
        {
            uv.resize(aMesh->mNumVertices);
            tangents.resize(aMesh->mNumVertices);
            bitangents.resize(aMesh->mNumVertices);
        }
        indices.resize(aMesh->mNumFaces * 3);

        glm::vec3 pMin(99999.0);
        glm::vec3 pMax(-99999.0);

        for (unsigned int i = 0; i < aMesh->mNumVertices; ++i)
        {
            positions[i] = glm::vec3(aMesh->mVertices[i].x, aMesh->mVertices[i].y, aMesh->mVertices[i].z);
            normals[i]   = glm::vec3(aMesh->mNormals[i].x, aMesh->mNormals[i].y, aMesh->mNormals[i].z);
            if (aMesh->mTextureCoords[0])
            {
                uv[i] = glm::vec2(aMesh->mTextureCoords[0][i].x, aMesh->mTextureCoords[0][i].y);
            }
            if (aMesh->mTangents)
            {
                tangents[i]   = glm::vec3(aMesh->mTangents[i].x, aMesh->mTangents[i].y, aMesh->mTangents[i].z);
                bitangents[i] = glm::vec3(aMesh->mBitangents[i].x, aMesh->mBitangents[i].y, aMesh->mBitangents[i].z);
            }
            if (positions[i].x < pMin.x) pMin.x = positions[i].x;
            if (positions[i].y < pMin.y) pMin.y = positions[i].y;
            if (positions[i].z < pMin.z) pMin.z = positions[i].z;
            if (positions[i].x > pMax.x) pMax.x = positions[i].x;
            if (positions[i].y > pMax.y) pMax.y = positions[i].y;
            if (positions[i].z > pMax.z) pMax.z = positions[i].z;
        }
        for (unsigned int f = 0; f < aMesh->mNumFaces; ++f)
        {
            for (unsigned int i = 0; i < 3; ++i)
            {
                indices[f * 3 + i] = aMesh->mFaces[f].mIndices[i];
            }
        }

        vantor::Graphics::RenderDevice::OpenGL::Mesh *mesh = new vantor::Graphics::RenderDevice::OpenGL::Mesh;
        mesh->Positions                                    = positions;
        mesh->UV                                           = uv;
        mesh->Normals                                      = normals;
        mesh->Tangents                                     = tangents;
        mesh->Bitangents                                   = bitangents;
        mesh->Indices                                      = indices;
        mesh->Topology                                     = vantor::Graphics::RenderDevice::OpenGL::TRIANGLES;
        mesh->Finalize(true);

        out_Min.x = pMin.x;
        out_Min.y = pMin.y;
        out_Min.z = pMin.z;
        out_Max.x = pMax.x;
        out_Max.y = pMax.y;
        out_Max.z = pMax.z;

        MeshLoader::meshStore.push_back(mesh);

        return mesh;
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Material *
    MeshLoader::parseMaterial(vantor::Graphics::RenderDevice::OpenGL::Renderer *renderer, aiMaterial *aMaterial, const aiScene *aScene, std::string directory)
    {
        vantor::Graphics::RenderDevice::OpenGL::Material *material;

        aiString file;
        aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        std::string diffPath = std::string(file.C_Str());
        bool        alpha    = false;
        if (diffPath.find("_alpha") != std::string::npos)
        {
            material = renderer->CreateMaterial("alpha discard");
            alpha    = true;
        }
        else
        {
            material = renderer->CreateMaterial();
        }

        /* TEXTURE TYPES:

            - aiTextureType_DIFFUSE:   Albedo
            - aiTextureType_NORMALS:   Normal
            - aiTextureType_SPECULAR:  metallic
            - aiTextureType_SHININESS: roughness
            - aiTextureType_AMBIENT:   AO (ambient occlusion)
            - aiTextureType_EMISSIVE:  Emissive

        */
        if (aMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString file;
            aMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
            std::string                                      fileName = MeshLoader::processPath(&file, directory);
            vantor::Graphics::RenderDevice::OpenGL::Texture *texture
                = Resources::LoadTexture(fileName, fileName, GL_TEXTURE_2D, alpha ? GL_RGBA : GL_RGB, true);
            if (texture)
            {
                material->SetTexture("TexAlbedo", texture, 3);
            }
        }
        if (aMaterial->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
        {
            aiString file;
            aMaterial->GetTexture(aiTextureType_DISPLACEMENT, 0, &file);
            std::string fileName = MeshLoader::processPath(&file, directory);

            vantor::Graphics::RenderDevice::OpenGL::Texture *texture = Resources::LoadTexture(fileName, fileName);
            if (texture)
            {
                material->SetTexture("TexNormal", texture, 4);
            }
        }
        if (aMaterial->GetTextureCount(aiTextureType_SPECULAR) > 0)
        {
            aiString file;
            aMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
            std::string fileName = MeshLoader::processPath(&file, directory);

            vantor::Graphics::RenderDevice::OpenGL::Texture *texture = Resources::LoadTexture(fileName, fileName);
            if (texture)
            {
                material->SetTexture("TexMetallic", texture, 5);
            }
        }
        if (aMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
        {
            aiString file;
            aMaterial->GetTexture(aiTextureType_SHININESS, 0, &file);
            std::string fileName = MeshLoader::processPath(&file, directory);

            vantor::Graphics::RenderDevice::OpenGL::Texture *texture = Resources::LoadTexture(fileName, fileName);
            if (texture)
            {
                material->SetTexture("TexRoughness", texture, 6);
            }
        }
        if (aMaterial->GetTextureCount(aiTextureType_AMBIENT) > 0)
        {
            aiString file;
            aMaterial->GetTexture(aiTextureType_AMBIENT, 0, &file);
            std::string fileName = MeshLoader::processPath(&file, directory);

            vantor::Graphics::RenderDevice::OpenGL::Texture *texture = Resources::LoadTexture(fileName, fileName);
            if (texture)
            {
                material->SetTexture("TexAO", texture, 7);
            }
        }

        return material;
    }
    // --------------------------------------------------------------------------------------------
    std::string MeshLoader::processPath(aiString *aPath, std::string directory)
    {
        std::string path = std::string(aPath->C_Str());
        if (path.find(":/") == std::string::npos || path.find(":\\") == std::string::npos) path = directory + "/" + path;
        return path;
    }

    /*

      Shader loading

    */
    vantor::Graphics::RenderDevice::OpenGL::Shader
    ShaderLoader::Load(std::string name, std::string vsPath, std::string fsPath, std::vector<std::string> defines)
    {
        std::ifstream vsFile, fsFile;
        vsFile.open(vsPath);
        fsFile.open(fsPath);

        if (!vsFile.is_open() || !fsFile.is_open())
        {
            vantor::Backlog::Log("ResourceLoader", "Shader failed to load at path: " + vsPath + " and " + fsPath, vantor::Backlog::LogLevel::ERR);
            return vantor::Graphics::RenderDevice::OpenGL::Shader();
        }

        std::string vsDirectory = vsPath.substr(0, vsPath.find_last_of("/\\"));
        std::string fsDirectory = fsPath.substr(0, fsPath.find_last_of("/\\"));
        std::string vsSource    = readShader(vsFile, name, vsPath);
        std::string fsSource    = readShader(fsFile, name, fsPath);

        vantor::Graphics::RenderDevice::OpenGL::Shader shader(name, vsSource, fsSource, defines);

        vsFile.close();
        fsFile.close();

        return shader;
    }
    // --------------------------------------------------------------------------------------------
    std::string ShaderLoader::readShader(std::ifstream &file, const std::string &name, std::string path)
    {
        std::string directory = path.substr(0, path.find_last_of("/\\"));
        std::string source, line;
        while (std::getline(file, line))
        {
            if (line.substr(0, 8) == "#include")
            {
                std::string   includePath = directory + "/" + line.substr(9);
                std::ifstream includeFile(includePath);
                if (includeFile.is_open())
                {
                    source += readShader(includeFile, name, includePath);
                }
                else
                {
                    vantor::Backlog::Log("ResourceLoader", "Shader: " + name + ": include: " + includePath + " failed to open.",
                                         vantor::Backlog::LogLevel::ERR);
                }
                includeFile.close();
            }
            else
                source += line + "\n";
        }
        return source;
    }

    /*

      Texture loading

    */
    vantor::Graphics::RenderDevice::OpenGL::Texture TextureLoader::LoadTexture(std::string path, GLenum target, GLenum internalFormat, bool srgb)
    {
        vantor::Graphics::RenderDevice::OpenGL::Texture texture;
        texture.Target         = target;
        texture.InternalFormat = internalFormat;
        if (texture.InternalFormat == GL_RGB || texture.InternalFormat == GL_SRGB) texture.InternalFormat = srgb ? GL_SRGB : GL_RGB;
        if (texture.InternalFormat == GL_RGBA || texture.InternalFormat == GL_SRGB_ALPHA) texture.InternalFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;

        stbi_set_flip_vertically_on_load(true);

        int            width, height, nrComponents;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            if (target == GL_TEXTURE_1D)
                texture.Generate(width, texture.InternalFormat, format, GL_UNSIGNED_BYTE, data);
            else if (target == GL_TEXTURE_2D)
                texture.Generate(width, height, texture.InternalFormat, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader", "Texture failed to load at path: " + path, vantor::Backlog::LogLevel::ERR);
            stbi_image_free(data);
            return texture;
        }
        texture.Width  = width;
        texture.Height = height;

        return texture;
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::Texture TextureLoader::LoadHDRTexture(std::string path)
    {
        vantor::Graphics::RenderDevice::OpenGL::Texture texture;
        texture.Target     = GL_TEXTURE_2D;
        texture.FilterMin  = GL_LINEAR;
        texture.Mipmapping = false;

        stbi_set_flip_vertically_on_load(true);

        if (stbi_is_hdr(path.c_str()))
        {
            int    width, height, nrComponents;
            float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum internalFormat, format;
                if (nrComponents == 3)
                {
                    internalFormat = GL_RGB32F;
                    format         = GL_RGB;
                }
                else if (nrComponents == 4)
                {
                    internalFormat = GL_RGBA32F;
                    format         = GL_RGBA;
                }

                texture.Generate(width, height, internalFormat, format, GL_FLOAT, data);
                stbi_image_free(data);
            }
            texture.Width  = width;
            texture.Height = height;
        }
        else
        {
            vantor::Backlog::Log("ResourceLoader",
                                 "Trying to load a HDR texture with invalid "
                                 "path or texture is not HDR: "
                                     + path + ".",
                                 vantor::Backlog::LogLevel::ERR);
        }

        return texture;
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::TextureCube
    TextureLoader::LoadTextureCube(std::string top, std::string bottom, std::string left, std::string right, std::string front, std::string back)
    {
        vantor::Graphics::RenderDevice::OpenGL::TextureCube texture;

        stbi_set_flip_vertically_on_load(false);

        std::vector<std::string> faces = {top, bottom, left, right, front, back};
        for (unsigned int i = 0; i < faces.size(); ++i)
        {
            int            width, height, nrComponents;
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);

            if (data)
            {
                GLenum format;
                if (nrComponents == 3)
                    format = GL_RGB;
                else
                    format = GL_RGBA;

                texture.GenerateFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                vantor::Backlog::Log("ResourceLoader", "Cube texture at path: " + faces[i] + " failed to load.", vantor::Backlog::LogLevel::ERR);
                stbi_image_free(data);
                return texture;
            }
        }
        if (texture.Mipmapping) glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        return texture;
    }
    // --------------------------------------------------------------------------------------------
    vantor::Graphics::RenderDevice::OpenGL::TextureCube TextureLoader::LoadTextureCube(std::string folder)
    {
        return TextureLoader::LoadTextureCube(folder + "right.jpg", folder + "left.jpg", folder + "top.jpg", folder + "bottom.jpg", folder + "front.jpg",
                                              folder + "back.jpg");
    }
} // namespace vantor