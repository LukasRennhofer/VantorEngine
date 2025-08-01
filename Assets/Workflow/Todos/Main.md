# Vantor Todos

# Blog ideas

- How to start your Graphics Engine (RenderPasses, RenderPaths)
- How to do abstraction layer
- A closer look at RAGE (GTA VI Engine)

# Line

 - New RenderFactory Design:
    - VRenderCoordinator : This VSingleton can create and manage RenderDevices with a VERenderAPI and stores them 
                           in its private. You can access the API with: GetBackend() and get the RenderDevice 
                          (which is a shared Pointer) with :  GetRenderDevice(), wwe do this alll before Initializing
                          the Application Context and the Application gets the RenderDevice through the VRenderCoordinator

    - This Class can be used through the whole Engine code to create and manage Meshes, Textures and Shaders

    - User API:
        The User itself can access the RenderDevice with special made API functions to create its own resources
        e.g.
                        vRDevice_CreateMesh(...);
                        vRDevice_CreateTexture2D(...);
                        vRDevice_SetActive(...);

        or:             vTexture_Create2D(...);
                        vTexture_CreateFromFile(...);



 - Different Modes : Studio Mode, Dev Mode (Debugging), Release Mode
 - Implement Frustum in CommandBuffer
 - dirLight, SpotLight, AreaLight
 - Model Loading ?
 - Resource Manager GetTexture2D(handle) method and so on ?

 - PBR:	
    0   RGB8 or RGBA8	Albedo (RGB) + AO (A)
    1	RGB16F	World-space normals
    2	RGB16F	World-space position
    3	RGB8 or RGBA8	Metallic (R), Roughness (G), AO (B), maybe Specular (A)

 - Let Users be able to make their own Virtual Include paths for Shaders ?
 
# Performance

 - Implement Tiled Deferred Shading for lights
 - Use SIMD for AABB, collision, Vector/Matrix Math
 - Use VVector and VSafeString anywhere
 - Use frustum culling for meshes and lights
 - Make copys of Meshes to point to one Mesh, to reduce memory usage

 - Entity ID pool / free list: Keep a list (stack or queue) of free entity IDs that get reused when entities are destroyed:
        When creating a new entity:

            If the free list is not empty, pop an ID from it.

            Otherwise, assign a new incremental ID.

        When destroying an entity:

            Add its ID back to the free list.

# Other
- Vulkan Style User API (dont use full names or Namespaces, use handles for smart pointers (like VRDeviceHandle*))
- Profiling Macros and Structs to get data

## Bugs and fixes

 - Remove the newly added Wireframe on Code and use the StateCache off the RenderPath instead
 - LoadTexture2D should use CreateFromMemory not CreateFromFile (from VTexture2D), so support loading file with platform