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
 *  Date: 2025-07-11
 *
 *  File: VRDO_Shader.cpp
 *  Last Change: Automatically updated
 */

// Core
#include "../../Core/Backlog/VCO_Backlog.hpp"
#include "VRDO_Shader.hpp"
#include "VRDO_Shader.hpp"

// TODO: Work with differemt Filesystems (e.g. Switch)
namespace Vantor::RenderDevice
{

    VOpenGLShader::VOpenGLShader(const char *vertexCode, const char *fragmentCode)
    {

        GLuint vertexShader   = compileShader(GL_VERTEX_SHADER, vertexCode);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

        ID = linkProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    VOpenGLShader::~VOpenGLShader() { glDeleteProgram(ID); }

    void VOpenGLShader::use() const { glUseProgram(ID); }

    void VOpenGLShader::setUniformBool(const std::string &name, bool value) const { glUniform1i(getUniformLocation(name), static_cast<int>(value)); }

    void VOpenGLShader::setUniformInt(const std::string &name, int value) const { glUniform1i(getUniformLocation(name), value); }

    void VOpenGLShader::setUniformFloat(const std::string &name, float value) const { glUniform1f(getUniformLocation(name), value); }

    void VOpenGLShader::setVec2(const std::string &name, const Vantor::Math::VVector2 &value) const { glUniform2fv(getUniformLocation(name), 1, value.Data()); }

    void VOpenGLShader::setVec2(const std::string &name, float x, float y) const { glUniform2f(getUniformLocation(name), x, y); }

    void VOpenGLShader::setVec3(const std::string &name, const Vantor::Math::VVector3 &value) const { glUniform3fv(getUniformLocation(name), 1, value.Data()); }

    void VOpenGLShader::setVec3(const std::string &name, float x, float y, float z) const { glUniform3f(getUniformLocation(name), x, y, z); }

    void VOpenGLShader::setVec4(const std::string &name, const Vantor::Math::VVector4 &value) const { glUniform4fv(getUniformLocation(name), 1, value.Data()); }

    void VOpenGLShader::setVec4(const std::string &name, float x, float y, float z, float w) const { glUniform4f(getUniformLocation(name), x, y, z, w); }

    void VOpenGLShader::setMat2(const std::string &name, const Vantor::Math::VMat2 &mat) const
    {
        glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, mat.Data());
    }

    void VOpenGLShader::setMat3(const std::string &name, const Vantor::Math::VMat3 &mat) const
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, mat.Data());
    }

    void VOpenGLShader::setMat4(const std::string &name, const Vantor::Math::VMat4 &mat) const
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, mat.Data());
    }

    GLuint VOpenGLShader::compileShader(GLenum type, const std::string &source)
    {
        GLuint      shader = glCreateShader(type);
        const char *src    = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            Vantor::Backlog::Log("RenderDevice::OpenGL", "Shader Compilation Failed", Vantor::Backlog::LogLevel::ERR);

            std::cout << infoLog << std::endl;
        }

        return shader;
    }

    GLuint VOpenGLShader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetProgramInfoLog(program, 512, nullptr, infoLog);
            Vantor::Backlog::Log("RenderDevice::OpenGL", "Shader Linking Failed", Vantor::Backlog::LogLevel::ERR);
        }

        return program;
    }

    GLint VOpenGLShader::getUniformLocation(const std::string &name) const { return glGetUniformLocation(ID, name.c_str()); }

} // namespace Vantor::RenderDevice