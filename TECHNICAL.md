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

We accumulate the color with the **Phong model**: ambient (a constant base light), diffuse ($\vec{N} \cdot \vec{L}$, how directly the surface normal $\vec{N}$ faces the light direction $\vec{L}$), and specular (the shiny highlight).

The **specular** is the reflection of the light off the surface towards the eye. We reflect the light direction around the normal, then dot it with the view direction (the negated ray). The more aligned they are, the brighter the highlight:

$$\text{specular} = (\vec{R} \cdot \vec{V})^{\,\alpha}, \qquad \vec{R} = 2(\vec{N} \cdot \vec{L})\,\vec{N} - \vec{L}$$

where $\vec{R}$ is the reflected light direction, $\vec{V}$ the view direction, and $\alpha$ the shininess exponent.

The **bump map** fakes surface detail without extra geometry. We sample a grayscale height texture at the hit's UV, take its gradient via central differences (the slope in $u$ and $v$), and use it to perturb the normal in the surface's tangent basis (TBN). Lighting then reacts to this perturbed normal, so a flat surface looks bumpy.

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

Rodrigues' rotation of a vector $\vec{v}$ around a unit axis $\vec{k}$ by angle $\theta$:

$$\vec{v}_{\text{rot}} = \vec{v}\cos\theta + (\vec{k} \times \vec{v})\sin\theta + \vec{k}\,(\vec{k} \cdot \vec{v})(1 - \cos\theta)$$

# Mathematical details


## What's a ray

A ray is defined as a normalized directional vector $\vec{D}$ with an origin $\vec{O}$. It can be represented with the function notation:

$$\vec{P}(t) = \vec{O} + t\vec{D}$$

Where

- $\vec{P}$ is the collection of points forming the ray.
- $\vec{O}$ is the origin of the ray (the camera).
- $\vec{D}$ the direction vector.
- $t$ a parameter describing time.

This is equivalent to the system of equations:

$$
\begin{aligned}
x(t) &= O_x + D_x\, t \\
y(t) &= O_y + D_y\, t \\
z(t) &= O_z + D_z\, t
\end{aligned}
$$

Each value of $t$ gives one point $(x, y, z)$ on the ray.

We construct a ray from the camera to each pixel points. Given a pixel $(x, y)$ on a screen, we first have to calculate the coordinates of that vector $\vec{P}$ of coordinate $P_x, P_y, P_z$.


## dot product (fr: produit scalaire)

Let $\vec{A}$ and $\vec{B}$ be two vectors of the same dimension, the dot product is defined as:

$$\vec{A} \cdot \vec{B} = |\vec{A}|\,|\vec{B}|\cos\theta$$

Geometrically, $\vec{A} \cdot \vec{B}$ is the length of the orthogonal projection of $\vec{A}$ onto the line passing through $\vec{B}$.

$$
\begin{aligned}
\vec{A} \cdot \vec{B} > 0 \quad &\Longleftrightarrow \quad \vec{A} \text{ and } \vec{B} \text{ point in the same direction} \\
\vec{A} \cdot \vec{B} = 0 \quad &\Longleftrightarrow \quad \text{vectors are perpendicular} \\
\vec{A} \cdot \vec{B} < 0 \quad &\Longleftrightarrow \quad \text{vectors point in opposite directions}
\end{aligned}
$$

In an orthonormal basis space, it can also be calculated using the coordinates of the vectors $\vec{A}$ and $\vec{B}$.

if $\vec{A}(x_A, y_A)$ and $\vec{B}(x_B, y_B)$, then

$$\vec{A} \cdot \vec{B} = x_A x_B + y_A y_B$$

or also

$$
\begin{aligned}
\vec{A} \cdot \vec{B} &= \tfrac{1}{2}\left(|\vec{A}+\vec{B}|^2 - |\vec{A}|^2 - |\vec{B}|^2\right) \\
\vec{A} \cdot \vec{B} &= \tfrac{1}{2}\left(|\vec{A}|^2 + |\vec{B}|^2 - |\vec{A}-\vec{B}|^2\right)
\end{aligned}
$$

### Normalized case

