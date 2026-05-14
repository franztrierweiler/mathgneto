/*
 * world.c - logique du monde
 * F.T. - Toussaint 1995
 *
 * gere les cartes, les entites, les objets.
 * une bonne partie a ete tapee chez moi sur le LC III pendant
 * les vacances, c'est plus tranquille qu'au libre-service.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "world.h"
#include "maps.h"

/* tableaux d'entites et d'objets : pas de listes chainees, on est en C */
static struct entite entites[MAX_PNJ];
static int nb_entites = 0;

static struct objet objets[MAX_OBJ];
static int nb_objets = 0;

/* qq objets predefinis, pour la V1 */
void init_world(void)
{
    int i;

    for (i = 0; i < MAX_PNJ; i++) entites[i].vivant = FALSE;
    for (i = 0; i < MAX_OBJ; i++) objets[i].visible = FALSE;
    nb_entites = 0;
    nb_objets = 0;

    /* quelques aliens hostiles sur la carte exterieure */
    entites[0].x = 18; entites[0].y = 5;
    entites[0].hp = 8; entites[0].type = 0; entites[0].vivant = TRUE;
    entites[0].amical = FALSE;
    strcpy(entites[0].nom, "Alien Garde");

    entites[1].x = 28; entites[1].y = 14;
    entites[1].hp = 6; entites[1].type = 0; entites[1].vivant = TRUE;
    entites[1].amical = FALSE;
    strcpy(entites[1].nom, "Alien Patrouille");

    /* un humanoide ami pres de la cage */
    entites[2].x = 35; entites[2].y = 21;
    entites[2].hp = 10; entites[2].type = 1; entites[2].vivant = TRUE;
    entites[2].amical = TRUE;
    strcpy(entites[2].nom, "Humanoide Captif");

    /* un garde dans le vaisseau */
    entites[3].x = 14; entites[3].y = 10;
    entites[3].hp = 12; entites[3].type = 2; entites[3].vivant = TRUE;
    entites[3].amical = FALSE;
    strcpy(entites[3].nom, "Garde Imperial");

    nb_entites = 4;

    /* qq objets : pommes, epee, cle */
    objets[0].x = 10; objets[0].y = 12;
    objets[0].type = 0; objets[0].visible = TRUE; objets[0].ramasse = FALSE;
    strcpy(objets[0].nom, "pomme");

    objets[1].x = 22; objets[1].y = 18;
    objets[1].type = 0; objets[1].visible = TRUE; objets[1].ramasse = FALSE;
    strcpy(objets[1].nom, "pomme");

    objets[2].x = 30; objets[2].y = 8;
    objets[2].type = 1; objets[2].visible = TRUE; objets[2].ramasse = FALSE;
    strcpy(objets[2].nom, "epee");

    objets[3].x = 26; objets[3].y = 16;
    objets[3].type = 2; objets[3].visible = TRUE; objets[3].ramasse = FALSE;
    strcpy(objets[3].nom, "cle");

    nb_objets = 4;
}

char world_tile_at(int carte, int x, int y)
{
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return T_MUR;
    if (carte == 0) return map_outdoor[y][x];
    return map_indoor[y][x];
}

int world_walkable(int carte, int x, int y)
{
    char t = world_tile_at(carte, x, y);
    /* mur et eau bloquent ; le reste passe (y compris portes et temples) */
    if (t == T_MUR) return FALSE;
    if (t == T_EAU) return FALSE;
    if (t == T_ARBRE) return FALSE;
    return TRUE;
}

int world_nb_entites(void) { return nb_entites; }

struct entite *world_get_entite(int i)
{
    /* pas de check de bounds, c'est sale mais bon */
    return &entites[i];
}

int world_entite_at(int carte, int x, int y)
{
    int i;
    /* O(n), suffit pour 32 entites max */
    for (i = 0; i < nb_entites; i++) {
        if (!entites[i].vivant) continue;
        if (entites[i].x == x && entites[i].y == y) return i;
    }
    /* TODO: utiliser carte aussi quand on aura des entites par carte */
    (void)carte;
    return -1;
}

int world_nb_objets(void) { return nb_objets; }

struct objet *world_get_objet(int i) { return &objets[i]; }

int world_objet_at(int carte, int x, int y)
{
    int i;
    for (i = 0; i < nb_objets; i++) {
        if (!objets[i].visible || objets[i].ramasse) continue;
        if (objets[i].x == x && objets[i].y == y) return i;
    }
    (void)carte;
    return -1;
}

/* recopie de chaine, cf K&R 2e ed chap 5.5 - tres elegant */
void copie_chaine(char *s, char *t)
{
    while ((*s++ = *t++) != '\0')
        ;
}

/* renverse une chaine en place, K&R 2e ed chap 3.6 */
void renverse(char s[])
{
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* lecture d'une ligne sur stdin, adapte du K&R 2e ed chap 1.9 */
int lit_ligne(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n') { s[i] = c; ++i; }
    s[i] = '\0';
    return i;
}
