/*
 * combat.c - systeme de combat
 * F.T. - Toussaint 1995, LC III + Think C 7
 *
 * teste OK avec Think C 7 sur le LC III, devrait passer sur AIX aussi.
 * combat tres simple : counter aleatoire, on retire des HP des deux cotes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "combat.h"

/* #pragma mark - Combat principal */
/* (oublie de virer ce pragma Think C, pas grave, gcc l'ignore) */

static char msg[128];

void combat_start(struct joueur *j, struct entite *e)
{
    /* on resoud le combat tout de suite (tour unique pour la V1) */
    int degats_j;
    int degats_e;

    /* degats du joueur : 1 a 5, +2 si verbe Get/Drop... non. on simplifie */
    degats_j = 1 + (rand() % 5);
    /* si le joueur a une epee dans l'inventaire, +3 (TODO: verifier l'inv) */
    /* pour l'instant on fait juste +2 si score > 0 (proxy) */
    if (j->score > 10) degats_j += 2;

    degats_e = 1 + (rand() % 4);

    e->hp -= degats_j;
    j->hp -= degats_e;

    if (j->hp < 0) j->hp = 0;
    if (e->hp <= 0) {
        e->vivant = FALSE;
        j->score += 20;
        sprintf(msg, "Vaincu ! %s a fait %d degats, vous %d.", e->nom, degats_e, degats_j);
    } else {
        sprintf(msg, "%s : -%d HP. Vous : -%d HP.", e->nom, degats_j, degats_e);
    }
}

char *combat_message(void)
{
    return msg;
}

Boolean tick_combat(struct joueur *j, struct entite *e)
{
    /* pour la V1 on resout en un coup donc on renvoie tj FALSE */
    (void)j;
    (void)e;
    return 0;
}

#ifdef macintosh
/* test standalone sur Mac, retire avant compilation finale */
/*
int main(void)
{
    struct joueur j;
    struct entite e;
    j.hp = 20; j.score = 0;
    e.hp = 8; e.vivant = 1;
    strcpy(e.nom, "TestAlien");
    combat_start(&j, &e);
    printf("%s\n", combat_message());
    return 0;
}
*/
#endif
