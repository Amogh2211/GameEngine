# GameEngine
A 2D Game Engine designed using OpenGL as the primary rendering APIs, which runs only on Windows at the moment. Cross platform functionality will be added in the future.

## Getting Started
Visual studio 2017 or newer is recommended to run this engine.
###1. Downloading the repository
Clone the repository with `git clone --https:://github.com/Amogh2211/GameEngine`.

If repo was cloned non recursively before, use `git submodule update --init` to clone the necessary submodules.

###2. Pre requisites

One of the main pre requisites is Premake5. This creates the visual studio solution for you to run the project. 
Run the [GenerateProjects.bat](https://github.com/Amogh2211/GameEngine/blob/main/scripts/GenerateProjects.bat) file to run Premake.

### Main Features of the Current State of the Engine
- Setup a functioning 2D Renderer, which you can make games with (Full 3D Rendering pipeline to come)
- Currently only supports Windows

## Features to come
- Set up a proper GUI to set shaders, textures etc. Currently everything has to be hardcoded, or given through a file.
- Audio system
- Fast  2D rendering (for UI, sprites etc)

### See also
[Ray Tracer Project](https://github.com/Amogh2211/raytracing)
