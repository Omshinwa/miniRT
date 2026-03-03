# Useful commands

valgrind --leak-check=full ./miniRT


# Camera

## View of the world

We construct a normalized directional ray from the camera for each pixel points.
We create a pixel(x,y) -> Point(x,y,z) on virtual screen function.
The FOV (field of view), is the angle that tells us how much we see.

2) So we first have to calculate the coordinates of that point P.

3) Then at the end we normalize this directional vector.

## Explanation camera movement
	else if (keycode == g_KEY_ARROW_UP)
    {
        delta = vec3_scale(cam->up, g_MOV_STRENGTH);
        cam->pos = vec3_add(cam->pos, delta);
    }
when we press up, we use cam->up's orientation vector to move the camera's y position. Not just camera.pos.y += g_MOV_STRENGTH!

## Rotation for camera

https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

# MATH TOOLS

## dot product (fr: produit scalaire)

Let A and B be two vectors of the same dimension, the dot product is defined as:

`A.B = |A| * |B| * cos(θ)`

Geometrically, A.B is the length of the orthogonal projection of A onto the line passing through B.
A.B >  0  ←→ A and B points in the same direction.
A.B == 0  ←→ vectors are perpendicular
A.B <  0  ←→ vectors point opposite directions

In an orthonormee basis space, it can also be calculated using the coordinates of the vectors A and B.
if A(xa, ya) and B(xb, yb), then

`A.B = xa * xb + ya * yb`

or also

`A.B = 1/2 ( |A+B|² - |A|² - |B|² )`
`A.B = 1/2 ( |A|² + |B|² - |A-B|² )`

### normalized case

if B is normalized, then `A.B = |A| * cos(θ)`. It is the length of the orthogonal projection of A onto B.
if both are normalized, then `A.B = cost(θ)`.

## cross product (fr: produit vectoriel)

Let A and B be two 3D vectors. The cross product, A × B is a vector C that is orthogonal to the plane defined by A and B (/ orthonogal to both A and B).

	Cx=	Ay * Bz - Az * By
	Cy=	Az * Bx - Ax * Bz
	Cz=	Ax * By - Ay * Bx

# Calculating intersection with objects

## Intersection sphere:

A ray is defined as
`P(t) = O + D*t`
Where P is the collection of points forming the ray.
O is the origin of the ray (the camera).
D the direction vector.
t a parameter describing time.

A sphere centered around the point C with radius r satisfies:
`(x - Cx)² + (y - Cy)² = r²`
It can be rewritten in vector form as:
`|P - C|² = r²`, with P(x, y)

A point that is the intersection of both the ray and the sphere must
verify both equations:

`P(t) = O + D*t`
`|P - C|² = r²`

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

Equation d'un plan defini par un point P quelconque sur le plan, avec le vecteur normal N et R un point de reference du plan:
`(P - R) . N = 0`
Substitute P with `P = O + D * t`, O is the origin of the ray, D is the directional vector
`(O + D * t - R) . N = 0`
solve for t: (we check if it's positive, if the plane is in front of the camera)
`(O + D * t - R) . N = 0` ==
`O.N + (D*t).N - R.N = 0` ==
`(D*t).N = - O.N + R.N` ==
`t*(D.N) = R.N - O.N` ==

`t = (R - O) . N / (D . N)`

## Intersection cylinder:

1) Find the intersection points between the ray and the infinite cylinder tube.
2) Check if those points are inside the finite cylinder.
3) Find the intersection with the two disks that are the top and bottom of the cylinder.

-> returns the lowest positive t between the hits of the disks and the tube.

### 1)
I would have done it in the geometrical approach, but apparently it's slower. So the current
implementation is:
Let's project D onto the cylinder's axis.
`D = D_perp + D_parallel` with
* D_parallel = how much the ray moves along the axis
* D_perp = how much the ray moves around the cylinder
Let A be the cylinder's axis.
`D_parallel = D.A * A`
`D_perp = D - D_parallel`

Finding the intersection point with the tube is equivalent to determining
`| D_perp * t + OC_perp |² = r²` (check sphere)
with OC_perp being the perpendicular component of OC along A.
C is the center of the cylinder.

### 2)

we get `t`. We check if P(t) is inside the finite cylinder.
-cylinder.height / 2< y < cylinder.height / 2
with y being the projection of P onto the axis
`y = CP . A`

### 3)

First we check the intersection with the ray and the plane that emcompass the disk.
Then we check if that intersection point is inside the disk (the length from the center to that point is inferior to the radius of the disk).

## UV

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
`u = CP⋅X`
`v = CP⋅Y`


### Sphere point to UV
https://en.wikipedia.org/wiki/UV_mapping
local = vec3_normalize(vec3_minus(P, obj.pos));
👉 Ça transforme ton point en vecteur unité depuis le centre de la sphère.
Donc maintenant tu es sur une sphère unité.

Projection sphérique → coordonnées angulaires
Projection angulaire → carré 2D
Damier sur ce carré
