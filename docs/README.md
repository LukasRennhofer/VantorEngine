<p align="center">
  <a href="https://voronoid.tech/">
    <img src="https://raw.githubusercontent.com/LukasRennhofer/CHIFEngine/refs/heads/main/config/assets/CHIFEngine_Logo_2_wbg.png" width="200" alt="CHIFEngine logo">
  </a>
</p>
<p align="center">
<img src="https://raw.githubusercontent.com/LukasRennhofer/CHIFEngine/refs/heads/main/config/assets/gif1.gif" alt="GIF Show Background" style="">
</p>

<p align="center">
  <img alt="GitHub Release" src="https://img.shields.io/github/v/release/LukasRennhofer/CHIFEngine">
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/LukasRennhofer/CHIFEngine">
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/t/LukasRennhofer/CHIFEngine">
  <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/LukasRennhofer/CHIFEngine">
  <img alt="GitHub Downloads (all assets, all releases)" src="https://img.shields.io/github/downloads/LukasRennhofer/CHIFEngine/total">
  
</p>

<br>

<p align="center">
  <img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/LukasRennhofer">
  <img alt="GitHub Issues or Pull Requests" src="https://img.shields.io/github/issues/LukasRennhofer/CHIFEngine">
  <img alt="Website" src="https://img.shields.io/website?url=https%3A%2F%2Fvoronoid.tech%2F">
</p>

---

## About CHIFEngine

[CHIFEngine](https://github.com/LukasRennhofer/CHIFEngine) is a low-level, high-performance game engine designed for **large-scale Open-World games**. Built from the ground up with **OpenGL** and custom rendering techniques, it provides fine-grained control over every aspect of game development while maintaining **maximum efficiency**. Unlike high-level game engines, the engine is designed to be **lightweight**, **modular**, and **fully extensible**, allowing developers to directly manipulate rendering, physics, and memory management **without unnecessary abstractions**.

---

## Why CHIFEngine?

- **Free and Open Source**: CHIFEngine is licensed under the MIT License, allowing complete freedom with no royalties or hidden fees, just the same License.
- **Hobby Project for Open-World Game**: As a hobby project, **CHIFEngine** was created with a passion for building a custom low-level game engine to support large-scale open-world games. While it may not offer the conveniences of commercial engines, it provides an invaluable opportunity for hands-on learning, deep customization, and experimenting with game engine design.

> *"Open source is about collaborating; not competing." – **Kelsey Hightower**, technologist at google and open-source advocate*

---

## Getting Started

### Download
You can download the latest binaries of CHIFEngine from:

- The [Releases Tab](https://github.com/LukasRennhofer/CHIFEngine/releases)

> **Note**: Currently, CHIFEngine supports only Windows x64 and Nintendo Switch (Homebrew) systems.

### Build Instructions

#### Windows

1. Clone the repository:

   ```bash
   git clone https://github.com/LukasRennhofer/CHIFEngine.git
   ```

2. Install the Dependencies

   ```bash
      CMake: ^3.27.0

      OpenGL: ^4.6

      GLFW: ^3.3.8

      GLAD: ^2.2.0

      GLM: ^0.9.9.8

      Assimp: ^5.2.5
   ```

3. Build it with the Windows BAT File
  ```bash
    ./buildWindows.bat
   ```

#### Linux

- **Status**: Not currently supported. Building on Linux may lead to errors due to limited development resources.
  - Needs Dependencies of the ```CMakeLists.txt``` file and can be build with ```./buildLinux.sh```, but not supported⚠️

---

## Features

- **Cross-Platform Support** – Runs on PC and supports Nintendo Switch Homebrew, making it suitable for both modern and embedded hardware
- **Minimal External Dependencies** – Designed to be self-contained, avoiding reliance on bulky third-party libraries while giving developers direct access to OpenGL, Vulkan (experimental), and custom math libraries.
- **Custom OpenGL (Vulkan not yet) Rendering Pipeline** – Uses modern graphics techniques such as deferred shading, GPU-driven culling, and physically-based rendering (PBR) in the future to handle complex environments efficiently.
---

## Documentation & Demos

- **Documentation**: *Currently in progress.* Stay tuned for updates.
- **Demos**: *Currently in progress.* Stay tuned for updates.

---

## Roadmap & Todos

Check out our [Todo List](Todo.md) for upcoming features and tasks.

---
## Q&A

 - "When will the public-use version be published?"
      - When its ready.
 - "Why does the Project takes so long to be complete?"
      - Rome wasn't built in a day.
 - "How can I simply make games with it?"
      - The Engine is specified for complex open-world games only.
 - "Will there be a build for MacOS?"
      - Yeah, right after Apple learns to not be a sandbox for overpriced toys.

## Contributors

We extend our gratitude to the following individuals for their efforts and contributions:

| Name                      | GitHub Handle   | Contributions                     | 
| ------------------------- | --------------- | --------------------------------- |
| **Lukas Rennhofer**       | @LukasRennhofer     | **Maintainer**, Engine Logic, Physics |
| **Moritz Rottensteiner**  | @Alyopolo       | 3D Assets and Game Development    |

---

## Stay Connected

- Visit our [Website](https://voronoid.tech/).
- Give us a star on [GitHub](https://github.com/LukasRennhofer/CHIFEngine) for the latest updates.

---