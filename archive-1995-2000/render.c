/*
 * render.c - rendu graphique X11
 * Captain Mathgneto - F.T. - Octobre 1995, RS/6000
 *
 * tout le code Xlib est ici. j'ai mis les pixmaps en global pour
 * pas les passer en parametre partout, c'est plus simple.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "render.h"
#include "maps.h"
#include "world.h"
#include "quiz.h"

/* on inclut les sprites directement, c'est ce qui est pratique en XBM */
#include "sprites/magneto.xbm"
#include "sprites/alien.xbm"
#include "sprites/apple.xbm"
#include "sprites/wall.xbm"
#include "sprites/temple.xbm"
#include "sprites/sword.xbm"
#include "sprites/key.xbm"
#include "sprites/tree.xbm"
#include "sprites/grass.xbm"
#include "sprites/water.xbm"
#include "sprites/sand.xbm"
#include "sprites/guard.xbm"

/* globales : c'est plus simple comme ca */
Display *dpy = NULL;
Window   win = 0;
int      screen = 0;
GC       gc = NULL;
XFontStruct *fnt = NULL;

/* pixmaps des sprites */
static Pixmap pm_magneto, pm_alien, pm_apple, pm_wall, pm_temple;
static Pixmap pm_sword, pm_key, pm_tree, pm_grass, pm_water, pm_sand, pm_guard;

int verbe_x[NB_VERBES];
int verbe_y[NB_VERBES];
int verbe_w = 72;
int verbe_h = 18;

int compass_x = 0;
int compass_y = 0;
int compass_r = 30;

int qcm_x[4];
int qcm_y[4];
int qcm_w = 320;
int qcm_h = 24;

/* labels des verbes (cf screenshots originaux) */
static char *verbe_label[NB_VERBES] = {
    "PAUSE", "Walk", "Jump", "Fly",
    "Get",   "Drop", "Identify", "Status", "Z Gate"
};

int init_render(void)
{
    int i;
    unsigned long black, white;
    XSetWindowAttributes attr;

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "pas de display X !\n");
        return -1;
    }
    screen = DefaultScreen(dpy);
    black = BlackPixel(dpy, screen);
    white = WhitePixel(dpy, screen);

    win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
                              0, 0, WIN_W, WIN_H,
                              1, black, white);

    XStoreName(dpy, win, "Captain Mathgneto");

    /* on demande tout, on triera apres (pas tres propre mais bon) */
    XSelectInput(dpy, win,
                 ExposureMask | KeyPressMask | ButtonPressMask
               | StructureNotifyMask | PointerMotionMask);

    XMapWindow(dpy, win);

    /* GC unique pour tout le programme */
    gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, black);
    XSetBackground(dpy, gc, white);

    /* font - on prend "fixed" qui est tj la sur un X11 standard */
    fnt = XLoadQueryFont(dpy, "fixed");
    if (fnt) {
        XSetFont(dpy, gc, fnt->fid);
    }
    /* sinon on continue sans, ca affichera la font par defaut */

    /* chargement des sprites en pixmaps */
    pm_magneto = XCreateBitmapFromData(dpy, win, (char*)magneto_bits,
                                       magneto_width, magneto_height);
    pm_alien = XCreateBitmapFromData(dpy, win, (char*)alien_bits,
                                     alien_width, alien_height);
    pm_apple = XCreateBitmapFromData(dpy, win, (char*)apple_bits,
                                     apple_width, apple_height);
    pm_wall = XCreateBitmapFromData(dpy, win, (char*)wall_bits,
                                    wall_width, wall_height);
    pm_temple = XCreateBitmapFromData(dpy, win, (char*)temple_bits,
                                      temple_width, temple_height);
    pm_sword = XCreateBitmapFromData(dpy, win, (char*)sword_bits,
                                     sword_width, sword_height);
    pm_key = XCreateBitmapFromData(dpy, win, (char*)key_bits,
                                   key_width, key_height);
    pm_tree = XCreateBitmapFromData(dpy, win, (char*)tree_bits,
                                    tree_width, tree_height);
    pm_grass = XCreateBitmapFromData(dpy, win, (char*)grass_bits,
                                     grass_width, grass_height);
    pm_water = XCreateBitmapFromData(dpy, win, (char*)water_bits,
                                     water_width, water_height);
    pm_sand = XCreateBitmapFromData(dpy, win, (char*)sand_bits,
                                    sand_width, sand_height);
    pm_guard = XCreateBitmapFromData(dpy, win, (char*)guard_bits,
                                     guard_width, guard_height);

    /* hitboxes du panneau des verbes - calculees une fois pour toutes */
    /* compass rose en haut a droite */
    compass_x = WIN_W - 64;
    compass_y = 36;
    compass_r = 28;

    /* boutons : 9 verbes, hauteur 18, espace 4 */
    for (i = 0; i < NB_VERBES; i++) {
        verbe_x[i] = WIN_W - 92;
        verbe_y[i] = 80 + i * (verbe_h + 4);
    }

    /* zones QCM - calcule simplement, vertical sous l'enonce */
    for (i = 0; i < 4; i++) {
        qcm_x[i] = 32;
        qcm_y[i] = 130 + i * 30;
    }

    /* on attend le premier Expose avant de dessiner */
    XFlush(dpy);
    return 0;
}

