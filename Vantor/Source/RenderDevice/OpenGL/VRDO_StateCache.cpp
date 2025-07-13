#include "VRDO_StateCache.hpp"

namespace Vantor::RenderDevice {
    void VOpenGLStateChache::SetDepthTest(bool enable)
    {
        if (m_DepthTest != enable)
        {
            m_DepthTest = enable;
            if (enable)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);
        }

    }

    void VOpenGLStateChache::SetDepthFunc(GLenum depthFunc)
    {
        if (m_DepthFunc != depthFunc)
        {
            m_DepthFunc = depthFunc;
            glDepthFunc(depthFunc);
        }
    }

    void VOpenGLStateChache::SetBlend(bool enable)
    {
        if (m_Blend != enable)
        {
            m_Blend = enable;
            if(enable)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }
    }

    void VOpenGLStateChache::SetBlendFunc(GLenum src, GLenum dst)
    {
        if (m_BlendSrc != src || m_BlendDst != dst)
        {
            m_BlendSrc = src;
            m_BlendDst = dst;
            glBlendFunc(src, dst);
        }
    }

    void VOpenGLStateChache::SetCull(bool enable)
    {
        if (m_CullFace != enable)
        {
            m_CullFace = enable;
            if(enable)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);
        }
    }

    void VOpenGLStateChache::SetCullFace(GLenum face)
    {
        if (m_FrontFace != face)
        {
            m_FrontFace = face;
            glCullFace(face);
        }
    }

    void VOpenGLStateChache::SetPolygonMode(GLenum mode)
    {
        if (m_PolygonMode != mode)
        {
            m_PolygonMode = mode;
            glPolygonMode(GL_FRONT_AND_BACK, mode);
        }
    }

    // TODO: Implement with Shader itself
    void VOpenGLStateChache::SwitchUsedShader(unsigned int ID)
    {
        if (m_UsedShaderID != ID)
        {
            glUseProgram(ID);
        }
    }
}