if $\vec{B}$ is normalized, then $\vec{A} \cdot \vec{B} = |\vec{A}|\cos\theta$. It is the length of the orthogonal projection of $\vec{A}$ onto $\vec{B}$.

if both are normalized, then $\vec{A} \cdot \vec{B} = \cos\theta$.

## cross product (fr: produit vectoriel)

Let $\vec{A}$ and $\vec{B}$ be two 3D vectors. The cross product, $\vec{A} \times \vec{B}$ is a vector $\vec{C}$ that is orthogonal to the plane defined by $\vec{A}$ and $\vec{B}$ (orthogonal to both $\vec{A}$ and $\vec{B}$).

$$
\vec{C} = \vec{A} \times \vec{B} =
\begin{pmatrix}
A_y B_z - A_z B_y \\
A_z B_x - A_x B_z \\
A_x B_y - A_y B_x
\end{pmatrix}
$$

# Calculating intersection with objects

Raytracing is about casting rays, calculating the intersections between those rays and every objects in the scene.

## Intersection sphere:

A ray is defined as

$$\vec{P}(t) = \vec{O} + t\vec{D}$$

A sphere centered around the point $\vec{C}$ with radius $r$ satisfies:

$$(x - C_x)^2 + (y - C_y)^2 = r^2$$

It can be rewritten in vector form as:

$$|\vec{P} - \vec{C}|^2 = r^2, \quad \text{with } \vec{P}(x, y)$$

A point that is the intersection of both the ray and the sphere must verify both equations:

$$
\begin{aligned}
\vec{P}(t) &= \vec{O} + t\vec{D} \\
|\vec{P} - \vec{C}|^2 &= r^2
\end{aligned}
$$

By substituting the ray equation into the sphere equation:

$$|\vec{O} + t\vec{D} - \vec{C}|^2 = r^2$$

Let $\vec{L} = \vec{O} - \vec{C}$ (vector from sphere center to ray origin), we have:

$$|t\vec{D} + \vec{L}|^2 = r^2$$

Expand the squared length (dot product with itself)

$$|\vec{D}|^2\, t^2 + 2(\vec{D} \cdot \vec{L})\, t + |\vec{L}|^2 - r^2 = 0$$

This is a quadratic equation in $t$ with the form

$$a t^2 + b t + c = 0, \quad \text{where}$$

$$a = \vec{D} \cdot \vec{D}, \qquad b = 2(\vec{D} \cdot \vec{L}), \qquad c = \vec{L} \cdot \vec{L} - r^2$$

We solve it using the discriminant $\Delta = b^2 - 4ac$,

$$
\begin{aligned}
\Delta < 0 \quad &\Rightarrow \quad \text{no intersection} \\
\Delta = 0 \quad &\Rightarrow \quad \text{one intersection (tangent)} \\
\Delta > 0 \quad &\Rightarrow \quad \text{two intersections}
\end{aligned}
$$

## Intersection plane:

Equation of a plane defined with a point $\vec{P}$ on the plane, with the normal vector $\vec{N}$ and $\vec{R}$, a reference point on the plane.

$$(\vec{P} - \vec{R}) \cdot \vec{N} = 0$$

Substitute $\vec{P}$ with $\vec{P} = \vec{O} + t\vec{D}$, $\vec{O}$ is the origin of the ray, $\vec{D}$ is the directional vector

$$(\vec{O} + t\vec{D} - \vec{R}) \cdot \vec{N} = 0$$

solve for $t$: (we check if it's positive, if the plane is in front of the camera)

$$
\begin{aligned}
(\vec{O} + t\vec{D} - \vec{R}) \cdot \vec{N} &= 0 \\
\vec{O} \cdot \vec{N} + t(\vec{D} \cdot \vec{N}) - \vec{R} \cdot \vec{N} &= 0 \\
t(\vec{D} \cdot \vec{N}) &= \vec{R} \cdot \vec{N} - \vec{O} \cdot \vec{N}
\end{aligned}
$$

$$t = \frac{(\vec{R} - \vec{O}) \cdot \vec{N}}{\vec{D} \cdot \vec{N}}$$

## Intersection cylinder:

1) Find the intersection points between the ray and the infinite cylinder tube.
2) Check if those points are inside the finite cylinder.
3) Find the intersection with the two disks that are the top and bottom of the cylinder.

