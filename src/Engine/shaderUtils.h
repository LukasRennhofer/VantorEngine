
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
 * File: shaderUtils.h
 * Last Change: 
 */
 

#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#include <string>

struct shaderType {
	shaderType() : type(-1), name("") {}
	shaderType(unsigned int type, std::string name) : type(type), name(name) {}
	unsigned int type;
	std::string name;
};

bool checkCompileErrors(unsigned int shader, std::string type, std::string shaderName);
std::string getShaderName(const char* shaderPath);
shaderType getShaderType(const char* path);
#endif