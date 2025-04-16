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
 * File: chifOpenGLShader.cpp
 * Last Change: 
*/


#include "chifOpenglShader.hpp"
namespace chif::Graphics::RenderDevice::OpenGL {
    // =========== Base Shader ===========
    bool checkCompileErrors(unsigned int shader, const std::string &type, const std::string &shaderName)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);

				std::string errorMessage = "Shader: \"" + shaderName  + "\" Compilation Error of type: " + type;
				chif::Backlog::Log("Shader", errorMessage, chif::Backlog::LogLevel::ERR); 
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::string errorMessage = "Programming linking error of type: " + type;
				chif::Backlog::Log("Shader", errorMessage, chif::Backlog::LogLevel::ERR); 
			}
		}
		return success;
	}

	std::string getShaderName(const char *path)
	{
		std::string pathstr = std::string(path);
		const size_t last_slash_idx = pathstr.find_last_of("/");
		if (std::string::npos != last_slash_idx)
		{
			pathstr.erase(0, last_slash_idx + 1);
		}
		return pathstr;
	}

	shaderType getShaderType(const char *path)
	{
		std::string type = getShaderName(path);
		const size_t last_slash_idx = type.find_last_of(".");
		if (std::string::npos != last_slash_idx)
		{
			type.erase(0, last_slash_idx + 1);
		}
		if (type == "vert")
			return shaderType(GL_VERTEX_SHADER, "VERTEX");
		if (type == "frag")
			return shaderType(GL_FRAGMENT_SHADER, "FRAGMENT");
		if (type == "tes")
			return shaderType(GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
		if (type == "tcs")
			return shaderType(GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
		if (type == "geom")
			return shaderType(GL_GEOMETRY_SHADER, "GEOMETRY");
		if (type == "comp")
			return shaderType(GL_COMPUTE_SHADER, "COMPUTE");
	}

	OpenGLBaseShader::OpenGLBaseShader(const char *shaderPath)
	{
		path = std::string(shaderPath);
		std::string shaderCode = loadShaderFromFile(shaderPath);
		const char *shaderString = shaderCode.c_str();

		shadType = getShaderType(shaderPath);
		shad = glCreateShader(shadType.type);
		glShaderSource(shad, 1, &shaderString, NULL);
		glCompileShader(shad);
		checkCompileErrors(shad, shadType.name.c_str(), getShaderName(shaderPath));
	}

	std::string OpenGLBaseShader::loadShaderFromFile(const char *shaderPath)
	{
		std::string shaderCode;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			shaderFile.open(shaderPath);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			shaderCode = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::string errorMessage = "Shader: \"" + getShaderName(shaderPath) + "\" not succesfully read";
			chif::Backlog::Log("Shader", errorMessage, chif::Backlog::LogLevel::ERR); 
		}
		return shaderCode;
	}

	OpenGLBaseShader::~OpenGLBaseShader()
	{
		// glDeleteShader(shad); 
	}

	unsigned int OpenGLBaseShader::getShad()
	{
		return shad;
	}

    // =========== Shader ===========
	OpenGLShader::OpenGLShader(std::string name) : name(name)
	{
		linked = false;
		isCompute = false;
		ID = glCreateProgram();
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(ID);
	}

	OpenGLShader::OpenGLShader(std::string name, const char * computeShaderPath) : name(name)
	{
		linked = false;
		isCompute = false;
		ID = glCreateProgram();

		this->attachShader(chif::Graphics::RenderDevice::OpenGL::OpenGLBaseShader(computeShaderPath));
		this->linkPrograms();
	}


	OpenGLShader * OpenGLShader::attachShader(chif::Graphics::RenderDevice::OpenGL::OpenGLBaseShader s)
	{
		if (!isCompute) {
			glAttachShader(ID, s.getShad());
			if (s.getName() == "COMPUTE")
				isCompute = true;
			this->shaders.push_back(s.getShad());
		}
		else {
			chif::Backlog::Log("Shader", "Trying to link a non compute shader to compute program.", chif::Backlog::LogLevel::ERR);
		}

		return this;
	}

	void OpenGLShader::linkPrograms()
	{
		glLinkProgram(ID);

		if (checkCompileErrors(ID, "PROGRAM", "")) {
			linked = true;
			while (!shaders.empty()) {
				glDeleteShader(shaders.back());
				shaders.pop_back();
			}
		}
		else {
			std::string errorMessage = "Error occured while linking to: " + name;
			chif::Backlog::Log("Shader", errorMessage, chif::Backlog::LogLevel::ERR);
		}
	}

	void OpenGLShader::use()
	{
		if (linked) {
			glUseProgram(ID);
		} else {
			chif::Backlog::Log("Shader", "Programs not linked!", chif::Backlog::LogLevel::ERR);

			// Check linking status and log
			GLint success;
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				char log[512];
				glGetProgramInfoLog(ID, 512, NULL, log);
				std::string errorMessage = "Shader linking Error";
				chif::Backlog::Log("Shader", errorMessage, chif::Backlog::LogLevel::ERR);
			} else {
				chif::Backlog::Log("Shader", "'linked' flag is false, but OpenGL thinks the program is linked.", chif::Backlog::LogLevel::WARNING);
			}
		}
	}

	// ------------------------------------------------------------------------
	void OpenGLShader::setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setVec2(const std::string &name, glm::vec2 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(vector));
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setVec3(const std::string &name, glm::vec3 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(vector));
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setVec4(const std::string &name, glm::vec4 vector) const
	{
		unsigned int location = glGetUniformLocation(ID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(vector));
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setMat4(const std::string &name, glm::mat4 matrix) const
	{
		unsigned int mat = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(mat, 1, false, glm::value_ptr(matrix));
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setSampler2D(const std::string &name, unsigned int texture, int id) const
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, texture);
		this->setInt(name, id);
	}
	// ------------------------------------------------------------------------
	void OpenGLShader::setSampler3D(const std::string &name, unsigned int texture, int id) const
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_3D, texture);
		this->setInt(name, id);
	}
} // namespace chif::Graphics::RenderDevice::OpenGL