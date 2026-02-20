// basique:
// soit un espace xyz.
// on place une camera au point xyz oriente vers xyz.
// il y a un plan (de meme resolution que l'ecran) place a une certaine distance de la camera (FOV)
// alors, pour chaque pixel de l'ecran, on project un rayon partant de la camera
// qui passe par le meme point sur le plan
// et lorsqu'il rencontre un objet: ce pixel est de la couleur de l'objet

// Pour chaque pixel de l'ecran:
// faire une equation (pour chaque object) qui verifie la collision
// dessiner le pixel.

#include <stdio.h>

#include "main.h"

t_sphere g_s = {{0, 0, 100}, 10};
t_sphere g_light = {{0, 20, 100}, 10};

int main(void)
{
	t_app *app;

	app = create_app();
	if (!app)
		return (-1);
	mlx_hook(app->win_ptr, 17, 0, exit_n_clean, app);
	redraw(app);
	mlx_loop(app->mlx_ptr);
}