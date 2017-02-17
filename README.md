# Attempting the Entity Component System Architectural Pattern in C

This project had several important aspects;

* Learning GLSL
* Understanding the ECS game architecture
* Deepening my knowledge of physics

The goal was to implement a rendering pipeline (using OpenGL), a linear algebra library (specifically for vector and matrix operations), a generalized (though not portable) device input package and a physics simulation.

**Features:**

* Linear algebra library for vector-vector and matrix-vector operations etc.
* File packing and unpacking
* 3DS file loader (standardized CAD/3D model format)
* Rigid body physics (using fourth-order Runge—Kutta integration)
* GLSL shaders (compiled and run on the GPU)
  * Material shader (Phong shading model)
  * Noise/fake-ISO shader
  * Motion blur shader (Custom implementation)
  * Exposure shader (Solvent based)
* ECS architecture

Everything was implemented in C, although I did not follow the C89 standard, but rather C99 and even some C11 features (by extension since the MSVC++ compiler is not quite there yet).

### Materials!
<img src="sa14-game1/images/game1.png" alt="" />

Notice the use of ADS (ambient-diffuse-specular) material shaders, a (barely noticeable) noise shader, a motion blur shader as well as an exposure shader for mor realistic colors!

### Textures!
<img src="sa14-game1/images/game0.png" alt="" />

