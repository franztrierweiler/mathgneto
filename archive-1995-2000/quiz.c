/*
 * quiz.c - logique QCM
 * F.T. - Toussaint 1995, LC III + Think C
 *
 * tirage avec un bitset pour eviter de retomber sur la meme question
 * tant qu'on n'a pas vu tout le niveau.
 * cf K&R 2e ed chap 6.6 pour les bit fields, mais ici je fais a la main
 * avec des unsigned long.
 */

#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "quiz.h"
#include "questions.h"

/* compteurs : on calcule via sizeof, comme ca ils restent
   automatiquement a jour quand on ajoute des questions */
#define NB_QU_COLLEGE (sizeof(questions_college) / sizeof(questions_college[0]))
#define NB_QU_LYCEE   (sizeof(questions_lycee)   / sizeof(questions_lycee[0]))
#define NB_QU_PREPA   (sizeof(questions_prepa)   / sizeof(questions_prepa[0]))
#define NB_QU_INGE    (sizeof(questions_inge)    / sizeof(questions_inge[0]))

/* bitset 64 bits (suffit pour 64 questions par niveau) */
static unsigned long vu[NB_NIVEAUX];

static int qid_courant = -1;
static int niv_courant = NIV_COLLEGE;

void quiz_init(void)
{
    int i;
    for (i = 0; i < NB_NIVEAUX; i++) vu[i] = 0UL;
    qid_courant = -1;
}

static int nb_questions(int niveau)
{
    if (niveau == NIV_COLLEGE) return (int)NB_QU_COLLEGE;
    if (niveau == NIV_LYCEE)   return (int)NB_QU_LYCEE;
    if (niveau == NIV_PREPA)   return (int)NB_QU_PREPA;
    if (niveau == NIV_INGE)    return (int)NB_QU_INGE;
    return 0;
}

struct question *quiz_get_question(int niveau, int qid)
{
    if (qid < 0) return NULL;
    if (niveau == NIV_COLLEGE && qid < (int)NB_QU_COLLEGE)
        return &questions_college[qid];
    if (niveau == NIV_LYCEE && qid < (int)NB_QU_LYCEE)
        return &questions_lycee[qid];
    if (niveau == NIV_PREPA && qid < (int)NB_QU_PREPA)
        return &questions_prepa[qid];
    if (niveau == NIV_INGE && qid < (int)NB_QU_INGE)
        return &questions_inge[qid];
    return NULL;
}

int pick_question(int niveau)
{
    int n = nb_questions(niveau);
    int tries;
    int q;

    if (n <= 0) return -1;

    /* si tout vu, on remet a zero (sinon boucle infinie) */
    if (vu[niveau] == ((1UL << n) - 1)) {
        vu[niveau] = 0UL;
    }

    /* on tire au hasard jusqu'a tomber sur une question pas vue */
    /* boucle bornee, sinon ca pourrait coincer si tout vu */
    for (tries = 0; tries < 100; tries++) {
        q = rand() % n;
        if (!(vu[niveau] & (1UL << q))) {
            vu[niveau] |= (1UL << q);
            return q;
        }
    }
    /* fallback : on prend la premiere non vue */
    {
        int i;
        for (i = 0; i < n; i++) {
            if (!(vu[niveau] & (1UL << i))) {
                vu[niveau] |= (1UL << i);
                return i;
            }
        }
    }
    return 0;  /* bidouille mais ca marche */
}

void quiz_start(int niveau)
{
    niv_courant = niveau;
    qid_courant = pick_question(niveau);
}

struct question *quiz_courante(void)
{
    return quiz_get_question(niv_courant, qid_courant);
}

int quiz_id_courant(void)
{
    return qid_courant;
}

int quiz_niveau_courant(void)
{
    return niv_courant;
}

int quiz_repondre(int rep)
{
    struct question *q = quiz_courante();
    if (!q) return 0;
    return (rep == q->bonne) ? 1 : 0;
}
