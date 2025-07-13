#include "VRDO_ScreenQuad.hpp"

namespace Vantor::RenderDevice {
    VScreenQuad::~VScreenQuad() = default;

    VOpenGLScreenQuad::VOpenGLScreenQuad() {
        // Loading the Screen Quad Shader
        auto ResShaderScreenQuad = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("VIShaderScreenQuad", "Shaders/Private/Common/VScreenQuad.vglsl", "Shaders/Private/Common/VScreenQuad.fglsl");

        m_ShaderScreenQuad = ResShaderScreenQuad->GetShader();

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        // No VBO needed when using gl_VertexID
        glBindVertexArray(0);
    }

    VOpenGLScreenQuad::~VOpenGLScreenQuad() {
        glDeleteVertexArrays(1, &m_VAO);
        // Shader deleted by unique_ptr
    }

    void VOpenGLScreenQuad::Draw() {
        m_ShaderScreenQuad->use();
        m_ShaderScreenQuad->setUniformInt("screenTexture", 1);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
}