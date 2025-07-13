#version 330 core

// TODO: Make Shaders work with includes, See Cell Engines Shaders

// Constants
#define FORWARD_MAX_POINT_LIGHTS 8 // Just for Forward Rendering, because of GLSL flexibility issues

struct PointLight {
    vec3 position;
    float constant;        // attenuation constant

    vec3 ambient;
    float linear;          // attenuation linear

    vec3 diffuse;
    float quadratic;       // attenuation quadratic

    vec3 specular;
    float padding;         // pad to 16 bytes alignment
};

// This is currently bound to the Update function in the VOpenGLUBO and binding 0
layout(std140, binding = 0) uniform VCommonUniforms
{
    mat4 Vu_View;
    mat4 Vu_Projection;
    vec3 Vu_CameraPosition;
    float _pad0; // padding for alignment

    int Vu_NumPointLights;
    vec3 _pad1; // padding for alignment

    PointLight Vu_PointLights[FORWARD_MAX_POINT_LIGHTS];
};
