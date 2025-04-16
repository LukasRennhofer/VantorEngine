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
 * File: chifOpenGLShader.hpp
 * Last Change: 
*/

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "../../../Core/Backlog/chifBacklog.h"
namespace chif::Graphics::RenderDevice::OpenGL {
    // =========== Base Shader ===========
	struct shaderType
	{
		shaderType() : type(-1), name("") {}
		shaderType(unsigned int type, std::string name) : type(type), name(name) {}

		unsigned int type;
		std::string name;
	};

    // Standart Shader Functions
	bool checkCompileErrors(unsigned int shader, const std::string &type, const std::string &shaderName);
	std::string getShaderName(const char *shaderPath);
	shaderType getShaderType(const char *path);

	class OpenGLBaseShader
	{
	public:
	OpenGLBaseShader(const char *shaderPath);
		virtual ~OpenGLBaseShader();

		std::string getName()
		{
			return getShaderName(path.c_str());
		}

		shaderType getType();
		unsigned int getShad();

	private:
		std::string loadShaderFromFile(const char *shaderPath);
		std::string path;
		unsigned int shad;
		shaderType shadType;
	};

    // =========== Shader ===========
	class OpenGLShader
	{
	public:
		OpenGLShader(std::string name);
		OpenGLShader(std::string name, const char * computeShader);
		OpenGLShader * attachShader(OpenGLBaseShader s);

		unsigned int ID;
		
		void linkPrograms();

		virtual ~OpenGLShader();
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec2(const std::string &name, glm::vec2 vector) const;
		void setVec3(const std::string &name, glm::vec3 vector) const;
		void setVec4(const std::string &name, glm::vec4 vector) const;
		void setMat4(const std::string &name, glm::mat4 matrix) const;
		void setSampler2D(const std::string &name, unsigned int texture, int id) const;
		void setSampler3D(const std::string &name, unsigned int texture, int id) const;

	protected:

		bool linked, isCompute; // Checking Variables
		std::list<unsigned int> shaders;
		std::string name;
	};
} // namespace chif::Graphics::RenderDevice::OpenGL