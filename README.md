# Intersection sphere explanation:

A ray is defined as
`P(t) = O + D*t`
Where P is the collection of points forming the ray.
O is the origin of the ray (the camera).
D the direction vector.
t a parameter describing time.

A sphere centered around the point C with radius r satisfies:
`|P - C|² = r²`

A point that is the intersection of both the ray and the sphere must
verify both equations:

`P(t) = O + D*t`
`|P - C|² = r²`

By substituting the ray equation into the sphere equation:
`|O + D*t - C|² = r²`

Let `L = 0 - C` (vector from sphere center to ray origin), we have:

`|L + D*t|² = r²`
Expand the squared length (dot product with itself)
`|D|²t² + 2(D.L)t + |L|² - r² = 0`

This is a quadratic equation in `t` with the form
`at² + bt + c = 0`, where

`a= D.D`, `b= 2(D.L)`, `c= L.L - r²`

We solve it, 

If Δ<0: no intersection
If Δ=0: one intersection (tangent)
If Δ>0: two intersections