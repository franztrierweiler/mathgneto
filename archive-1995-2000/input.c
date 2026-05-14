/*
 * input.c - gestion souris
 * Captain Mathgneto - F.T. - Octobre 1995
 *
 * tout est a la souris : compass rose pour bouger, boutons verbes
 * pour les actions, et clic sur les reponses du QCM.
 * pas de clavier en V1 (comme dans le jeu original Mac).
 */

#include <stdio.h>
#include <stdlib.h>   /* rand() */

#include "types.h"
#include "input.h"
#include "render.h"
#include "world.h"
#include "combat.h"
#include "quiz.h"

int etat_jeu = ST_EXPLORE;

/* test : un point est-il dans le rectangle (rx,ry,rw,rh) ? */
static int in_rect(int x, int y, int rx, int ry, int rw, int rh)
{
    return (x >= rx && x < rx + rw && y >= ry && y < ry + rh);
}

/* test : un point est-il dans le losange compass de centre (cx,cy) rayon r ? */
static int in_compass(int x, int y, int cx, int cy, int r)
{
    int dx = x - cx;
    int dy = y - cy;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return (dx + dy <= r);
}

/* sous-zones du compass : N S E O */
static int compass_quadrant(int x, int y, int cx, int cy)
{
    int dx = x - cx;
    int dy = y - cy;
    int adx = dx < 0 ? -dx : dx;
    int ady = dy < 0 ? -dy : dy;
    if (ady > adx) {
        if (dy < 0) return HIT_NORD;
        else        return HIT_SUD;
    } else {
        if (dx < 0) return HIT_OUEST;
        else        return HIT_EST;
    }
}

int hit_test(int mx, int my)
{
    int i;

    /* compass rose ? */
    if (in_compass(mx, my, compass_x, compass_y, compass_r)) {
        return compass_quadrant(mx, my, compass_x, compass_y);
    }

    /* un des boutons de verbes ? */
    for (i = 0; i < NB_VERBES; i++) {
        if (in_rect(mx, my, verbe_x[i], verbe_y[i], verbe_w, verbe_h)) {
            return HIT_VERBE + i;
        }
    }

    /* en mode QCM, clic sur une reponse ? */
    if (etat_jeu == ST_QUIZ) {
        for (i = 0; i < 4; i++) {
            if (in_rect(mx, my, qcm_x[i], qcm_y[i], qcm_w, qcm_h)) {
                return HIT_QCM + i;
            }
        }
    }

    /* zone de jeu ? */
    if (in_rect(mx, my, 8, 8, GAME_W, GAME_H)) {
        return HIT_GAME;
    }

    return HIT_NONE;
}

/* gestion d'un clic souris */
void on_click(int mx, int my, struct joueur *j)
{
    int h = hit_test(mx, my);
    int dx = 0, dy = 0;

    /* clic sur un bouton de verbe = changement de verbe actif */
    if (h >= HIT_VERBE && h < HIT_VERBE + NB_VERBES) {
        int v = h - HIT_VERBE;
        /* PAUSE est un toggle, pas un verbe persistant */
        if (v == V_PAUSE) {
            /* TODO: gerer la pause vraiment, pour l'instant on ignore */
            return;
        }
        if (v == V_STATUS) {
            /* affiche le status, mais c'est gere par render_status
               appele depuis main.c avec un etat dedie - ici on signale */
            j->verbe_actif = V_STATUS;
            return;
        }
        j->verbe_actif = v;
        return;
    }

    /* en mode QCM */
    if (etat_jeu == ST_QUIZ) {
        if (h >= HIT_QCM && h < HIT_QCM + 4) {
            int rep = h - HIT_QCM;
            int ok = quiz_repondre(rep);
            if (ok) {
                j->score += 10;
                j->hp += 2;
                if (j->hp > j->hp_max) j->hp = j->hp_max;
            } else {
                j->hp -= 2;
                if (j->hp < 0) j->hp = 0;
            }
            etat_jeu = ST_EXPLORE;
        }
        return;
    }

    /* en mode combat, n'importe quel clic = continuer */
    if (etat_jeu == ST_COMBAT) {
        etat_jeu = ST_EXPLORE;
        return;
    }

    /* clic sur la compass = deplacement */
    if (h == HIT_NORD)  dy = -1;
    if (h == HIT_SUD)   dy = +1;
    if (h == HIT_EST)   dx = +1;
    if (h == HIT_OUEST) dx = -1;

    if (dx != 0 || dy != 0) {
        int nx = j->x + dx;
        int ny = j->y + dy;
        if (world_walkable(j->carte, nx, ny)) {
            j->x = nx;
            j->y = ny;
            /* on regarde si on entre dans un temple ou on tombe sur un alien */
            if (world_tile_at(j->carte, nx, ny) == T_TEMPLE) {
                /* niveau aleatoire pour la V1 */
                quiz_start(rand() % NB_NIVEAUX);
                etat_jeu = ST_QUIZ;
                return;
            }
            if (world_tile_at(j->carte, nx, ny) == T_PORTE) {
                j->carte = 1 - j->carte;
                /* hack : on remet le joueur a une position safe */
                j->x = 4;
                j->y = 4;
                return;
            }
            /* check les entites adjacentes (combat) */
            if (world_entite_at(j->carte, nx, ny) >= 0) {
                int idx = world_entite_at(j->carte, nx, ny);
                struct entite *e = world_get_entite(idx);
                if (!e->amical) {
                    combat_start(j, e);
                    etat_jeu = ST_COMBAT;
                    return;
                }
            }
            /* check les objets : si verbe = Get on ramasse */
            if (j->verbe_actif == V_GET) {
                int idx = world_objet_at(j->carte, nx, ny);
                if (idx >= 0) {
                    struct objet *o = world_get_objet(idx);
                    o->ramasse = TRUE;
                    if (j->nb_obj < MAX_INV) {
                        j->inv[j->nb_obj++] = idx;
                    }
                    j->score += 5;
                }
            }
        }
        return;
    }

    /* clic dans la zone de jeu sans verbe particulier : on ignore */
    if (h == HIT_GAME) {
        /* TODO: faire bouger en cliquant sur la carte ? plus tard */
        return;
    }
}
