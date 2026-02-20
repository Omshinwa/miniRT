# file organization
* create branches for each parts
* Separer la struct app en deux: une partie pour l'interface avec la minilibx, et une pour l'environ 3D
* * for example, the hooks and app could be put together?, same for raytrace and math, idk about draw
* * seems like draw is closer to app

# app structure
* linked list of objects, remove globals

# math / raytracing
* put the light in the t_app, do the light calc
* add plane raytracing
* instead of having 4 vec3, put everything inside a single matrix[4][4]?

# UI / UX
* click to target object, make it moveable
* turn camera view

# others
* parse files
* use david's libft?