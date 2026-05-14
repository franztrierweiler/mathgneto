/*
 * input.h - gestion des entrees souris
 * F.T. - Octobre 1995, RS/6000
 */

#ifndef INPUT_H
#define INPUT_H

#include <X11/Xlib.h>

#include "types.h"

/* codes retour des fonctions de hit-test */
#define HIT_NONE     0
#define HIT_NORD     1
#define HIT_SUD      2
#define HIT_EST      3
#define HIT_OUEST    4
#define HIT_VERBE    100  /* + index du verbe */
#define HIT_QCM      200  /* + index 0..3 */
#define HIT_GAME     300  /* clic dans la zone de jeu */

int hit_test(int mx, int my);
void on_click(int mx, int my, struct joueur *j);

/* etat global du jeu (cf ST_xxx dans types.h) */
extern int etat_jeu;

#endif
