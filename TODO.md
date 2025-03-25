# TODOS of CHIFEngine future updates

 - New Architecture:
     - chif::
        - Entitiy::
            - EntityBase (Class)
            - ComponentBase (Class)
            - Mesh (Class)
            - Model (Class)
            - Camera (Class)
        - Network::
            - Server (Class)
            - Client (Class)
        - Core::
            - Version::
            - (Resource Managment)
            - (Application Handling [chif::Core::Application])
            - (Job System)
            - (BackLog System)
        - Graphics::
            - Renderer::
                	- Shader (Class)
                    - Buffer (Class)
                    - etc.
            - Water (Class)
            - Terrain (Class)
            - Texture (Class)
            - VolumetricClouds (Class)
            - Light (Class)
            - VFX::
        - GUI::
            - Sprite (Class)
            - AnimatedSprite (Class)
        - Physics::
        - Utils::
        - Platform::
            - Window (Class)
        - Audio:: (Maybe in Paltform)


Anmerkung : ´´´Get-ChildItem -Recurse -Path .\src\ -Filter *.c | ForEach-Object { $_.FullName.Replace((Get-Location).Path + '\', '') }
 ´´´
- Porting from GLWF to SDL2
- Remake Architecture / Advanced Infrastructure
- Advanced Lighting
- Bloom , etc.
- Collision / Jolt
- Game ?
- ECS ?
- Switch
- Studio


## ECS System: 

#### Denoiser
To enable denoising for path traced images, you can use the [Open Image Denoise library](https://github.com/OpenImageDenoise/oidn). To enable this functionality, the engine will try to include the "WickedEngine/OpenImageDenoise/oidn.hpp" file. If this file could be included, it will attempt to link with OpenImageDenoise.lib and tbb.lib and any other lib that is supplied in the OpenImageDenoise release version The libs should be located in the `$(SolutionDir)BUILD\$(Platform)\$(Configuration)` path, which is for example: `C:/PROJECTS/WickedEngine/BUILD/x64/Release`. It is also required to provide the OpenImageDenoise.dll and tbb.dll and any other dll near the exe to correctly launch the application after this, which is for example: `C:\PROJECTS\WickedEngine\BUILD\x64\Release\Editor_Windows`. If you satisfy these steps, the denoiser will work automatically after you rebuild the engine and the path tracer reached the target sample count, or lightmap baking is stopped.

### LoadingScreen
[[Header]](../../WickedEngine/wiLoadingScreen.h) [[Cpp]](../../WickedEngine/wiLoadingScreen.cpp)
The LoadingScreen is an example RenderPath that can be used as a drop-in loading screen for your application. It is designed to be a simple 2D screen that is shown during resource loading. You can add generic tasks with `addLoadingFunction()`, or to load an other `RenderPath` using `addLoadingComponent()`. You can specify what will be called after loading is finished by adding a task to with `onFinished()`. You can query the progress of loading with the `getProgress()` function which returns the percentage of loading as an integer in the range 0-100. You can use it to display a loading progress bar for example. The loading will start when `Start()` is called for the loading screen, but you nedd not call this yourself usually. When you activate your loading screen with the `Application` class, it will switch to the loading screen when appropriate and start it automatically. With this you can achieve nice fade-in-out of loading screen easily, as the `Application`'s `ActivatePath()` simply has a parameter for fading duration.

Because the `LoadingScreen` is a `RenderPath2D`, it is safe to load a `Scene` directly into the main scene (with `LoadModel()`), because `RenderPath2D` doesn't utilize scene at all for rendering. If you use a custom loading screen that is also performing 3D rendering with a scene, you would instead load into a separate scene with `LoadModel()`, and merge into your main scene in the `LoadingScreen::onFinished()` callback.

A simple helper feature of this LoadingScreen class is the `BackgroundMode` setting. You can set a background texture easily for the LoadingScreen which will simply be shown as a full screen image during loading. For this, you should at least specify the `LoadingScreen::bacgroundTexture` resource, which you can simply load from an asset file with `wi::resourcemanager::Load()`. You can set the `BackgroundMode` of this image via `LoadingScreen::background_mode` with the following modes:
- Fill: fill the whole screen, will cut off parts of the image if aspects don't match (default)
- Fit: fit the image completely inside the screen, will result in black bars on screen if aspects don't match
- Stretch: fill the whole screen, and stretch the image if needed

If you want to render other things on the LoadingScreen, then you can also override RenderPath2D methods such as Render() or Compose() and implement custom rendering for yourself.

## System
You can find out more about the Entity-Component system and other engine-level systems under ENGINE/System filter in the solution.
### Entity-Component System
[[Header]](../../WickedEngine/wiECS.h)

#### ComponentManager
The Component Manager is responsible of binding data (component) to an Entity (identifier). The component can be a c++ struct that contains data for an entity. It supports serialization of this data, and if this is used, then the component structs must have a Serialize() function. Otherwise the component can be any c++ struct that can be moved.

#### Entity
Entity is an identifier (number) that can reference components through ComponentManager containers. An entity is always valid if it exists. It's not required that an entity has any components. An entity has a component, if there is a ComponentManager that has a component which is associated with the same entity.

#### Using the entity-component system
To use the entity-component system, you must use the ComponentManager<T> to store components of the T type, where T is an type of c++ struct. To bind a component to an entity, this procedure should be followed:

```cpp
struct MyComponent
{
	float3 position;
	float speed;
};
ComponentManager<MyComponent> components; // create a component manager
Entity entity = CreateEntity(); // create a new entity ID
MyComponent& component = components.Create(entity); // create a component and bind it to the entity
```

When you create a new component, it will be added to the end of the array, in a contiguous memory allocation. The ComponentManager can be indexed and iterated efficiently:

```cpp
for(size_t i = 0; i < components.GetCount(); ++i)
{
	MyComponent& component = components[i];
	Entity entity = components.GetEntity(i);
}
```

You can see that we can both iterate through components and entities this way by the same index. It is useful when you just want to go through everything one-by one. But it is not useful when you want to query a component for an entity but you don't know its index. Indices can change when components are removed, so you cannot usually rely on remembering indices for entities yourself. However, the component manager does manage this information internally. To query a component for a given entity, use the GetComponent() function:

```cpp
MyComponent* component = components.GetComponent(entity);
if(component != nullptr) // check for null, which can be returned if component doesn't exist for the entity
{
	// use component
}
```

You can also get the index of an entity in a component manager:

```cpp
size_t index = components.GetIndex(entity);
if(index != ~0ull) // check if it's valid. If invalid, then the index returned will be the max value of 64 bit uint
{
	MyComponent& component = components[index];
}
```

It is NOT valid to use the array operator with entity ID:

```cpp
// NOT VALID! result will be undefined (could be a valid component, or overindexing crash)
MyComponent& component = components[entity];
```

A useful pattern is to offload the iteration of a large component manager to the job system, to parallelize the work:

```cpp
wi::jobsystem::context ctx;
wi::jobsystem::Dispatch(ctx, (uint32_t)components.GetCount(), 64, [&](wi::jobsystem::JobArgs args){
	MyComponent& component = components[args.jobIndex];
	Entity entity = components.GetEntity(args.jobIndex);
	// Do things...
});
// optionally, do unrelated things here...
wi::jobsystem::Wait(ctx); // wait for Dispatch to finish
```
In this example, each entity/component will be iterated by one job, and 64 batch of jobs will be executed on one thread as a group. This can greatly improve performance when iterating a large component manager.

One thing that you must look out for is pointer invalidation of the Component Manager. It is always safe to use Entity IDs whenever, but it is faster to use a pointer or an index. Those can, hovewer change as components are added or removed. You must look out for index and pointer invalidation when components are removed, because the ordering of the components can change there (the component manager is always kept dense, without holes). When adding/creating components, pointers can occasionally get invalidated the same way as for example std::vector, because reallocation of memory can happen to keep everyting contiguous. This can lead to unexpected consequences:

```cpp
MyComponent& component1 = components[0];
MyComponent& component2 = components.Create(CreateEntity()); // here the component1 reference/pointer can get invalidated
component1.speed = 10; // POINTER INVALIDATION HAZARD!
```
In the example above, a new component is created while the reference to the first element is still going to be used, this can lead to crash if the memory got reallocated while creating the component2. The correct solution would be to first create all components, then use them after all creations finished. Or re-query component pointers with GetComponent(entity) after a creation happens.

The Scene structure of the engine is essentially a collection of ComponentManagers, which let you access everything in the scene in the same manner.

### Scene System
[[Header]](../../WickedEngine/wiScene.h) [[Cpp]](../../WickedEngine/wiScene.cpp)
The logical scene representation using the Entity-Component System
- GetScene <br/>
Returns a global scene instance. This is a convenience feature for simple applications that need a single scene. The RenderPath3D will use the global scene by default, but it can be set to a custom scene if needed.
- LoadModel() <br/>
There are two flavours to this. One of them immediately loads into the global scene. The other loads into a custom scene, which is useful to manage the contents separately. This function will return an Entity that represents the root transform of the scene - if the attached parameter was true, otherwise it will return INVALID_ENTITY and no root transform will be created.
- LoadModel2() <br/>
This is an alternative usage of LoadModel, which lets you give the root entity ID as a parameter. Apart from that, it works the same way as LoadModel(), just that attachments will be made to your specified root entity.
- Intersects(Ray/Capsule/Sphere, filterMask, layerMask, lod) <br/>
Intersection function with scene entities and primitives. You can specify various settings to filter intersections. The filterMask lets you specify an engine-defined type enum bitmask combination, so you can choose to intersect with objects, and/or colliders, and various specifications. The layerMask lets you filter the intersections with layer bits, where binary OR of the parameter and entity layers will decide active entities. The lod parameter lets you force a lod level for meshes. 
- Pick <br/>
Allows to pick the closest object with a RAY (closest ray intersection hit to the ray origin). The user can provide a custom scene or layermask to filter the objects to be checked.
- SceneIntersectSphere <br/>
Performs sphere intersection with all objects and returns the first occured intersection immediately. The result contains the incident normal and penetration depth and the contact object entity ID.
- SceneIntersectCapsule <br/>
Performs capsule intersection with all objects and returns the first occured intersection immediately. The result contains the incident normal and penetration depth and the contact object entity ID.

Below you will find the structures that make up the scene. These are intended to be simple structures that will be held in [ComponentManagers](#componentmanager). Keep these structures minimal in size to use cache efficiently when iterating a large amount of components.

<b>Note on bools: </b> using bool in C++ structures is inefficient, because they take up more space in memory than required. Instead, bitmasks will be used in every component that can store up to 32 bool values in each bit. This also makes it easier to add bool flags and not having to worry about serializing them, because the bitfields themselves are already serialized (but the order of flags must never change without handling possible side effects with serialization versioning!). C++ enums are used in the code to manage these bool flags, and the bitmask storing these is always called `uint32_t _flags;` For example: