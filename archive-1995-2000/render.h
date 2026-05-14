/*
 * render.h - rendu graphique
 * F.T. - Octobre 1995, RS/6000
 */

#ifndef RENDER_H
#define RENDER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "types.h"

/* globales d'affichage : declarees ici, definies dans render.c */
extern Display *dpy;
extern Window   win;
extern int      screen;
extern GC       gc;
extern XFontStruct *fnt;

/* hitboxes du panneau des verbes (calculees dans init_render) */
extern int verbe_x[NB_VERBES];
extern int verbe_y[NB_VERBES];
extern int verbe_w;
extern int verbe_h;

/* hitbox compass rose */
extern int compass_x, compass_y, compass_r;

/* zones cliquables QCM (4 reponses) */
extern int qcm_x[4];
extern int qcm_y[4];
extern int qcm_w, qcm_h;

int  init_render(void);
void close_render(void);

void render_clear(void);
void render_world(struct joueur *j);
void render_panel(struct joueur *j);
void render_compass(void);
void render_titre(void);
void render_player(struct joueur *j);
void render_combat_msg(char *msg);
void render_quiz(int niveau, int qid);
void render_dialog(char *txt);
void render_status(struct joueur *j);

void render_flush(void);

/* utilitaires de dessin */
void draw_text(int x, int y, char *s);
void draw_box(int x, int y, int w, int h);
void draw_box_filled(int x, int y, int w, int h);

#endif
