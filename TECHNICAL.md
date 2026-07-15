# Project Architecture

## parse/

First we need to parse the `.rt` file.

One cool technique I implemented for this parser is the use of the `offsetof` macro. This allows me to describe the structure of the parsing.

I can define:
```cpp
static const t_instruction g_cylinder_instruction
    = { "cy", OBJ_CYLINDER, false, offsetof(t_object, data),
          {
              { "pos", T_VEC, true, offsetof(t_cylinder, pos) },
              { "axis", T_UNIT, true, offsetof(t_cylinder, axis) },
              { "diameter", T_DIAMETER, true, offsetof(t_cylinder, radius) },
              { "height", T_FLOAT, true, offsetof(t_cylinder, height) },
              { NULL, T_INVALID, false, 0 },
          } };
```
`g_cylinder_instruction` holds the information that a Cylinder must first be defined as **`cy`** for the identifier, followed by the **position** as a vector, then the **axis** as a unit vector, and last should be the **height** as a float.

Instead of one routine per object, the grammar is described as *data*. A single generic engine handles all of them as a pipeline: **file → line → instruction → field → token**.

While we parse, we setup the program and world.

## app/

The App is the interface with the minilibx. It hooks the events, constructs the window and handle interactions such as allowing the user to move around or to select any object to move. It is highlighted yellow in the 3D space for clarity

The object to move is simply a generic pointer. If it's NULL the camera is to be moved.

When the user clicks, a ray is cast, the first object to hit is the new object to move.

## raytrace/

We have to render every pixel on screen.

The typical workflow to render a pixel:

Create the ray (from the camera, to that pixel). Solve math equations to find the closest object this ray hits. Every math equation is different depending on the math object (see below for details). When we hit an object, we also note other infos such as the normal it hits the surface, the UV point etc...

## render /

Now that we know which point on which object we hit, we try to reach every light in the scene.

Each light we reach illuminates this point, if we hit another object before, we are in this object's shadow.

We accumulate the color with the **Phong model**: ambient (a constant base light), diffuse (`normal . light_dir`, how directly the surface faces the light), and specular (the shiny highlight).

The **specular** is the reflection of the light off the surface towards the eye. We reflect the light direction around the normal, then dot it with the view direction (the negated ray). The more aligned they are, the brighter the highlight.

The **bump map** fakes surface detail without extra geometry. We sample a grayscale height texture at the hit's UV, take its gradient via central differences (the slope in u and v), and use it to perturb the normal in the surface's tangent basis (TBN). Lighting then reacts to this perturbed normal, so a flat surface looks bumpy.

Each contribution is multiplied by the object's **color** (which can come from a texture UV map) and attenuated by distance, then all lights are summed into the final pixel color.

## Camera

### Camera movement
	else if (keycode == g_KEY_ARROW_UP)
    {
        delta = vec3_sca(cam->up, g_MOV_STRENGTH);
        cam->pos = vec3_add(cam->pos, delta);
    }
When we press up, we use cam->up's orientation vector to move the camera's y position. We want to move up relative to the camera, not based on the world!

### Rotation for camera

https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

# Mathematical details


## What's a ray

A ray is defined as a normalized directional vector D with an origin O. It can be represented with the function notation:

`P(t) = O + D*t`

Where

    P is the collection of points forming the ray.
    O is the origin of the ray (the camera).
    D the direction vector.
    t a parameter describing time.

This is equivalent to the system of equation:

```
x(t) = Ox + Dx * t
y(t) = Oy + Dy * t
z(t) = Oz + Dz * t
```

Each value of `t` gives one point `(x, y, z)` on the ray.

We construct a ray from the camera to each pixel points. Given a pixel (x,y) on a screen, we first have to calculate the coordinates of that Vector P of coordinate p.x, p.y, p.z.


## dot product (fr: produit scalaire)

Let A and B be two vectors of the same dimension, the dot product is defined as:

`A.B = |A| * |B| * cos(θ)`

Geometrically, A.B is the length of the orthogonal projection of A onto the line passing through B.
```
A.B >  0  ←→ A and B points in the same direction.
A.B == 0  ←→ vectors are perpendicular
A.B <  0  ←→ vectors point opposite directions
```
In an orthonormee basis space, it can also be calculated using the coordinates of the vectors A and B.

if A(xa, ya) and B(xb, yb), then

`A.B = xa * xb + ya * yb`

or also

```
A.B = 1/2 ( |A+B|² - |A|² - |B|² )
A.B = 1/2 ( |A|² + |B|² - |A-B|² )
```
### Normalized case

if B is normalized, then `A.B = |A| * cos(θ)`. It is the length of the orthogonal projection of A onto B.

if both are normalized, then `A.B = cost(θ)`.

## cross product (fr: produit vectoriel)

