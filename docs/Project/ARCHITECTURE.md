# TODO: CHIFEngine Architecture docs

## Graphics Architecture:
```
graphics/
├── renderer/
│   ├── Shader.hpp          # Abstract shader interface
│   ├── Texture.hpp         # Abstract texture interface (same idea)
│   └── ...
├── renderdevice/
│   ├── RenderFactory.hpp
│   ├── RenderFactory.cpp
│   ├── opengl/
│   │   ├── GLShader.hpp
│   │   ├── GLShader.cpp
│   │   └── ...
│   └── vulkan/
│       └── VKShader.hpp / cpp (TODO: Vulkan Support)
```