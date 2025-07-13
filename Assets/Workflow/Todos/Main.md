# Vantor Todos

# Blog ideas

- How to start your Graphics Engine (RenderPasses, RenderPaths)
- How to do abstraction layer

# Line

 - Implement Deffered Commands in Command Buffer
 - Own GBuffer class in common folder ?
 - Entity ID pool / free list: Keep a list (stack or queue) of free entity IDs that get reused when entities are destroyed:
        When creating a new entity:

            If the free list is not empty, pop an ID from it.

            Otherwise, assign a new incremental ID.

        When destroying an entity:

            Add its ID back to the free list.
 

# Other

- Make Shaders work with includes, See Cell Engines Shaders (Custom Shader Parser) With own Single Header C utility
- VSyncOn/Off fucntion for Context
- VTransformComponent
- MaterialLibrary
- Remove Camera from RenderPasses
- Custom Shader / Common Uniform Includes (Maybe Slang)
- Profiling Macros and Structs to get data

## Bugs and fixes

 - LoadTexture2D should use CreateFromMemory not CreateFromFile (from VTexture2D), so support loading file with platform