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
 *  Date: 2025-07-16
 *
 *  File: VRDO_UBO.cpp
 *  Last Change: Automatically updated
 */

#include "VRDO_UBO.hpp"

namespace Vantor::RenderDevice
{
    VOpenGLCommonUBO::VOpenGLCommonUBO()
    {
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    VOpenGLCommonUBO::~VOpenGLCommonUBO()
    {
        if (m_Buffer != 0) glDeleteBuffers(1, &m_Buffer);
    }

    void VOpenGLCommonUBO::Update(Vantor::Renderer::Camera *Camera)
    {
        if (!Camera)
        {
            return;
        } // Return if camera is NULL (fixed logic)

        m_Data.view           = Camera->View;
        m_Data.projection     = Camera->Projection;
        m_Data.cameraPosition = Camera->Position;
        m_Data.padding0       = 0.0f;

        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLCommonUBO::Bind(GLuint bindingPoint) const { glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer); }

    VOpenGLLightDataUBO::VOpenGLLightDataUBO()
    {
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    VOpenGLLightDataUBO::~VOpenGLLightDataUBO()
    {
        if (m_Buffer != 0) glDeleteBuffers(1, &m_Buffer);
    }

    void VOpenGLLightDataUBO::Update()
    {
        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLLightDataUBO::Bind(GLuint bindingPoint) const { glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer); }

    void VOpenGLLightDataUBO::UploadLightData(int numPointLights, int numDirectionalLights, int numSpotLights) { 
        m_Data.numPointLights = numPointLights;
        m_Data.numDirectionalLights = numDirectionalLights;
        m_Data.numSpotLights = numSpotLights; 
    }

    // AmbientLight UBO
    VOpenGLAmbientLightUBO::VOpenGLAmbientLightUBO()
    {
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    VOpenGLAmbientLightUBO::~VOpenGLAmbientLightUBO()
    {
        if (m_Buffer != 0) glDeleteBuffers(1, &m_Buffer);
    }

    void VOpenGLAmbientLightUBO::Update()
    {
        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLAmbientLightUBO::Bind(GLuint bindingPoint) const { glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer); }

    void VOpenGLAmbientLightUBO::UploadLightData(Vantor::Renderer::VAmbientLightData &ambientLightdata) { 
        m_Data.ambientLightdata = ambientLightdata;
    }
} // namespace Vantor::RenderDevice