void close_render(void)
{
    /* TODO: free les pixmaps proprement, j'ai pas le temps */
    if (fnt) XFreeFont(dpy, fnt);
    if (gc)  XFreeGC(dpy, gc);
    if (dpy) XCloseDisplay(dpy);
}

void render_clear(void)
{
    XClearWindow(dpy, win);
}

void draw_text(int x, int y, char *s)
{
    XDrawString(dpy, win, gc, x, y, s, strlen(s));
}

void draw_box(int x, int y, int w, int h)
{
    XDrawRectangle(dpy, win, gc, x, y, w, h);
}

void draw_box_filled(int x, int y, int w, int h)
{
    XFillRectangle(dpy, win, gc, x, y, w, h);
}

/* dessine une seule tuile a la position ecran (px,py) */
static void draw_tile(int px, int py, char tile)
{
    Pixmap pm;
    int w, h;

    pm = pm_grass;
    w = grass_width; h = grass_height;

    switch (tile) {
        case T_MUR:
            pm = pm_wall; w = wall_width; h = wall_height;
            break;
        case T_EAU:
            pm = pm_water; w = water_width; h = water_height;
            break;
        case T_ARBRE:
            pm = pm_tree; w = tree_width; h = tree_height;
            break;
        case T_TEMPLE:
            pm = pm_temple; w = temple_width; h = temple_height;
            break;
        case T_SABLE:
            pm = pm_sand; w = sand_width; h = sand_height;
            break;
        case T_PORTE:
            /* porte = un trait blanc dans un mur */
            XSetForeground(dpy, gc, WhitePixel(dpy, screen));
            XFillRectangle(dpy, win, gc, px, py, TILE, TILE);
            XSetForeground(dpy, gc, BlackPixel(dpy, screen));
            XDrawRectangle(dpy, win, gc, px+2, py+2, TILE-4, TILE-4);
            return;
        case T_HERBE:
            pm = pm_grass; w = grass_width; h = grass_height;
            break;
        case T_SOL:
        default:
            /* sol nu = blanc */
            XSetForeground(dpy, gc, WhitePixel(dpy, screen));
            XFillRectangle(dpy, win, gc, px, py, TILE, TILE);
            XSetForeground(dpy, gc, BlackPixel(dpy, screen));
            return;
    }
    XCopyPlane(dpy, pm, win, gc, 0, 0, w, h, px, py, 1L);
}

