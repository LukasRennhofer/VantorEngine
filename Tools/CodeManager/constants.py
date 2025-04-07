"""
*    				~ CHIFEngine ~
*
* Copyright (c) 2025 Lukas Rennhofer
*
* Licensed under the MIT License. See LICENSE file for more details.
*
* Author: Lukas Rennhofer
* Date: 03-23-2025
*
* File: constants.py
* Last Change: Tooling Updated
"""

SRC_DIR = "CHIFEngine"

TEST_ENV_DIR =  "Test"
GLOBAL_VERSION = "0.0.4"

MAINTAIN_AUTHOR = "Lukas Rennhofer"

SRC_DIR_DESCRIPTIONS = {}
# CHIFEngine/ DIR External Ressources
EXTERN_RESSOURCES = {
    'Jolt' : {
        'description' : 'CHIFEngines mainly used Physics Library.',
        'website' : 'https',
        'author' : 'Github@jrouwe'
    },
    'LUA' : {
        'description' : 'The Lua Interpreter',
        'website' : 'https',
        'author' : 'Github@lua'
    },
    'PS5' : {
        'description' : 'The PS5 CHIFEngine build files',
        'website' : 'None',
        'author' : 'Lukas Rennhofer, Sony Interactives'
    },
    'shaders' : {
        'description' : 'The CHIFEngine Shader Library',
        'website' : 'None',
        'author' : 'Lukas Rennhofer, (shadercomp)'
    },
    'Utility' : {
        'description' : 'CHIFEngine Util Library / Frameworks : *basis_u , dx12 API, FAudio, meshopt., vulkan API*',
        'website' : 'None',
    }
}