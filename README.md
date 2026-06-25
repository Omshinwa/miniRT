*This project has been created as part of the 42 curriculum by dasamuel and wiwu.*

# miniRT — A Ray Tracer in C

// here is the content section

## Description

**miniRT** is a raytracer written in C, built as part of the 42 school curriculum. The goal of the project is to implement the fundamental principles of ray tracing from scratch: casting rays from a camera into a 3D scene, computing intersections with geometric primitives, and simulating the interaction of light with surfaces.

The program parses a `.rt` scene file, renders the resulting image in a window using **MiniLibX**.

// features

# Features

// features

https://github.com/user-attachments/assets/be7b838a-5d68-479e-bbc8-d2b2d9043ebf




## Features



* Able to render basic 3D shapes (sphere, plane, cylinder, cone).
* Phong reflection model (Ambient, diffuse lighting, specular highlights)
* Hard shadows
* Multi colored lights
* Interactively move the objects and camera in the 3D space
* Texture map and Bump map (with XPM image files)


move the camera in space
select an object, move it in space
toggle textures etc

 and supports the following mandatory features:

- **Geometric primitives**: sphere, plane, cylinder
- **Lighting model**: ambient light, diffuse (Lambertian) shading, hard shadows
- **Camera**: configurable position, orientation, and field of view
- **Scene description**: parsed from a `.rt` text file

### Bonus Features

- Specular highlights (Phong reflection model)
- Checkerboard texture pattern (UV-mapped on all primitives)
- Bump mapping / normal perturbation using XPM texture maps
- Additional primitives: **cone**, **disk**, **hyperboloid** *(optional)*
- Multi-light support
- Colored lights

---

## Installation

### Requirements

- **OS**: Linux or macOS
- **Compiler**: `gcc` or `clang`
- **Make**
- **MiniLibX**: a graphic library, it is included here

### Compilation

```bash
# Clone the repository
git clone https://github.com/Omshinwa/miniRT.git miniRT
cd miniRT

# Build
make
```

### Execution

```bash
./miniRT <FILEPATH>
```

## Configuration File

The **miniRT** programs takes as argument a text file with the extension `.rt`. This file is the scene descriptor. It is must be made up of several lines where each line describe an object formatted as such:

`<Identifier> <Fields>`

The different elements are:

| Identifier | Object           | Fields in order										| Must be Unique? |
|------------|-------------------|------------------------------------------------------|---------|
| `C`        | Camera            | Position, Orientation, FOV							| Yes     |
| `A`        | Ambient light     | Brightness, Color									| Yes     |
| `L`        | Point light       | Position, Brightness, Color							| No      |
| `sp`       | Sphere            | Position, Diameter, Surface				 			| No      |
| `pl`       | Plane             | Position, Orientation, Surface						| No      |
| `cy`       | Cylinder          | Position, Orientation, Diameter, Height, Surface		| No      |
| `co`       | Cone              | Position, Orientation, Opening angle, Height, Surface| No      |

### Field definitions:

| Field | Definition |
| ----- | ---------- |
| **Brightness** | Float number in [0,1] |
| **Color** | 3 integers in [0-255] |
| **Position** | 3 Float numbers separated by a comma `,` eg: `0.2,-45,788789` |
| **Orientation** | 3 Float numbers in [-1,1] separated by a comma `,` represents a 3D vector with a norm of 1. Y axis is up. |
| **FOV** | An integer in [0,180] |
| **Diameter, Height, Opening angle** | A single float number, note that the opening angle is in radians. |

### Surface

For Shapes (sphere, plane cylinder, cone), the last field describes the appareance of their surface. It can be made up of up to three arguments:

**`<Color/Texture Map> [Checkerboard Overlay] [Texture Bump]`**

- `<Color/Texture Map>`: Mandatory, it's either a **Color** (see above). Or a **Texture**, in this case it must be a path to a **.XPM** file.

- `[Checkerboard Overlay?]`: You can optionally overlay a checkerboard pattern as a texture. To do so, write the **`check`** keyword, otherwise leave it empty.

