
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
 * File: BaseShader.cpp
 * Last Change: 
 */

#include "BaseShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace chif::Graphics::Renderer::Shader {
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
				std::cout << "[ERROR::SHADER] SHADER \"" << shaderName << "\" COMPILATION ERROR of type: \"" << type << "\"\n"
						<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "[ERROR::SHADER] PROGRAM_LINKING_ERROR of type: \"" << type << "\"\n"
						<< infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
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

	BaseShader::BaseShader(const char *shaderPath)
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

	std::string BaseShader::loadShaderFromFile(const char *shaderPath)
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
			std::cout << "[ERROR::SHADER] SHADER \"" << getShaderName(shaderPath) << "\" FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		return shaderCode;
	}

	BaseShader::~BaseShader()
	{
		// glDeleteShader(shad); 
	}

	unsigned int BaseShader::getShad()
	{
		return shad;
	}
} // NAMESPACE chif::Graphics::Renderer::Shader
