/*
 * types.h - typedefs et constantes communes
 * Captain Mathgneto - F.T. - Octobre 1995
 */

#ifndef TYPES_H
#define TYPES_H

#define TRUE  1
#define FALSE 0

/* note : on n'a pas de typedef Bool, parce que Xlib le definit deja
   en macro (#define Bool int). on utilise donc int directement
   pour les booleens, c'est de toute facon ce que fait Xlib. */

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

/* dimensions de la fenetre, copie de l'original Mac (512x342) */
#define WIN_W 512
#define WIN_H 342

/* zone de jeu : le panneau des verbes prend la droite */
#define GAME_W 384
#define GAME_H 280

/* taille d'une tuile carte */
#define TILE   16

/* taille des cartes (cf maps.h) */
#define MAP_W  40
#define MAP_H  24

/* tuiles */
#define T_HERBE  '.'
#define T_MUR    '#'
#define T_EAU    '~'
#define T_PORTE  'D'
#define T_ARBRE  'T'
#define T_TEMPLE 'X'
#define T_SABLE  ','
#define T_SOL    ' '
#define T_CAGE   'C'

/* etats du jeu */
#define ST_EXPLORE  0
#define ST_COMBAT   1
#define ST_QUIZ     2
#define ST_DIALOG   3
#define ST_GAMEOVER 4
#define ST_WIN      5

/* verbes (ordre des boutons dans le panneau) */
#define V_PAUSE    0
#define V_WALK     1
#define V_JUMP     2
#define V_FLY      3
#define V_GET      4
#define V_DROP     5
#define V_IDENT    6
#define V_STATUS   7
#define V_ZGATE    8
#define NB_VERBES  9

/* niveaux QCM (variante "lieux de connaissance") */
#define NIV_COLLEGE  0
#define NIV_LYCEE    1
#define NIV_PREPA    2
#define NIV_INGE     3
#define NB_NIVEAUX   4

/* limites */
#define MAX_PNJ     32
#define MAX_OBJ     64
#define MAX_INV     16

/* objets */
struct objet {
    int  x, y;
    int  type;       /* 0=pomme, 1=epee, 2=cle, 3=potion */
    int  visible;
    int  ramasse;
    char nom[32];
};

/* entites (pnj, ennemis) */
struct entite {
    int  x, y;
    int  hp;
    int  type;       /* 0=alien, 1=humanoide ami, 2=garde */
    int  vivant;
    int  amical;
    char nom[32];
};

/* etat du joueur */
struct joueur {
    int x, y;
    int hp;
    int hp_max;
    int score;
    int nb_obj;
    int inv[MAX_INV];   /* indices dans la liste des objets */
    int verbe_actif;    /* cf V_xxx */
    int carte;          /* 0=exterieur, 1=interieur, ... */
};

#endif