- `[Texture Bump]`: Finally, you can add a bump map, it must be a path to a **.XPM** file.

### Example

`pl  15,0,0    0,1,0    scenes/earth.xpm     check    scenes/moon.xpm`

This will create a Plane object, at position (15,0,0), oriented up, with the texture `scenes/earth.xpm`, have a checkerboard overlay and a bump map of `scenes/moon.xpm`.

### Example Scene File

```
A  0.2  255,255,255

C  0,0,-5  0,0,1  70

L  -2,5,-3  0.8  255,255,255

sp  0,0,0  2  255,0,0
pl  0,-2,0  0,1,0  100,200,100
cy  1,0,3  0,1,0  1  3  0,0,255
```

## Controls

| Key           | Action             									|
|---------------|-------------------------------------------------------|
| `ESC` key		| Close the window   									|
| Arrow keys    | Move the camera (or the active object)				|
| Numpad keys   | Toggle on or off some of the rendering pipeline 		|
| `WASDQE` keys | Rotate the camera (or the active object, if possible)	|
| `+-` keys		| Change the camera's Field Of View						|

---

## Technical Choices

- **global variables** Used for parsing (they are const static, which is compliant with the Norm), they connect each field of an object with how they should be formatted in the file. This allow for a single parsing function `do_instruction` that handles all entities uniformly instead of writing one parser per object type.
- **Norminette compliant**: functions ≤ 25 lines, files ≤ 5 functions, lines ≤ 80 chars
- **Vector math** implemented from scratch: dot product, cross product, normalization, reflection
- **UV mapping** computed analytically per primitive (sphere: spherical, cylinder/cone: cylindrical, plane: planar)
- **Bump mapping** uses the TBN frame (Tangent/Bitangent/Normal) and central-difference gradients sampled from the XPM heightmap

---

## Resources

### Ray Tracing & 3D Graphics

- [_Ray Tracing in One Weekend_ — Peter Shirley (free online book)](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Scratchapixel — Ray-Sphere, Ray-Cylinder, Ray-Cone intersection math](https://www.scratchapixel.com/)
- [Wikipedia — Phong reflection model](https://en.wikipedia.org/wiki/Phong_reflection_model)
- [Wikipedia — UV mapping](https://en.wikipedia.org/wiki/UV_mapping)
- [Wikipedia — Bump mapping](https://en.wikipedia.org/wiki/Bump_mapping)
- [Inigo Quilez — Implicit surface ray intersections](https://iquilezles.org/articles/)
- [Sebastian Lague — Coding Adventure: Ray Tracing](https://www.youtube.com/watch?v=Qz0KTGYJtUk)

### MiniLibX

- [MiniLibX Linux source — 42Paris/minilibx-linux](https://github.com/42Paris/minilibx-linux)

### Mathematics

- [3Blue1Brown — Essence of Linear Algebra (YouTube)](https://www.youtube.com/playlist?list=PLZHQObOWTQDPD3MizzM2xVFitgF8hE_ab)
- [Khan Academy — Dot product and cross product](https://www.khanacademy.org/math/linear-algebra)
- [Yvan Monka — LE COURS : Produit scalaire - Première](https://www.youtube.com/watch?v=dII7myZuLvo)

## Ideas for improvements

### Bounding Volume Hierarchy (BVH)
    Objects are put in boxes. These boxes are organized into a tree. When a ray comes in, you test the cheap box first; if it misses the box, you skip the expensive triangle/sphere intersection tests for everything inside it. The hierarchy lets you cull huge chunks of the scene.

### Monte Carlo light sampling (progressive convergence)
    On each frame, every point is lit by a single randomly chosen light rather than every light. The result is accumulated — summed across frames and divided by the frame count. This average converges to the correct render image while the framerate is still as smooth as a single-light scene.

    Note that each light's contribution is multiplied by N (the number of lights) to offset the monte carlo (each light only has a 1/N chance of being picked).
