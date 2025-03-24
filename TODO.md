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
