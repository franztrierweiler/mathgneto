/*
 * combat.h - systeme de combat
 * F.T. - Toussaint 1995, ecrit sur LC III avec Think C
 */

#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"

/* j'avais defini Boolean dans le projet Think C, je laisse pour l'instant */
typedef unsigned char Boolean;

/* maximum d'ennemis pour les combats de groupe (V2) */
#define NB_ENNEMIS_MAX 4

void combat_start(struct joueur *j, struct entite *e);
char *combat_message(void);

/* tour de combat : renvoie TRUE si le combat continue, FALSE si fini */
Boolean tick_combat(struct joueur *j, struct entite *e);

#endif
