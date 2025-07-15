#include "VRDO_UBO.hpp"

namespace Vantor::RenderDevice {
    VOpenGLCommonUBO::VOpenGLCommonUBO()
    {
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    VOpenGLCommonUBO::~VOpenGLCommonUBO()
    {
        if (m_Buffer != 0)
            glDeleteBuffers(1, &m_Buffer);
    }

    void VOpenGLCommonUBO::Update(Vantor::Renderer::Camera* Camera)
    {
        if (!Camera) { return; }  // Return if camera is NULL (fixed logic)

        m_Data.view = Camera->View;
        m_Data.projection = Camera->Projection;
        m_Data.cameraPosition = Camera->Position;
        m_Data.padding0 = 0.0f;

        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLCommonUBO::Bind(GLuint bindingPoint) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer);
    }

    VOpenGLLightDataUBO::VOpenGLLightDataUBO()
    {
        glGenBuffers(1, &m_Buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UBOData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    VOpenGLLightDataUBO::~VOpenGLLightDataUBO()
    {
        if (m_Buffer != 0)
            glDeleteBuffers(1, &m_Buffer);
    }

    void VOpenGLLightDataUBO::Update()
    {
        // Upload updated UBO data to GPU
        glBindBuffer(GL_UNIFORM_BUFFER, m_Buffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UBOData), &m_Data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    // Bind the UBO at beginning of rendering, after updating the UBO
    void VOpenGLLightDataUBO::Bind(GLuint bindingPoint) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer);
    }

    void VOpenGLLightDataUBO::UploadLightData(int numPointLights) {
        m_Data.numPointLights = numPointLights;
    }
}