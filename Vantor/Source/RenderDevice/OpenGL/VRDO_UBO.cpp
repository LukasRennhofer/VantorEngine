#include "VRDO_UBO.hpp"

namespace Vantor::RenderDevice {
    VOpenGLUBO::VOpenGLUBO()
    {
        glGenBuffers(1, &m_UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // Bind the UBO to binding point 0 (must match GLSL)
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBO);
    }

    VOpenGLUBO::~VOpenGLUBO()
    {
        if (m_UBO != 0)
            glDeleteBuffers(1, &m_UBO);
    }

    void VOpenGLUBO::Update(
        Vantor::Renderer::Camera* camera,
        const std::vector<Vantor::Renderer::VPointLightData>& pointLights)
    {
        m_Data.view = camera->View;
        m_Data.projection = camera->Projection;
        m_Data.cameraPosition = camera->Position;
        m_Data.padding0 = 0.0f;

        // === Point Lights ===
        // Only works for forward now
        if ((int)pointLights.size() > FORWARD_MAX_POINT_LIGHTS) {
            m_Data.numPointLights = FORWARD_MAX_POINT_LIGHTS;
        } else {
            m_Data.numPointLights = (int)pointLights.size();
        }

        // Clear previous lights with zeros (using default constructor or zero initialization)
        for (int i = 0; i < FORWARD_MAX_POINT_LIGHTS; ++i) {
            m_Data.pointLights[i] = Vantor::Renderer::VPointLightData{
                Vantor::Math::VVector3(0.0f, 0.0f, 0.0f),
                0.0f,
                Vantor::Math::VVector3(0.0f, 0.0f, 0.0f),
                0.0f,
                Vantor::Math::VVector3(0.0f, 0.0f, 0.0f),
                0.0f,
                Vantor::Math::VVector3(0.0f, 0.0f, 0.0f)
            };
        }

        // Copy the input point lights data into the UBO data
        for (int i = 0; i < m_Data.numPointLights; ++i) {
            m_Data.pointLights[i] = pointLights[i];
        }

        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLUBO::Bind() const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBO);
    }

}