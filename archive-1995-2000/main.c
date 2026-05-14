/*
 * main.c - boucle principale Captain Mathgneto
 * F.T. - Octobre/Novembre 1995, RS/6000 + LC III
 *
 * grosse fonction main qui fait tout : init, boucle d'evenements, cleanup.
 * c'est pas tres beau mais ca marche. tant que ca tient en memoire.
 *
 * note : j'ai lu le K&R 2e ed chap 7 sur les fichiers, mais pour la V1
 * je n'ai pas besoin de fichier de save donc je laisse tomber.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

/* unistd pour usleep, c'est pas ANSI mais c'est dispo sur AIX et Linux */
#include <unistd.h>

#include "types.h"
#include "render.h"
#include "input.h"
#include "world.h"
#include "combat.h"
#include "quiz.h"

/* etat global du joueur, accessible partout (plus simple) */
struct joueur joueur;

/* nom de la fenetre, en CamelCase parce que je l'ai pique d'un exemple Mac */
static char *mainWindowTitle = "Captain Mathgneto - F.T. 1995";

/* ces deux noms sont en doublon historique : j'avais mis NB_PNJ_MAX
   dans world.h et NB_ENNEMIS_MAX dans combat.h, meme valeur en gros. */

int main()  /* notez : pas de void en parametre, comme dans K&R 1ere ed */
{
    XEvent ev;
    int running = TRUE;
    int redraw = TRUE;

    /* seed random : on prend l'heure */
    srand((unsigned int)time(NULL));

    /* init du moteur de rendu */
    if (init_render() < 0) {
        fprintf(stderr, "init_render a echoue\n");
        return 1;
    }
    (void)mainWindowTitle;  /* pas utilise tout le temps, on tait le warning */

    /* init monde et quiz */
    init_world();
    quiz_init();

    /* etat initial du joueur */
    joueur.x = 5;
    joueur.y = 5;
    joueur.hp = 20;
    joueur.hp_max = 20;
    joueur.score = 0;
    joueur.nb_obj = 0;
    joueur.verbe_actif = V_WALK;
    joueur.carte = 0;

    /* boucle principale */
    while (running) {

        /* on pompe tous les events en attente */
        while (XPending(dpy)) {
            XNextEvent(dpy, &ev);
            switch (ev.type) {
                case Expose:
                    redraw = TRUE;
                    break;

                case ButtonPress:
                    if (ev.xbutton.button == Button1) {
                        on_click(ev.xbutton.x, ev.xbutton.y, &joueur);
                        redraw = TRUE;
                    }
                    break;

                case KeyPress: {
                    /* en V1 on ne gere pas le clavier sauf 'q' pour quitter */
                    KeySym ks = XLookupKeysym(&ev.xkey, 0);
                    if (ks == XK_q || ks == XK_Q || ks == XK_Escape) {
                        running = FALSE;
                    }
                    break;
                }

                case ConfigureNotify:
                    redraw = TRUE;
                    break;

                case ClientMessage:
                    /* probablement un WM_DELETE_WINDOW mais on ne l'a pas demande,
                       donc on laisse passer */
                    break;

                default:
                    break;
            }
        }

        /* tests de fin de partie */
        if (joueur.hp <= 0 && etat_jeu != ST_GAMEOVER) {
            etat_jeu = ST_GAMEOVER;
            redraw = TRUE;
        }
        if (joueur.score >= 100 && etat_jeu != ST_WIN) {
            /* victoire = avoir 100 pts (proxy pour la Crown of Control en V1) */
            etat_jeu = ST_WIN;
            redraw = TRUE;
        }

        if (redraw) {
            render_clear();

            if (etat_jeu == ST_QUIZ) {
                /* on dessine le QCM par dessus */
                render_quiz(quiz_niveau_courant(), quiz_id_courant());
                render_panel(&joueur);
                render_titre();
            } else if (etat_jeu == ST_COMBAT) {
                render_world(&joueur);
                render_combat_msg(combat_message());
                render_panel(&joueur);
                render_titre();
            } else if (etat_jeu == ST_GAMEOVER) {
                render_dialog("GAME OVER - Fritz est tombe au combat !");
                render_panel(&joueur);
                render_titre();
            } else if (etat_jeu == ST_WIN) {
                render_dialog("VICTOIRE ! Fritz a recupere la Crown of Control !");
                render_panel(&joueur);
                render_titre();
            } else {
                /* mode exploration normal */
                render_world(&joueur);
                render_panel(&joueur);
                render_player(&joueur);
                render_titre();
                /* status est un mode en plus : si verbe = STATUS, on overlaye */
                if (joueur.verbe_actif == V_STATUS) {
                    render_status(&joueur);
                }
            }
            render_flush();
            redraw = FALSE;
        }

        /* on dort un peu, sinon le X server est inonde
           33 ms = environ 30 FPS, ca suffit largement pour un jeu tour par tour */
        usleep(33333);
    }

    /* cleanup */
    close_render();

    /* printf("score final : %d\n", joueur.score); */
    return 0;
}