void render_world(struct joueur *j)
{
    int i, k;
    char (*m)[MAP_W + 1];
    int ox, oy;
    int vw, vh;

    /* selection de la carte */
    if (j->carte == 0) m = map_outdoor;
    else m = map_indoor;

    /* zone de jeu : on affiche autour du joueur (scrolling style) */
    /* mais sur 384x280 a 16px = 24x17 tuiles, pas tout a fait la carte */
    vw = GAME_W / TILE;   /* 24 */
    vh = GAME_H / TILE;   /* 17 */

    ox = j->x - vw / 2;
    oy = j->y - vh / 2;
    if (ox < 0) ox = 0;
    if (oy < 0) oy = 0;
    if (ox > MAP_W - vw) ox = MAP_W - vw;
    if (oy > MAP_H - vh) oy = MAP_H - vh;

    /* fond noir pour bordure */
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, 4, 4, GAME_W + 4, GAME_H + 4);

    for (i = 0; i < vh; i++) {
        for (k = 0; k < vw; k++) {
            int mx = ox + k;
            int my = oy + i;
            int px = 8 + k * TILE;
            int py = 8 + i * TILE;
            if (mx < 0 || mx >= MAP_W || my < 0 || my >= MAP_H) continue;
            draw_tile(px, py, m[my][mx]);
        }
    }

    /* entites */
    for (i = 0; i < world_nb_entites(); i++) {
        struct entite *e = world_get_entite(i);
        int px, py;
        Pixmap pm;
        int pw, ph;
        if (!e->vivant) continue;
        if (e->x < ox || e->x >= ox + vw) continue;
        if (e->y < oy || e->y >= oy + vh) continue;
        px = 8 + (e->x - ox) * TILE;
        py = 8 + (e->y - oy) * TILE;
        if (e->type == 0) { /* alien */
            pm = pm_alien; pw = alien_width; ph = alien_height;
        } else if (e->type == 2) { /* garde */
            pm = pm_guard; pw = guard_width; ph = guard_height;
        } else {
            pm = pm_alien; pw = alien_width; ph = alien_height;
        }
        XCopyPlane(dpy, pm, win, gc, 0, 0, pw, ph, px, py, 1L);
    }

    /* objets */
    for (i = 0; i < world_nb_objets(); i++) {
        struct objet *o = world_get_objet(i);
        int px, py;
        Pixmap pm;
        int pw, ph;
        if (!o->visible || o->ramasse) continue;
        if (o->x < ox || o->x >= ox + vw) continue;
        if (o->y < oy || o->y >= oy + vh) continue;
        px = 8 + (o->x - ox) * TILE;
        py = 8 + (o->y - oy) * TILE;
        if (o->type == 0) {
            pm = pm_apple; pw = apple_width; ph = apple_height;
        } else if (o->type == 1) {
            pm = pm_sword; pw = sword_width; ph = sword_height;
        } else if (o->type == 2) {
            pm = pm_key; pw = key_width; ph = key_height;
        } else {
            pm = pm_apple; pw = apple_width; ph = apple_height;
        }
        XCopyPlane(dpy, pm, win, gc, 0, 0, pw, ph, px+4, py+4, 1L);
    }

    /* le joueur */
    {
        int px = 8 + (j->x - ox) * TILE;
        int py = 8 + (j->y - oy) * TILE - 8;  /* legerement decale, sprite plus haut */
        XCopyPlane(dpy, pm_magneto, win, gc, 0, 0,
                   magneto_width, magneto_height, px, py, 1L);
    }
}

void render_compass(void)
{
    int cx = compass_x;
    int cy = compass_y;
    int r  = compass_r;

    /* compass = rose des vents, 4 fleches cliquables N S E O */
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));

    /* losange exterieur */
    {
        XPoint pts[5];
        pts[0].x = cx;     pts[0].y = cy - r;
        pts[1].x = cx + r; pts[1].y = cy;
        pts[2].x = cx;     pts[2].y = cy + r;
        pts[3].x = cx - r; pts[3].y = cy;
        pts[4].x = cx;     pts[4].y = cy - r;
        XDrawLines(dpy, win, gc, pts, 5, CoordModeOrigin);
    }
    /* croix interieure */
    XDrawLine(dpy, win, gc, cx - r, cy, cx + r, cy);
    XDrawLine(dpy, win, gc, cx, cy - r, cx, cy + r);

    /* petites fleches */
    XDrawString(dpy, win, gc, cx - 3, cy - r + 12, "N", 1);
    XDrawString(dpy, win, gc, cx - 3, cy + r - 4, "S", 1);
    XDrawString(dpy, win, gc, cx - r + 4, cy + 4, "O", 1);
    XDrawString(dpy, win, gc, cx + r - 8, cy + 4, "E", 1);
}

void render_panel(struct joueur *j)
{
    int i;
    /* trait separateur */
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawLine(dpy, win, gc, GAME_W + 12, 8, GAME_W + 12, GAME_H + 8);

    render_compass();

    /* boutons des verbes */
    for (i = 0; i < NB_VERBES; i++) {
        int bx = verbe_x[i];
        int by = verbe_y[i];
        if (i == j->verbe_actif) {
            /* bouton actif : noir avec texte blanc, comme dans le jeu original */
            XFillRectangle(dpy, win, gc, bx, by, verbe_w, verbe_h);
            XSetForeground(dpy, gc, WhitePixel(dpy, screen));
            XDrawString(dpy, win, gc, bx + 8, by + 13,
                        verbe_label[i], strlen(verbe_label[i]));
            XSetForeground(dpy, gc, BlackPixel(dpy, screen));
        } else {
            XDrawRectangle(dpy, win, gc, bx, by, verbe_w, verbe_h);
            XDrawString(dpy, win, gc, bx + 8, by + 13,
                        verbe_label[i], strlen(verbe_label[i]));
        }
    }
}

