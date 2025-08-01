#pragma once

#include "../common.h"

inline std::shared_ptr<VObject> CreateCubeObject(VRDevice* RenderDevice, VVector3 position = VVector3(1.0f, 1.0f, 1.0f)) {
    auto cube = vCreateActor<VCube>(); // Create Entity

    cube->AddComponentVoid<VMeshComponent>(); // Add a MeshComponent for Render
    cube->AddComponentVoid<VTransformComponent>(); // Add a TransformComponent for Render
    cube->AddComponentVoid<VMaterialComponent>(); // Add a MaterialComponent for Render

    VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = RenderDevice->CreateMesh(cubeCreateInfo);

    cube->GetComponent<VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent
    
    cube->GenerateMesh();

    auto cubeMaterial = VMaterialLibrary::Instance().CreateMaterial("VDefault");

    cube->GetComponent<VMaterialComponent>()->SetMaterial(cubeMaterial.get());

    cube->GetComponent<VTransformComponent>()->SetPosition(position);
    cube->GetComponent<VTransformComponent>()->SetScale({0.1f, 0.1f, 0.1f});

    return cube;
}

inline std::shared_ptr<VObject> CreatePlaneObject(VRDevice* RenderDevice, float width = 1.0f, float height = 1.0f, float depth = 0.0f, VVector3 position = VVector3(1.0f, 1.0f, 1.0f)) {
    auto cube = vCreateActor<VPlane>(width, height, depth); // Create Entity

    cube->AddComponentVoid<VMeshComponent>(); // Add a MeshComponent for Render
    cube->AddComponentVoid<VTransformComponent>(); // Add a TransformComponent for Render
    cube->AddComponentVoid<VMaterialComponent>(); // Add a MaterialComponent for Render

    VMeshCreateInfo cubeCreateInfo; // Create empty Mesh with Creation Data
    cubeCreateInfo.SetFinalized = false;

    auto cubeMesh = RenderDevice->CreateMesh(cubeCreateInfo);

    cube->GetComponent<VMeshComponent>()->SetMesh(cubeMesh); // Add empty Mesh to the cubes MeshComponent
    
    cube->GenerateMesh();

    auto cubeMaterial = VMaterialLibrary::Instance().CreateMaterial("VDefault");

    cube->GetComponent<VMaterialComponent>()->SetMaterial(cubeMaterial.get());

    cube->GetComponent<VTransformComponent>()->SetPosition(position);
    cube->GetComponent<VTransformComponent>()->SetScale({0.1f, 0.1f, 0.1f});

    return cube;
}