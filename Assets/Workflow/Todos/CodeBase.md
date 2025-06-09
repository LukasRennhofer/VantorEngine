# Vantor CodeBase Todo Stack

## => 0.21.0

 - Main Architecture
    - Should use more Interface Classes with virtual functions like (IRenderDevice)
    - Vantor Folder with Includes, Source, Documentation, External Dependencies, Configs (maybe another name) and Resources and Shaders and Studio Applications
    - Module System like Blender has (VLIB, VGFX, VCORE)
    - All External Dependenices via Github links and forks
    - Root build system integration
    - Maybe a Benchmarking system like testing too?

| Prefix | Meaning                     | Example                            |
| ------ | --------------------------- | ---------------------------------- |
| `V`    | General engine struct/class | `VVector3`, `VColor`, `VTransform` |
| `VA`   | Actor type (optional)       | `VAEntity`, `VAPlayer`             |
| `VO`   | UObject-style base class    | `VUAsset`, `VUSceneObject`         |
| `VI`   | Interfaces                  | `VIComponent`, `VIUpdatable`       |
| `VT`   | Template types              | `VTArray`, `VTMap`                 |
| `VE`   | Enums                       | `VEAxis`, `VERenderMode`           |
