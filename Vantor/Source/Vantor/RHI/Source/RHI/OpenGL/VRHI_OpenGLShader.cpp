/****************************************************************************
 * Vantor Engine™ - Source Code (2025)
 *
 * Author    : Lukas Rennhofer (@LukasRennhofer), Vantor Studios™
 * Copyright : © 2025 Lukas Rennhofer, Vantor Studios™
 * License   : GNU General Public License v3.0
 *             See LICENSE file for full details.
 ****************************************************************************/

#include <RHI/OpenGL/VRHI_OpenGLShader.hpp>

#include <iostream>
#include <unordered_map>

namespace Vantor::RHI
{

OpenGLShader::OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource)
    : m_program(0)
{
    // Compile vertex shader
    uint32_t vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
    if (vertexShader == 0) return;

    // Compile fragment shader
    uint32_t fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
    if (fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        return;
    }

    // Create program and link shaders
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    if (!CheckLinkErrors(m_program))
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }

    // Clean up shaders (they're linked into the program now)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

OpenGLShader::~OpenGLShader()
{
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
    }
}

void OpenGLShader::Use()
{
    if (m_program != 0)
    {
        glUseProgram(m_program);
    }
}

void OpenGLShader::SetBool(const std::string& name, bool value)
{
    glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}

void OpenGLShader::SetInt(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetVec2(const std::string& name, float x, float y)
{
    glUniform2f(GetUniformLocation(name), x, y);
}

void OpenGLShader::SetVec2(const std::string& name, const Vantor::Math::VVector2 &value)
{
    glUniform2fv(GetUniformLocation(name), 1,  value.Data());
}

void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z)
{
    glUniform3f(GetUniformLocation(name), x, y, z);
}

void OpenGLShader::SetVec3(const std::string& name, const Vantor::Math::VVector3 &value)
{
    glUniform3fv(GetUniformLocation(name), 1, value.Data());
}

void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w)
{
    glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void OpenGLShader::SetVec4(const std::string& name, const Vantor::Math::VVector4 &value)
{
    glUniform4fv(GetUniformLocation(name), 1, value.Data());
}

void OpenGLShader::SetMat2(const std::string& name, const Vantor::Math::VMat2 &mat)
{
    glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, mat.Data());
}

void OpenGLShader::SetMat3(const std::string& name, const Vantor::Math::VMat3 &mat)
{
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, mat.Data());
}

void OpenGLShader::SetMat4(const std::string& name, const Vantor::Math::VMat4 &mat)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, mat.Data());
}

uint32_t OpenGLShader::CompileShader(const std::string& source, GLenum type)
{
    uint32_t shader = glCreateShader(type);
    const char* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    std::string typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
    if (!CheckCompileErrors(shader, typeStr))
    {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool OpenGLShader::CheckCompileErrors(uint32_t shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                  << infoLog << "\n -- --------------------------------------------------- --" << std::endl;
        return false;
    }
    return true;
}

bool OpenGLShader::CheckLinkErrors(uint32_t program)
{
    int success;
    char infoLog[1024];
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" 
                  << infoLog << "\n -- --------------------------------------------------- --" << std::endl;
        return false;
    }
    return true;
}

int OpenGLShader::GetUniformLocation(const std::string& name)
{
    static std::unordered_map<std::string, int> uniformCache;
    
    auto it = uniformCache.find(name);
    if (it != uniformCache.end())
    {
        return it->second;
    }
    
    int location = glGetUniformLocation(m_program, name.c_str());
    uniformCache[name] = location;
    
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' not found in shader" << std::endl;
    }
    
    return location;
}

} // namespace Vantor::RHI
