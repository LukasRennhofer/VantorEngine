<a id="readme-top"></a>

<!-- Standard Project Badges -->
[![Issues](https://img.shields.io/github/issues/LukasRennhofer/Vantor.svg?style=for-the-badge)](https://github.com/LukasRennhofer/Vantor/issues)
[![License](https://img.shields.io/github/license/LukasRennhofer/Vantor.svg?style=for-the-badge)](https://github.com/LukasRennhofer/Vantor/blob/main/LICENSE)

<!-- Extra Badges for Features -->
[![Build Status](https://img.shields.io/github/actions/workflow/status/LukasRennhofer/Vantor/build.yml?branch=main&style=for-the-badge)](https://github.com/LukasRennhofer/Vantor/actions)
[![Code Quality](https://img.shields.io/codefactor/grade/github/LukasRennhofer/Vantor?style=for-the-badge)](https://www.codefactor.io/repository/github/LukasRennhofer/Vantor)
[![Last Commit](https://img.shields.io/github/last-commit/LukasRennhofer/Vantor?style=for-the-badge)](https://github.com/LukasRennhofer/Vantor/commits)

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/LukasRennhofer/Vantor">
    <img src="config/assets/VantorLogo_main_wbg.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">Vantor</h3>

  <p align="center">
    The Cross-Platform Game and Rendering Engine
    <br />
    <a href="https://github.com/LukasRennhofer/Vantor"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://vantor.netlify.app/">View Website</a>
    &middot;
    <a href="https://github.com/LukasRennhofer/Vantor/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/LukasRennhofer/Vantor/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project

<p align="center">
<img src="https://raw.githubusercontent.com/LukasRennhofer/Vantor/refs/heads/main/config/assets/gif1.gif" alt="GIF Show Background" style="">
</p>

Vantor is a modular, cross-platform game and rendering engine designed for **large-scale Open-World games** and is a code-focused development SDK and Framework. Built from the ground up with **OpenGL** and custom rendering techniques, it provides fine-grained control over every aspect of game development while maintaining **maximum efficiency**. Unlike high-level game engines, the engine is designed to be **lightweight**, **modular**, and **fully extensible**, allowing developers to directly manipulate rendering, physics, and resource management.

> *"Open source is about collaborating; not competing." – **Kelsey Hightower**, technologist at google and open-source advocate*

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

[![C++][C++-badge]][C++-url] [![OpenGL][OpenGL-badge]][OpenGL-url] [![Vulkan][Vulkan-badge]][Vulkan-url] [![GLSL][GLSL-badge]][GLSL-url] [![Python][Python-badge]][Python-url] [![Dear ImGui][ImGui-badge]][ImGui-url] [![SDL2][SDL2-badge]][SDL2-url] [![CMake][CMake-badge]][CMake-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple example steps.

### Prerequisites

* CMake
* Make
* A C++20 capable compiler (MSVC, Clang, GCC)
* Git
* Python Interpreter

### Building

#### Windows

0. Install Mingw Msys x64 from [their website](https://www.msys2.org/) and install python from [its website](https://www.python.org/).

---

1. Clone the repository:

   ```bash
   git clone https://github.com/LukasRennhofer/Vantor.git
   cd Vantor
   ```

---

2. Install the Dependencies with Msys2:

   ```bash
      pacman -S cmake mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut mingw-w64-x86_64-assimp -S mingw-w64-x86_64-sdl2
   ```

---

3. Build the libary with the CHIFDevConsole Script
  ```bash
    python DevConsole.py --platform Windows --build-lib
   ```

#### Linux

- **Status**: Not currently supported. Building on Linux may lead to errors due to limited development resources. (Tested on Ubuntu 24.10)

---

0. Install Python: 
   ```bash
   sudo apt-get install python3
   ```

0. Clone the repository:

   ```bash
   git clone https://github.com/LukasRennhofer/Vantor.git
   cd Vantor
   ```

---

1. Install the Dependencies:

   ```bash
   sudo apt-get install build-essential libsdl2-dev libsdl2-2.0-0 libassimp-dev cmake libglm-dev libglew-dev
   ```

---

2. Build the library from source:

   ```bash
   python3 DevConsole.py --platform Linux --build-lib
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

Once built, you can use the sample applications, build custom modules, or integrate Vantor in your own project. The architecture is designed to support real-time rendering and simulation-heavy applications.

_For more examples, please refer to the [Documentation](https://vantor.netlify.app/)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->
## Roadmap

- [🟩] Platform Abstraction Layer (Windows/Linux/Switch)
- [🟥] Editor Interface
- [🟧] Multi-threaded Resource Management
- [🟧] GPU-based Scene Graph
- [🟥] Full Vulkan Support
- [🟥] Documentation Expansion

See the [open issues](https://github.com/LukasRennhofer/Vantor/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**, but will not affect the internal studios engine.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/LukasRennhofer/Vantor/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=LukasRennhofer/Vantor" alt="contrib.rocks image" />
</a>

<!-- LICENSE -->
## License

Distributed under the GNU General Public License, v3. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Lukas Rennhofer - [@LukasRennhofer](https://twitter.com/LukasRennhofer) - lukas.renn@aon.at

Project Link: [https://github.com/LukasRennhofer/Vantor](https://github.com/LukasRennhofer/Vantor)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* Vantor Studios
* Inspiration from [WickedEngine](https://github.com/turanszkij/WickedEngine)
* All contributors of external libraries
* The open source community

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/LukasRennhofer/Vantor.svg?style=for-the-badge
[contributors-url]: https://github.com/LukasRennhofer/Vantor/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/LukasRennhofer/Vantor.svg?style=for-the-badge
[forks-url]: https://github.com/LukasRennhofer/Vantor/network/members
[stars-shield]: https://img.shields.io/github/stars/LukasRennhofer/Vantor.svg?style=for-the-badge
[stars-url]: https://github.com/LukasRennhofer/Vantor/stargazers
[issues-shield]: https://img.shields.io/github/issues/LukasRennhofer/Vantor.svg?style=for-the-badge
[issues-url]: https://github.com/LukasRennhofer/Vantor/issues
[license-shield]: https://img.shields.io/github/license/LukasRennhofer/Vantor.svg?style=for-the-badge
[license-url]: https://github.com/LukasRennhofer/Vantor/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/lukas-rennhofer
[C++-badge]: https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white
[C++-url]: https://isocpp.org/
[OpenGL-badge]: https://img.shields.io/badge/OpenGL-5586A4?style=for-the-badge&logo=opengl&logoColor=white
[OpenGL-url]: https://www.opengl.org/
[Vulkan-badge]: https://img.shields.io/badge/Vulkan-AC162C?style=for-the-badge&logo=vulkan&logoColor=white
[Vulkan-url]: https://www.khronos.org/vulkan/
[GLSL-badge]: https://img.shields.io/badge/GLSL-FF6C00?style=for-the-badge&logo=opengl&logoColor=white
[GLSL-url]: https://www.khronos.org/opengl/wiki/OpenGL_Shading_Language
[Python-badge]: https://img.shields.io/badge/Python-3670A0?style=for-the-badge&logo=python&logoColor=white
[Python-url]: https://www.python.org/
[ImGui-badge]: https://img.shields.io/badge/ImGui-9C27B0?style=for-the-badge
[ImGui-url]: https://github.com/ocornut/imgui
[Raylib-badge]: https://img.shields.io/badge/Raylib-000000?style=for-the-badge
[Raylib-url]: https://www.raylib.com/
[SDL2-badge]: https://img.shields.io/badge/SDL2-FF6C00?style=for-the-badge
[SDL2-url]: https://www.libsdl.org/
[CMake-badge]: https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white
[CMake-url]: https://cmake.org/