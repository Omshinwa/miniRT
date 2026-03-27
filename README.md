*This project has been created as part of the 42 curriculum by dasamuel and wiwu.*

---

# miniRT — A Ray Tracer in C

![42 Badge](https://img.shields.io/badge/42-miniRT-blue)
![Language](https://img.shields.io/badge/language-C-lightgrey)
![Score](https://img.shields.io/badge/score-125%2F100-brightgreen)

---

## Description

**miniRT** is a ray tracer written in C, built as part of the 42 school curriculum. The goal of the project is to implement the fundamental principles of ray tracing from scratch: casting rays from a camera into a 3D scene, computing intersections with geometric primitives, and simulating the interaction of light with surfaces.

The program parses a `.rt` scene file, renders the resulting image in a window using **MiniLibX**, and supports the following mandatory features:

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

## Instructions

### Requirements

- **OS**: Linux or macOS
- **Compiler**: `gcc` or `clang`
- **MiniLibX**: included or installed separately depending on your system
- **Make**

### Compilation

```bash
# Clone the repository
git clone git@vogsphere.42paris.fr:vogsphere/intra-uuid-3bcc727e-6788-4b4a-8c4a-f7668893ac81-7220980-wiwu miniRT
cd miniRT

# Build
make
```

### Execution

```bash
./miniRT scenes/<filename>.rt
```

The `.rt` file describes the scene using identifiers. Each line defines one element:

| Identifier | Element           | Fields in order										|
|------------|-------------------|------------------------------------------------------|
| `A`        | Ambient light     | Brightness, Color									|
| `C`        | Camera            | Position, Orientation, FOV							|
| `L`        | Point light       | Position, Brightness, Color							|
| `sp`       | Sphere            | Position, Diameter, Color				 			|
| `pl`       | Plane             | Position, Orientation, Color							|
| `cy`       | Cylinder          | Position, Orientation, Diameter, Height, Color		|
| `co`       | Cone *(bonus)*    | Position, Orientation, Opening angle, Height, Color	|

Field definitions:
Brightness: number in [0,1]
Color: 3 integers in [0-255]
Position: 3 numbers separated by a comma `,` eg: `0.2,-45,788789`
Orientation: 3 numbers in [-1,1] separated by a comma `,` represents a 3D vector with a norm of 1
FOV: integer in [0,180]
Diameter, Height, Opening angle: single number, note that the opening angle is in radians.

(optional)
For Shapes, they can have a Texture instead of the Color field.
They can also optionally have a bump map Texture as the last field.
A Texture is a file path to a .xpm file.
A key word `check` can be put between the Color/Texture and the Bump map texture of the
shape, this will have the object be displayed in checkerboard.
eg: `pl  15,0,0    0,1,0    scenes/earth.xpm     check    scenes/moon.xpm`

### Example Scene File

```
A  0.2  255,255,255

C  0,0,-5  0,0,1  70

L  -2,5,-3  0.8  255,255,255

sp  0,0,0  2  255,0,0
pl  0,-2,0  0,1,0  100,200,100
cy  1,0,3  0,1,0  1  3  0,0,255
```

### Controls

| Key           | Action             									|
|---------------|-------------------------------------------------------|
| `ESC` key		| Close the window   									|
| Red cross		| Close the window   									|
| Arrow keys    | Move the camera (or the active object)				|
| Numpad keys   | Toggle on or off some of the rendering pipeline 		|
| `WASDQE` keys | Rotate the camera (or the active object, if possible)	|
| `+-` keys		| Change the camera's Field Of View						|

---

## Features

### Mandatory

- [x] Sphere, plane, cylinder intersection
- [x] Ambient + diffuse lighting
- [x] Hard shadows
- [x] Scene parsing from `.rt` file
- [x] MiniLibX window rendering

### Bonus

- [x] Phong specular highlights
- [x] Checkerboard texture (UV-mapped)
- [x] Bump mapping via XPM height maps
- [x] Cone primitive
- [x] Multi-light support
- [x] Colored lights

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

### Norminette & 42 Tooling

- [Norminette — 42Paris/norminette](https://github.com/42Paris/norminette)

---

### AI Usage

**Claude (Anthropic)** was used during the development of this project for the following tasks:

- **Mathematical explanations**: understanding the intersection equations for cylinders and cones (quadratic form derivation), UV mapping formulas, and TBN frame construction for bump mapping
- **Debugging assistance**: identifying off-by-one errors in parsing, analyzing incorrect normals at primitive caps, and diagnosing shadow acne artifacts
- **Code review**: checking norminette compliance after refactoring, and reviewing modular architecture choices
- **Texture generation**: generating XPM heightmap textures (e.g., moon surface) used as bump map inputs

AI was used as a **learning and debugging tool**, not to generate the core implementation. All ray-object intersection code, the lighting model, the parser, and the rendering loop were written by the author.
