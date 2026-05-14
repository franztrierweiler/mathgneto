/*
 * quiz.h - QCM mathematiques (variante "lieux de connaissance")
 * F.T. - Toussaint 1995, LC III
 */

#ifndef QUIZ_H
#define QUIZ_H

#include "types.h"
#include "questions.h"

/* tirage d'une question : marque la question vue dans un bitset
   pour eviter les repetitions tant qu'on n'a pas tout vu */
void quiz_init(void);
void quiz_start(int niveau);
struct question *quiz_get_question(int niveau, int qid);
struct question *quiz_courante(void);
int  quiz_id_courant(void);
int  quiz_niveau_courant(void);

/* repondre ; renvoie 1 si correct, 0 sinon */
int  quiz_repondre(int rep);

/* tirer une question au hasard non encore vue dans le niveau */
int  pick_question(int niveau);

#endif
