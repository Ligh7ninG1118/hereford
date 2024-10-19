# Hereford
Hereford is an FPS demo planned to recreate the CQB test from the F.N.G. level of *Call of Duty 4: Modern Warfare*. 
The demo is developed using a simple game engine made from scratch, with **SDL2** and **OpenGL**. 

The project's main purpose is for me to learn more about game engine development, modern C++ and features
(Also, to pay homeage to the great game that inspired me to become a game developer)

>Note: Due to different license requirements, assets used aren't included in this repo

## Progress Showcase (Updated 10/04/2024)
[![Hereford Progress Showcase](http://img.youtube.com/vi/3NIVXB-3Icc/0.jpg)](https://youtu.be/3NIVXB-3Icc)

## Features Developed
or at least, "it's working!"
### Engine
- Windows Context Handling
- Asset Loading and Management (Textures, Models, Shaders)
- Math Library (Vector, Matrix, Quaternion)
- Actor and Component Structure
- Event
- Delayed Action
- Timeline Action
- Reflection
- Scene Content (JSON)

### Gameplay
- First Person Camera and Control
- Animation State Machine
- Gameplay Actions
- Movement States (Crouch, Sprint)
- Weapon
- Object Interaction

### Phyiscs
- Raycast Query (Against Sphere, AABB)

### Visual
- Basic Forward Rendering
- PBR Shading
- Skybox
- UI and Text Rendering
- Skeletal Animation

### Audio
- Sound Effects

## Planned Features
### Engine
- Lua Scripting
- Scripted Level Sequence

### Gameplay
- Movement States (Prone, Ladder Climbing, Rapple Down)
- Gadgets

### Physics
- Gravity
- Collision Response
- Collision Detection (Broad/Narrow Phase)

### Visual
- Particle Effect
- Decal Rendering

## Libraries Used
- SDL2
- OpenGL 4.6
- Assimp
- stb_image
- Freetype
- JSON for Modern C++
- ImGUI

## Assets Used
#### BarcodeGames (https://sketchfab.com/barcodegames)
- Mark 23 (Mesh and Animations)
- MP5SD (Mesh and Animations)

#### Tauffiq Abdllah (https://sketchfab.com/3d-models/shooting-range-target-7ab89185712c4d5c97f99fb1f5b2d530)
- Shooting Range Target

#### 3D Content Online (https://sketchfab.com/3d-models/pine-and-ply-site-3-401b1b4d9d3e4ece9862d3719ae0db7d)
- Plywood Walls

#### Mixamo (https://www.mixamo.com)
- SWAT Guy (Mesh)
- Silly Dancing (Animation)

#### Soundsnap (https://www.soundsnap.com/)
- Sound Effects

#### Free PBR (https://freepbr.com/)
- PBR Textures

#### rpgwhitelock (https://assetstore.unity.com/packages/2d/textures-materials/sky/allsky-220-sky-skybox-set-10109)
- Skyboxes

## References
- LearnOpenGL by Joey De Vries (https://learnopengl.com/)
- ITP 380 (USC) by Sanjay Madhav (https://itp380.org/)
- 3D Collisions by Gabor Szauer (https://gdbooks.gitbooks.io/3dcollisions/content/)