-> returns the lowest positive $t$ between the hits of the disks and the tube.

### 1) Step 1
I would have done it in the geometrical approach, but apparently it's slower. So the current
implementation is:
Let's project $\vec{D}$ onto the cylinder's axis.

$$\vec{D} = \vec{D}_{\perp} + \vec{D}_{\parallel}$$

with

- $\vec{D}_{\parallel}$ = how much the ray moves along the axis
- $\vec{D}_{\perp}$ = how much the ray moves around the cylinder

Let $\vec{A}$ be the cylinder's axis.

$$
\begin{aligned}
\vec{D}_{\parallel} &= (\vec{D} \cdot \vec{A})\,\vec{A} \\
\vec{D}_{\perp} &= \vec{D} - \vec{D}_{\parallel}
\end{aligned}
$$

Finding the intersection point with the tube is equivalent to determining

$$|t\,\vec{D}_{\perp} + \vec{OC}_{\perp}|^2 = r^2 \quad \text{(check sphere)}$$

with $\vec{OC}_{\perp}$ being the perpendicular component of $\vec{OC}$ along $\vec{A}$. $\vec{C}$ is the center of the cylinder.

### 2) Step 2

we get $t$. We check if $\vec{P}(t)$ is inside the finite cylinder.

$$-\frac{h}{2} < y < \frac{h}{2}$$

with $y$ being the projection of $\vec{P}$ onto the axis ($h$ is the cylinder height)

$$y = \vec{CP} \cdot \vec{A}$$

### 3) Step 3

First we check the intersection with the ray and the plane that emcompass the disk.
Then we check if that intersection point is inside the disk (the length from the center to that point is inferior to the radius of the disk).

# UV

For each object, we have a function that turns a point $\vec{P}(x, y, z)$ on the object to $(u, v)$ coordinates.
Using those $(u, v)$ coordinate, we can associate them to a pixel position on a 2D texture image.

### Plane point to UV

Establish a $(\vec{X}, \vec{Y}, \text{plane axis})$ orthonormal basis.

$\vec{X}$ and $\vec{Y}$ are two normalized vector on the plane.

For $\vec{P}$, a point on the plane. We have $\vec{CP}$ being the vector from the plane center point to the point $\vec{P}$ such that:

$$\vec{CP} = u\,\vec{X} + v\,\vec{Y}$$

But see that

$$\vec{CP} \cdot \vec{X} = |\vec{CP}|\cos\theta \quad \text{because } |\vec{X}| = 1.$$

In trigonometry, $|\vec{CP}|\cos\theta$ is exactly the length of the projection of $\vec{CP}$ onto the line defined by $\vec{X}$. This length is, by definition, the $u$ coordinate.

So we have

$$
\begin{aligned}
u &= \vec{CP} \cdot \vec{X} \\
v &= \vec{CP} \cdot \vec{Y}
\end{aligned}
$$


### Sphere point to UV

https://en.wikipedia.org/wiki/UV_mapping

`local = vec3_normalize(vec3_sub(P, obj.pos));`

👉 Ça transforme ton point en vecteur unité depuis le centre de la sphère.
Donc maintenant tu es sur une sphère unité.

Projection sphérique → coordonnées angulaires
Projection angulaire → carré 2D
Damier sur ce carré

$$
\begin{aligned}
u &= 0.5 + \frac{\operatorname{atan2}(\text{local}_z,\ \text{local}_x)}{2\pi} \\
v &= 0.5 - \frac{\arcsin(\text{local}_y)}{\pi}
\end{aligned}
$$

![Phong Model](https://upload.wikimedia.org/wikipedia/commons/6/6b/Phong_components_version_4.png)
![Lambert Specular](https://upload.wikimedia.org/wikipedia/commons/b/bd/Lambert2.gif)


### Useful commands

`valgrind --leak-check=full ./miniRT scenes/default.rt`

`convert a.jpg -compress none b.xpm`
