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
 *  File: vantorOpenGLMesh.cpp
 *  Last Change: Automatically updated
 */

#include "vantorOpenGLMesh.hpp"
#include "../../../Core/BackLog/vantorBacklog.h"

namespace vantor::Graphics::RenderDevice::OpenGL
{
    // --------------------------------------------------------------------------------------------
    Mesh::Mesh() {}
    // --------------------------------------------------------------------------------------------
    Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<unsigned int> indices)
    {
        Positions = positions;
        Indices   = indices;
    }
    // --------------------------------------------------------------------------------------------
    Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<unsigned int> indices)
    {
        Positions = positions;
        UV        = uv;
        Indices   = indices;
    }
    // --------------------------------------------------------------------------------------------
    Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec2> uv, std::vector<glm::vec3> normals, std::vector<unsigned int> indices)
    {
        Positions = positions;
        UV        = uv;
        Normals   = normals;
        Indices   = indices;
    }
    // --------------------------------------------------------------------------------------------
    Mesh::Mesh(std::vector<glm::vec3>    positions,
               std::vector<glm::vec2>    uv,
               std::vector<glm::vec3>    normals,
               std::vector<glm::vec3>    tangents,
               std::vector<glm::vec3>    bitangents,
               std::vector<unsigned int> indices)
    {
        Positions  = positions;
        UV         = uv;
        Normals    = normals;
        Tangents   = tangents;
        Bitangents = bitangents;
        Indices    = indices;
    }
    // --------------------------------------------------------------------------------------------
    void Mesh::SetPositions(std::vector<glm::vec3> positions) { Positions = positions; }
    // --------------------------------------------------------------------------------------------
    void Mesh::SetUVs(std::vector<glm::vec2> uv) { UV = uv; }
    // --------------------------------------------------------------------------------------------
    void Mesh::SetNormals(std::vector<glm::vec3> normals) { Normals = normals; }
    // --------------------------------------------------------------------------------------------
    void Mesh::SetTangents(std::vector<glm::vec3> tangents, std::vector<glm::vec3> bitangents)
    {
        Tangents   = tangents;
        Bitangents = bitangents;
    }
    // --------------------------------------------------------------------------------------------
    void Mesh::Finalize(bool interleaved)
    {
        // initialize object IDs if not configured before
        if (!m_VAO)
        {
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);
        }

        std::vector<float> data;
        if (interleaved)
        {
            for (int i = 0; i < Positions.size(); ++i)
            {
                data.push_back(Positions[i].x);
                data.push_back(Positions[i].y);
                data.push_back(Positions[i].z);
                if (UV.size() > 0)
                {
                    data.push_back(UV[i].x);
                    data.push_back(UV[i].y);
                }
                if (Normals.size() > 0)
                {
                    data.push_back(Normals[i].x);
                    data.push_back(Normals[i].y);
                    data.push_back(Normals[i].z);
                }
                if (Tangents.size() > 0)
                {
                    data.push_back(Tangents[i].x);
                    data.push_back(Tangents[i].y);
                    data.push_back(Tangents[i].z);
                }
                if (Bitangents.size() > 0)
                {
                    data.push_back(Bitangents[i].x);
                    data.push_back(Bitangents[i].y);
                    data.push_back(Bitangents[i].z);
                }
            }
        }
        else
        {
            for (int i = 0; i < Positions.size(); ++i)
            {
                data.push_back(Positions[i].x);
                data.push_back(Positions[i].y);
                data.push_back(Positions[i].z);
            }
            for (int i = 0; i < UV.size(); ++i)
            {
                data.push_back(UV[i].x);
                data.push_back(UV[i].y);
            }
            for (int i = 0; i < Normals.size(); ++i)
            {
                data.push_back(Normals[i].x);
                data.push_back(Normals[i].y);
                data.push_back(Normals[i].z);
            }
            for (int i = 0; i < Tangents.size(); ++i)
            {
                data.push_back(Tangents[i].x);
                data.push_back(Tangents[i].y);
                data.push_back(Tangents[i].z);
            }
            for (int i = 0; i < Bitangents.size(); ++i)
            {
                data.push_back(Bitangents[i].x);
                data.push_back(Bitangents[i].y);
                data.push_back(Bitangents[i].z);
            }
        }

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

        if (Indices.size() > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
        }
        if (interleaved)
        {
            size_t stride = 3 * sizeof(float);
            if (UV.size() > 0) stride += 2 * sizeof(float);
            if (Normals.size() > 0) stride += 3 * sizeof(float);
            if (Tangents.size() > 0) stride += 3 * sizeof(float);
            if (Bitangents.size() > 0) stride += 3 * sizeof(float);

            size_t offset = 0;
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
            offset += 3 * sizeof(float);
            if (UV.size() > 0)
            {
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
                offset += 2 * sizeof(float);
            }
            if (Normals.size() > 0)
            {
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
                offset += 3 * sizeof(float);
            }
            if (Tangents.size() > 0)
            {
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
                offset += 3 * sizeof(float);
            }
            if (Bitangents.size() > 0)
            {
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *) offset);
                offset += 3 * sizeof(float);
            }
        }
        else
        {
            size_t offset = 0;
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) offset);
            offset += Positions.size() * sizeof(float);
            if (UV.size() > 0)
            {
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *) offset);
                offset += UV.size() * sizeof(float);
            }
            if (Normals.size() > 0)
            {
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) offset);
                offset += Normals.size() * sizeof(float);
            }
            if (Tangents.size() > 0)
            {
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) offset);
                offset += Tangents.size() * sizeof(float);
            }
            if (Bitangents.size() > 0)
            {
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) offset);
                offset += Bitangents.size() * sizeof(float);
            }
        }
        glBindVertexArray(0);
    }
    // --------------------------------------------------------------------------------------------
    void Mesh::FromSDF(std::function<float(glm::vec3)> &sdf, float maxDistance, uint16_t gridResolution)
    {
        vantor::Backlog::Log("OpenGLMesh", "Generating 3D mesh from SDF", vantor::Backlog::LogLevel::DEBUG);

        // tables from: http://paulbourke.net/geometry/polygonise/
        static int edgeTable[256]
            = {0x0,   0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c, 0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00, 0x190, 0x99,  0x393, 0x29a,
               0x596, 0x49f, 0x795, 0x69c, 0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90, 0x230, 0x339, 0x33,  0x13a, 0x636, 0x73f, 0x435, 0x53c,
               0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30, 0x3a0, 0x2a9, 0x1a3, 0xaa,  0x7a6, 0x6af, 0x5a5, 0x4ac, 0xbac, 0xaa5, 0x9af, 0x8a6,
               0xfaa, 0xea3, 0xda9, 0xca0, 0x460, 0x569, 0x663, 0x76a, 0x66,  0x16f, 0x265, 0x36c, 0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
               0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff,  0x3f5, 0x2fc, 0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0, 0x650, 0x759, 0x453, 0x55a,
               0x256, 0x35f, 0x55,  0x15c, 0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950, 0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc,
               0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0, 0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc, 0xcc,  0x1c5, 0x2cf, 0x3c6,
               0x4ca, 0x5c3, 0x6c9, 0x7c0, 0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c, 0x15c, 0x55,  0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
               0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc, 0x2fc, 0x3f5, 0xff,  0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0, 0xb60, 0xa69, 0x963, 0x86a,
               0xf66, 0xe6f, 0xd65, 0xc6c, 0x36c, 0x265, 0x16f, 0x66,  0x76a, 0x663, 0x569, 0x460, 0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
               0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa,  0x1a3, 0x2a9, 0x3a0, 0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c, 0x53c, 0x435, 0x73f, 0x636,
               0x13a, 0x33,  0x339, 0x230, 0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c, 0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99,  0x190,
               0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c, 0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0};
        static int triTable[256][16] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
                                        {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
                                        {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
                                        {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
                                        {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
                                        {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
                                        {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
                                        {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
                                        {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
                                        {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
                                        {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
                                        {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
                                        {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
                                        {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
                                        {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
                                        {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
                                        {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
                                        {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
                                        {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
                                        {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
                                        {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
                                        {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
                                        {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
                                        {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
                                        {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
                                        {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
                                        {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
                                        {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
                                        {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
                                        {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
                                        {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
                                        {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
                                        {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
                                        {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
                                        {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
                                        {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
                                        {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
                                        {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
                                        {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
                                        {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
                                        {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
                                        {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
                                        {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
                                        {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                                        {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
                                        {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
                                        {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
                                        {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
                                        {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
                                        {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        UV.push_back(xy(vertList[triTable[index][i + 0]]));
                        UV.push_back(yz(vertList[triTable[index][i + 1]]));
                        UV.push_back(xy(vertList[triTable[index][i + 2]]) * 0.5f + yz(vertList[triTable[index][i + 2]]) * 0.5f);
                    }

        Topology = TRIANGLES;
        Finalize();

        vantor::Backlog::Log("OpenGLMesh", "SDF mesh generation complete!", vantor::Backlog::LogLevel::DEBUG);
    }
    // --------------------------------------------------------------------------------------------
    void Mesh::calculateNormals(bool smooth) {}
    // --------------------------------------------------------------------------------------------
    void Mesh::calculateTangents()
    {
        // tangents.resize(positions.size());
        // bitangents.resize(positions.size());
        // for (unsigned int i = 0; i < indices.size() - 2; ++i)
        //{
        //     unsigned int index1 = indices[i + 0];
        //     unsigned int index2 = indices[i + 1];
        //     unsigned int index3 = indices[i + 2];
        //     // TODO: do we take different winding order into account for
        //     triangle strip? glm::vec3 pos1 = positions[index1]; glm::vec3
        //     pos2 = positions[index2]; glm::vec3 pos3 = positions[index3];

        //    glm::vec2 uv1 = uv[index1];
        //    glm::vec2 uv2 = uv[index2];
        //    glm::vec2 uv3 = uv[index3];

        //    // due to winding order getting changed each next triangle (as we
        //    render as triangle strip) we
        //    // change the order of the cross product to account for winding
        //    order switch glm::vec3 edge1 = pos2 - pos1; glm::vec3 edge2 = pos3
        //    - pos1; glm::vec2 deltaUV1 = uv2 - uv1; glm::vec2 deltaUV2 = uv3 -
        //    uv1;

        //    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x *
        //    deltaUV1.y);

        //    glm::vec3 tangent, bitangent;
        //    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        //    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        //    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        //    /*    if (i % 2 == 0)
        //    {*/
        //    bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        //    bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        //    bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        //    //}
        //    /*    else
        //    {
        //    bitangent.x = f * (-deltaUV2.x * edge2.x + deltaUV1.x * edge1.x);
        //    bitangent.y = f * (-deltaUV2.x * edge2.y + deltaUV1.x * edge1.y);
        //    bitangent.z = f * (-deltaUV2.x * edge2.z + deltaUV1.x * edge1.z);
        //    }*/
        //    tangents[index1] += tangent;
        //    tangents[index2] += tangent;
        //    tangents[index3] += tangent;
        //    bitangents[index1] += bitangent;
        //    bitangents[index2] += bitangent;
        //    bitangents[index3] += bitangent;
        //}
        //// normalize all tangents/bi-tangents
        // for (int i = 0; i < tangents.size(); ++i)
        //{
        //     tangents[i] = glm::normalize(tangents[i]);
        //     bitangents[i] = glm::normalize(bitangents[i]);
        // }
    }
} // namespace vantor::Graphics::RenderDevice::OpenGL