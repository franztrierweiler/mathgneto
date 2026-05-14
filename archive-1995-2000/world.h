/*
 * world.h - etat du monde
 * F.T. - Toussaint 1995, LC III + Think C 7
 */

#ifndef WORLD_H
#define WORLD_H

#include "types.h"

void init_world(void);

/* tuile a la position (x,y) sur la carte donnee */
char world_tile_at(int carte, int x, int y);

/* peut-on marcher sur cette tuile ? */
int world_walkable(int carte, int x, int y);

int world_nb_entites(void);
struct entite *world_get_entite(int i);
int world_entite_at(int carte, int x, int y);

int world_nb_objets(void);
struct objet *world_get_objet(int i);
int world_objet_at(int carte, int x, int y);

/* renvoie une chaine en place, recopie d'un nom (cf K&R chap 5.5) */
void copie_chaine(char *s, char *t);

/* renverse une chaine en place, K&R 2e ed chap 3.6
   (j'avais pense m'en servir pour les bulles de dialogue
    en parlant en alien, mais finalement non) */
void renverse(char s[]);

/* lecture d'une ligne, adapte du K&R 2e ed chap 1.9
   (utilise pour parser les fichiers de config dans une V2 future) */
int lit_ligne(char s[], int lim);

#endif