void render_titre(void)
{
    char buf[80];
    /* bandeau "Captain Mathgneto - by F.T." en bas */
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, 4, GAME_H + 18, GAME_W + 4, 36);
    sprintf(buf, "Captain Mathgneto");
    XDrawString(dpy, win, gc, 16, GAME_H + 38, buf, strlen(buf));
    sprintf(buf, "by F.T.");
    XDrawString(dpy, win, gc, GAME_W - 60, GAME_H + 50, buf, strlen(buf));
}

void render_player(struct joueur *j)
{
    /* affiche les infos joueur dans le panneau du bas */
    char buf[128];
    sprintf(buf, "HP: %d/%d  Score: %d  Inv: %d", j->hp, j->hp_max, j->score, j->nb_obj);
    XDrawString(dpy, win, gc, 16, GAME_H + 50, buf, strlen(buf));
}

void render_combat_msg(char *msg)
{
    int x = 32;
    int y = GAME_H / 2;
    /* fond blanc */
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    XFillRectangle(dpy, win, gc, x, y - 20, GAME_W - 60, 60);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, x, y - 20, GAME_W - 60, 60);
    XDrawString(dpy, win, gc, x + 12, y + 8, msg, strlen(msg));
    XDrawString(dpy, win, gc, x + 12, y + 24, "(clic pour continuer)", 21);
}

void render_quiz(int niveau, int qid)
{
    int i;
    struct question *q;
    char *titre;
    char buf[256];

    /* fond blanc, on efface tout */
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    XFillRectangle(dpy, win, gc, 8, 8, GAME_W, GAME_H);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, 8, 8, GAME_W, GAME_H);

    /* titre du temple selon niveau */
    if (niveau == NIV_COLLEGE)      titre = "Temple du Savoir - College";
    else if (niveau == NIV_LYCEE)   titre = "Temple du Savoir - Lycee";
    else if (niveau == NIV_PREPA)   titre = "Temple du Savoir - Prepa";
    else                            titre = "Temple du Savoir - Ingenierie";
    XDrawString(dpy, win, gc, 24, 32, titre, strlen(titre));

    q = quiz_get_question(niveau, qid);
    if (!q) {
        XDrawString(dpy, win, gc, 24, 64, "(pas de question disponible)", 28);
        return;
    }

    /* enonce */
    sprintf(buf, "Q: %s", q->enonce);
    XDrawString(dpy, win, gc, 24, 80, buf, strlen(buf));

    /* 4 reponses cliquables */
    for (i = 0; i < 4; i++) {
        int bx = qcm_x[i];
        int by = qcm_y[i];
        XDrawRectangle(dpy, win, gc, bx, by, qcm_w, qcm_h);
        sprintf(buf, "%c) %s", 'A' + i, q->choix[i]);
        XDrawString(dpy, win, gc, bx + 12, by + 16, buf, strlen(buf));
    }
}

void render_dialog(char *txt)
{
    int x = 24;
    int y = GAME_H - 60;
    /* bulle de dialogue */
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    XFillRectangle(dpy, win, gc, x, y, GAME_W - 50, 50);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, x, y, GAME_W - 50, 50);
    XDrawString(dpy, win, gc, x + 12, y + 24, txt, strlen(txt));
    XDrawString(dpy, win, gc, x + 12, y + 40, "(clic pour continuer)", 21);
}

void render_status(struct joueur *j)
{
    char buf[128];
    int x = 40;
    int y = 40;
    XSetForeground(dpy, gc, WhitePixel(dpy, screen));
    XFillRectangle(dpy, win, gc, x, y, 280, 200);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    XDrawRectangle(dpy, win, gc, x, y, 280, 200);
    XDrawString(dpy, win, gc, x + 12, y + 20, "--- Status de Fritz Mathgneto ---", 33);
    sprintf(buf, "HP   : %d / %d", j->hp, j->hp_max);
    XDrawString(dpy, win, gc, x + 12, y + 50, buf, strlen(buf));
    sprintf(buf, "Score: %d", j->score);
    XDrawString(dpy, win, gc, x + 12, y + 70, buf, strlen(buf));
    sprintf(buf, "Inv  : %d / %d", j->nb_obj, MAX_INV);
    XDrawString(dpy, win, gc, x + 12, y + 90, buf, strlen(buf));
    sprintf(buf, "Carte: %s", j->carte == 0 ? "exterieur" : "vaisseau");
    XDrawString(dpy, win, gc, x + 12, y + 110, buf, strlen(buf));
    XDrawString(dpy, win, gc, x + 12, y + 160, "(clic pour fermer)", 18);
}

void render_flush(void)
{
    XFlush(dpy);
}