Let A and B be two 3D vectors. The cross product, A × B is a vector C that is orthogonal to the plane defined by A and B (orthonogal to both A and B).

	Cx=	Ay * Bz - Az * By
	Cy=	Az * Bx - Ax * Bz
	Cz=	Ax * By - Ay * Bx

# Calculating intersection with objects

Raytracing is about casting rays, calculating the intersections between those rays and every objects in the scene.

## Intersection sphere:

A ray is defined as

`P(t) = O + D*t`

A sphere centered around the point C with radius r satisfies:

`(x - Cx)² + (y - Cy)² = r²`

It can be rewritten in vector form as:

`|P - C|² = r²`, with P(x, y)

A point that is the intersection of both the ray and the sphere must
verify both equations:

    P(t) = O + D*t
    |P - C|² = r²

By substituting the ray equation into the sphere equation:

`|O + D*t - C|² = r²`

Let `L = 0 - C` (vector from sphere center to ray origin), we have:

`|D*t + L|² = r²`

Expand the squared length (dot product with itself)

`|D|²*t² + 2(D.L)*t + |L|² - r² = 0`

This is a quadratic equation in `t` with the form

`a*t² + b*t + c = 0`, where

`a= D.D`, `b= 2(D.L)`, `c= L.L - r²`

We solve it, 

    If Δ<0: no intersection
    If Δ=0: one intersection (tangent)
    If Δ>0: two intersections

## Intersection plane:

Equation of a plane defined with a point P on the plane, with the normal vector N and R, a reference point on the plane.

`(P - R) . N = 0`

Substitute P with `P = O + D * t`, O is the origin of the ray, D is the directional vector
`(O + D * t - R) . N = 0`

solve for t: (we check if it's positive, if the plane is in front of the camera)

    (O + D * t - R) . N = 0 thus
    O.N + (D*t).N - R.N = 0 thus
    (D*t).N = - O.N + R.N thus
    t*(D.N) = R.N - O.N

`t = (R - O) . N / (D . N)`

## Intersection cylinder:

1) Find the intersection points between the ray and the infinite cylinder tube.
2) Check if those points are inside the finite cylinder.
3) Find the intersection with the two disks that are the top and bottom of the cylinder.

-> returns the lowest positive t between the hits of the disks and the tube.

### 1) Step 1
I would have done it in the geometrical approach, but apparently it's slower. So the current
implementation is:
Let's project D onto the cylinder's axis.

`D = D_perp + D_parallel` with

    D_parallel = how much the ray moves along the axis
    D_perp = how much the ray moves around the cylinder

Let A be the cylinder's axis.

    D_parallel = D.A * A
    thus
    D_perp = D - D_parallel

Finding the intersection point with the tube is equivalent to determining
`| D_perp * t + OC_perp |² = r²` (check sphere)
with OC_perp being the perpendicular component of OC along A.
C is the center of the cylinder.

### 2) Step 2

we get `t`. We check if P(t) is inside the finite cylinder.

`-cylinder.height / 2< y < cylinder.height / 2`

with y being the projection of P onto the axis

`y = CP . A`

### 3) Step 3

First we check the intersection with the ray and the plane that emcompass the disk.
Then we check if that intersection point is inside the disk (the length from the center to that point is inferior to the radius of the disk).

# UV

For each object, we have a function that turns a point P(x,y,z) on the object to (u,v) coordinates.
Using those (u,v) coordinate, we can associate them to a pixel position on a 2D texture image.

### Plane point to UV

Establish a (X, Y, plane_axis) orthonormal basis.

X and Y are two normalized vector on the plane.

For P, a point on the plane. We have CP being the vector from the plane center point to the point P such that:

`CP = u * X + v * Y`

But see that

`CP⋅X = |CP|cos(θ)` because |X| = 1.

In trigonometry, |CP|cos(θ) is exactly the length of the projection of CP onto the line defined by X. This length is, by definition, the u coordinate.

So we have

    u = CP⋅X
    v = CP⋅Y


### Sphere point to UV

https://en.wikipedia.org/wiki/UV_mapping

`local = vec3_normalize(vec3_sub(P, obj.pos));`

👉 Ça transforme ton point en vecteur unité depuis le centre de la sphère.
Donc maintenant tu es sur une sphère unité.

Projection sphérique → coordonnées angulaires
Projection angulaire → carré 2D
Damier sur ce carré

![Phong Model](https://upload.wikimedia.org/wikipedia/commons/6/6b/Phong_components_version_4.png)
![Lambert Specular](https://upload.wikimedia.org/wikipedia/commons/b/bd/Lambert2.gif)


### Useful commands

`valgrind --leak-check=full ./miniRT scenes/default.rt`

`convert a.jpg -compress none b.xpm`