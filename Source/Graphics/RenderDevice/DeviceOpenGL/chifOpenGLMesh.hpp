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
 * File: chifOpenGLMesh.hpp 
 * Last Change: 
*/

// TODO: Maybe in Devices? and primitives will be abstracted by mesh

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <functional>

inline glm::vec3 xyz(const glm::vec4& v) { return glm::vec3(v.x, v.y, v.z); }
inline glm::vec2 xy(const glm::vec3& v) { return glm::vec2(v.x, v.y); }
inline glm::vec2 yz(const glm::vec3& v) { return glm::vec2(v.y, v.z); }

namespace chif::Graphics::RenderDevice::OpenGL {

    // ==== Basic OpenGL Topology ====
    enum TOPOLOGY
    {
        POINTS,
        LINES,
        LINE_STRIP,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
    };

    /* 

      =================== Base Mesh Class ====================

    */
    class Mesh
    {
    public:
        unsigned int m_VAO = 0;
        unsigned int m_VBO;
        unsigned int m_EBO;
    public:
        std::vector<glm::vec3> Positions;
        std::vector<glm::vec2> UV;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec3> Tangents;
        std::vector<glm::vec3> Bitangents;

        TOPOLOGY Topology = TRIANGLES;
        std::vector<unsigned int> Indices;

        Mesh();
        Mesh(std::vector<glm::vec3> positions, std::vector<unsigned int> indices);
        Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices);
        Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<unsigned int> indices);
        Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices);

        void SetPositions(std::vector<glm::vec3> positions);
        void SetUVs(std::vector<glm::vec2> uv);
        void SetNormals(std::vector<glm::vec3> normals);
        void SetTangents(std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents);

        void Finalize(bool interleaved = true);

        void FromSDF(std::function<float(glm::vec3)>& sdf, float maxDistance, uint16_t gridResolution);

    private:
        void calculateNormals(bool smooth = true);
        void calculateTangents();
    };
} // namespace chif::Graphics::RenderDevice::OpenGL
