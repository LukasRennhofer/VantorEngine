#include "VRDO_ScreenQuad.hpp"

namespace Vantor::RenderDevice {
    VScreenQuad::~VScreenQuad() = default;

    VOpenGLScreenQuad::VOpenGLScreenQuad() {
        // Loading the Screen Quad Shader
        auto ResShaderScreenQuad = Vantor::Resource::VResourceManager::Instance().LoadShaderProgram("VIShaderScreenQuad", "Shaders/Private/Common/VScreenQuad.vglsl", "Shaders/Private/Common/VScreenQuad.fglsl");

        m_ShaderScreenQuad = ResShaderScreenQuad->GetShader();

        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    VOpenGLScreenQuad::~VOpenGLScreenQuad() {
        glDeleteVertexArrays(1, &m_VAO);
        // Shader deleted by unique_ptr
    }

    void VOpenGLScreenQuad::Draw() {
        m_ShaderScreenQuad->Use();
        m_ShaderScreenQuad->setUniformInt("screenTexture", 1);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void VOpenGLScreenQuad::DrawRaw() {
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
}