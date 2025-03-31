
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
 * File: chifBaseShader.h
 * Last Change: 
 */

#pragma once
#include <string>
#include <glad/glad.h>

#include "../../Core/Backlog/chifBacklog.h"

namespace chif::Graphics::Renderer::Shader {

	// Creating various shadertypes for Fragment and Vertecie Shaders
	struct shaderType
	{
		shaderType() : type(-1), name("") {}
		shaderType(unsigned int type, std::string name) : type(type), name(name) {}

		unsigned int type;
		std::string name;
	};

	bool checkCompileErrors(unsigned int shader, const std::string &type, const std::string &shaderName);
	std::string getShaderName(const char *shaderPath);
	shaderType getShaderType(const char *path);

	class BaseShader
	{
	public:
		BaseShader(const char *shaderPath);
		virtual ~BaseShader();

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
} // NAMESPACE chif::Graphics::Renderer::Shader
