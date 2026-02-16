# 🌋 Vulcan Rainbow Engine

A minimalist 2D graphics engine built with **Vulkan API**. This project features a rotating triangle with dynamic color interpolation and real-time updates via Push Constants.

![Vulkan](https://img.shields.io/badge/Vulkan-1.3-red.svg?style=for-the-badge&logo=vulkan)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg?style=for-the-badge&logo=c%2B%2B)

----

## 🚀 Features
- **Vulkan 1.3** graphics pipeline implementation.
- **Push Constants** for high-performance rotation and color animation.
- **GLSL Shaders** with SPIR-V compilation.
- **Modular Code** (Window, Device, Pipeline, and Model abstractions).



## 🛠 Tech Stack
- **Language:** C++17
- **Platform:** Fedora Linux
- **Dependencies:** GLFW, GLM, Vulkan SDK


## 📦 Quick Start
### 1. Install Dependencies(Fedora)
```bash
sudo dnf install vulkan-loader-devel glfw-devel glm-devel glslang
```
### 2. Compile Shaders
```bash
glslangValidator -V shaders/shader.vert -o shaders/vert.spv
glslangValidator -V shaders/shader.frag -o shaders/frag.spv
```
### 3. Build and run
```bash
mkdir build && cd build
cmake ..
make && ./engine
```

## 📂 Project Structure
- **src/** - Engine source code.
- **shaders/** - GLSL vertex and fragment shaders.
- **extenal/** - Third-party libraries(ImGui).
