# dot product geometrical interpretation
How much are two vectors pointing in the same direction?
f θ < 90° → cos(θ) > 0 → dot > 0
→ vectors generally point same direction
If θ = 90° → cos(θ) = 0 → dot = 0
→ vectors are perpendicular
If θ > 90° → cos(θ) < 0 → dot < 0
→ vectors point opposite directions

valgrind --leak-check=full ./miniRT

# explanation camera movement
	else if (keycode == g_KEY_ARROW_UP)
    {
        delta = vec3_scale(cam->up, g_MOV_STRENGTH);
        cam->pos = vec3_add(cam->pos, delta);
    }
when we press up, we use cam->up's orientation vector to move the camera's y position. Not just camera.pos.y += g_MOV_STRENGTH!

# Intersection sphere explanation:

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

# Rotation for camera

https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula