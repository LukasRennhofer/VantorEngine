# TODOS of CHIFEngine future updates

- New Folder Structure:
    ```
    └── src
    ├── external           # External libraries (third-party)
    │   ├── imgui
    │   ├── assimp
    │   ├── glad
    │   ├── glm
    │   ├── glfw
    │   ├── freetype
    │   └── stb_image
    │
    ├── core              # Core engine systems (Initialization, Main Engine)
    │   ├── Engine        # Engine main loop, entry point
    │   ├── Compile       # Compilation helpers, platform-specific
    │   └── Config        # Config loader, settings management
    │
    ├── rendering         # Rendering-related components
    │   ├── Renderer      # Main rendering pipeline
    │   ├── Shaders       # Shader management
    │   ├── Textures      # Texture loading & handling
    │   ├── Materials     # Material definitions
    │   ├── Lighting      # Lighting systems (shadow, reflections)
    │   ├── Camera        # Camera & perspective
    │   └── DrawableObjects # Models, meshes, sprites
    │
    ├── physics           # Physics-related components
    │   ├── Collision     # Collision detection
    │   ├── RigidBody     # Rigid body physics
    │   └── SoftBody      # Cloth, fluid, and other deformable objects
    │
    ├── weather           # Weather simulation (skybox, rain, fog)
    │   ├── Atmosphere    # Skybox, clouds
    │   ├── Precipitation # Rain, snow, particles
    │   └── Wind          # Wind effects
    │
    ├── gui               # GUI System (UI components, ImGui integration)
    │   ├── UI            # Custom UI components
    │   ├── Windows       # Game windows (menus, overlays)
    │   └── HUD           # In-game HUD elements
    │
    ├── input             # Input handling (keyboard, mouse, gamepad)
    │   ├── KeyBindings   # Key-mapping system
    │   ├── Mouse         # Mouse handling
    │   └── Controller    # Gamepad support
    │
    ├── sound             # Sound system (audio engine)
    │   ├── Music         # Background music
    │   ├── SFX           # Sound effects
    │   └── AudioEngine   # OpenAL/SDL_mixer or other engine
    │
    ├── networking        # Networking (multiplayer, server-client)
    │   ├── Server        # Server-side code
    │   ├── Client        # Client-side code
    │   ├── Netcode       # Networking logic
    │   └── Replication   # Data synchronization
    │
    ├── 
    ```

- Get-ChildItem -Recurse -Path .\src\ -Filter *.c | ForEach-Object { $_.FullName.Replace((Get-Location).Path + '\', '') }
