/*
 * questions.h - QCM mathematiques
 * F.T. - Toussaint 1995, LC III + Think C
 * (saisi sur Mac, transfere par disquette HFS)
 *
 * 4 niveaux : college, lycee bac, lycee prepa, ingenierie.
 * environ 20 questions par niveau pour la V1.
 *
 * note : pas d'accents, j'avais des problemes avec l'editeur Think C
 * et la table de caracteres MacRoman vs Latin-1. plus simple sans.
 */

#ifndef QUESTIONS_H
#define QUESTIONS_H

struct question {
    char *enonce;
    char *choix[4];
    int   bonne;
};

/* === Niveau College (6e - 3e) === */
static struct question questions_college[] = {
    { "Combien font 7 x 8 ?",
      { "54", "56", "64", "63" }, 1 },

    { "Quelle est la racine carree de 144 ?",
      { "10", "11", "12", "14" }, 2 },

    { "Si 3x + 5 = 20, que vaut x ?",
      { "3", "5", "7", "15" }, 1 },

    { "Combien font (-3) x (-4) ?",
      { "-12", "12", "-7", "7" }, 1 },

    { "L'aire d'un disque de rayon 5 est environ :",
      { "31", "78", "25", "100" }, 1 },

    { "Triangle rect., hypotenuse 13, un cote 5. Autre cote ?",
      { "8", "10", "12", "11" }, 2 },

    { "PGCD de 24 et 36 ?",
      { "6", "12", "18", "24" }, 1 },

    { "Combien fait 2^10 ?",
      { "100", "512", "1024", "2048" }, 2 },

    { "Une fraction egale a 3/4 :",
      { "6/9", "9/12", "12/15", "15/18" }, 1 },

    { "Somme des angles d'un triangle (en degres) ?",
      { "90", "180", "270", "360" }, 1 },

    { "Combien font 15% de 200 ?",
      { "15", "20", "25", "30" }, 3 },

    { "Volume d'un cube de cote 4 ?",
      { "16", "32", "48", "64" }, 3 },

    { "Solution de x^2 = 49 :",
      { "7", "-7", "7 et -7", "rien" }, 2 },

    { "Combien font 1/2 + 1/3 ?",
      { "2/5", "5/6", "1/5", "2/6" }, 1 },

    { "Diagonale d'un carre de cote 1 :",
      { "1", "racine de 2", "2", "racine de 3" }, 1 },

    { "Combien fait 9! / 7! ?",
      { "63", "72", "81", "9" }, 1 },

    { "Probabilite de tirer un as dans 32 cartes ?",
      { "1/8", "1/4", "1/16", "1/32" }, 0 },

    { "Si f(x) = 2x + 1, que vaut f(3) ?",
      { "5", "6", "7", "8" }, 2 },

    { "Combien font 0,1 + 0,2 ?",
      { "0,12", "0,3", "0,21", "1,2" }, 1 },

    { "Perimetre d'un cercle de rayon 7, environ ?",
      { "22", "44", "49", "14" }, 1 }
};

/* === Niveau Lycee (2nde - Tle programme S des annees 90) === */
static struct question questions_lycee[] = {
    { "Derivee de f(x) = x * ln(x) :",
      { "ln(x)", "1 + ln(x)", "1/x", "x/ln(x)" }, 1 },

    { "Suite geom. raison 2, u_0 = 3. u_5 ?",
      { "48", "96", "32", "64" }, 1 },

    { "Integrale de 0 a pi de sin(x) dx :",
      { "0", "1", "2", "pi" }, 2 },

    { "Limite quand x -> +inf de (x^2 - x) / x^2 :",
      { "0", "1", "infini", "indefini" }, 1 },

    { "Derivee de cos(x)^2 :",
      { "-sin(2x)", "2 cos(x)", "-2 sin(x) cos(x)", "0" }, 0 },

    { "Equation : x^2 - 5x + 6 = 0, racines :",
      { "1 et 6", "2 et 3", "-2 et -3", "1 et 5" }, 1 },

    { "Discriminant de x^2 + x + 1 :",
      { "1", "-1", "-3", "5" }, 2 },

    { "Vecteur (1,2,3) . (4,5,6) =",
      { "32", "12", "21", "0" }, 0 },

    { "Logarithme : ln(e^3) =",
      { "1", "3", "e", "e^3" }, 1 },

    { "f(x) = e^x, primitive :",
      { "e^x", "e^(x+1)", "x e^x", "ln(x)" }, 0 },

    { "Combien de combinaisons C(5,2) ?",
      { "5", "10", "20", "25" }, 1 },

    { "Suite arith. raison 3, u_0 = 1. Somme u_0 a u_9 ?",
      { "145", "150", "135", "100" }, 0 },

    { "Module du complexe 3 + 4i :",
      { "3", "4", "5", "7" }, 2 },

    { "Argument de 1 + i (en radians) :",
      { "0", "pi/4", "pi/2", "pi" }, 1 },

    { "Aire sous y = x^2 entre 0 et 2 :",
      { "8/3", "4", "2", "16/3" }, 0 },

    { "lim sin(x)/x quand x -> 0 :",
      { "0", "1", "infini", "indefini" }, 1 },

    { "P(A inter B) si A et B independants, P(A)=0.5, P(B)=0.4 :",
      { "0.2", "0.9", "0.1", "0.5" }, 0 },

    { "Solution de 2^x = 8 :",
      { "2", "3", "4", "log(8)" }, 1 },

    { "Tangente a y = x^3 en x = 1, pente :",
      { "1", "2", "3", "0" }, 2 },

    { "Esperance d'un de a 6 faces :",
      { "3", "3.5", "4", "3.14" }, 1 }
};

/* === Niveau Prepa (MPSI / MP / PSI 1995) === */
static struct question questions_prepa[] = {
    { "Dans S_5, l'ordre maximal d'un element :",
      { "5", "6", "10", "120" }, 1 },

    { "Nombre d'ideaux dans Z/12Z :",
      { "3", "4", "6", "12" }, 2 },

    { "Theoreme de Cayley-Hamilton : toute matrice carree A...",
      { "est diagonalisable", "annule son polyn. carac.",
        "est inversible", "verifie det(A) = tr(A)" }, 1 },

    { "f endomorphisme de R^3, ker(f) = {0}. Alors f est :",
      { "nilpotent", "bijectif", "non defini", "de rang 2" }, 1 },

    { "Un groupe d'ordre 15 est :",
      { "necessairement abelien", "non-abelien",
        "isomorphe a S_3", "non identifiable" }, 0 },

    { "X^4 + 1 est-il irreductible sur Q ?",
      { "Oui", "Non, fact. sur R", "Non, divisible X^2+1", "Non, racine 1" }, 0 },

    { "Matrice nilpotente : ses valeurs propres sont :",
      { "toutes nulles", "egales a 1", "complexes conj.", "indeterminees" }, 0 },

    { "Dimension de l'espace des poly. de degre <= n :",
      { "n", "n+1", "n^2", "2n" }, 1 },

    { "Theoreme du rang : dim(ker f) + rg(f) =",
      { "dim domaine", "dim image", "0", "n+1" }, 0 },

    { "Convergence simple n'implique pas :",
      { "limite ponctuelle", "convergence uniforme",
        "borne", "continuite limite" }, 1 },

    { "Une suite de Cauchy dans R est :",
      { "convergente", "bornee mais pas conv.",
        "non bornee", "alternee" }, 0 },

    { "Inegalite de Cauchy-Schwarz : <u,v>^2 <=",
      { "<u,u> + <v,v>", "<u,u> * <v,v>",
        "norm(u)*norm(v)", "norm(u+v)^2" }, 1 },

    { "Dans un anneau integre fini, tout element non nul est :",
      { "nul", "inversible", "nilpotent", "idempotent" }, 1 },

    { "Le polynome minimal de la matrice diag(1,1,2) :",
      { "(X-1)(X-2)", "(X-1)^2(X-2)", "X^2 - 3X + 2", "X-1" }, 0 },

    { "Serie sum 1/n^2 vaut :",
      { "ln(2)", "pi^2/6", "infinie", "pi/4" }, 1 },

    { "Topologie : un compact de R est :",
      { "ouvert", "ferme borne", "ferme", "borne" }, 1 },

    { "Une matrice antisym. reelle a ses val. propres :",
      { "reelles", "imaginaires pures", "nulles", "rationnelles" }, 1 },

    { "Theoreme de Bezout : pgcd(a,b) = 1 ssi exist. u,v t.q.",
      { "a + b = 1", "au + bv = 1", "ab = 1", "a = b" }, 1 },

    { "L'ensemble des solutions de y' = y est :",
      { "{0}", "{exp}", "espace de dim 1", "espace de dim 2" }, 2 },

    { "Reduction de Jordan necessite quel corps ?",
      { "Q", "R", "C ou alg. clos", "n'importe lequel" }, 2 }
};

/* === Niveau Ingenierie (Bac+5 / 3e annee 1995) === */
static struct question questions_inge[] = {
    { "Coef. de x^4 dans le DL en 0 de cos(x^2) :",
      { "1/2", "-1/2", "1/24", "-1/24" }, 1 },

    { "Limite x -> 0 de (sin(x) - x) / x^3 :",
      { "0", "-1/6", "1/6", "infinie" }, 1 },

    { "Code RS(n,k) sur GF(2^m), capacite de correction :",
      { "n - k erreurs", "(n - k) / 2 erreurs", "k erreurs", "log2(n)" }, 1 },

    { "Polyn. generateur d'un RS(15,11) sur GF(16), degre :",
      { "4", "11", "15", "26" }, 0 },

    { "Integrale sur |z|=2 de dz/(z^2 + 1) :",
      { "0", "2 pi i", "pi i", "pi" }, 0 },

    { "Solution stationnaire de l'eq. de la chaleur 1D sur [0,L] :",
      { "constante", "sinusoidale", "affine", "exponentielle" }, 2 },

    { "Entropie de Shannon d'une source binaire equiprobable :",
      { "0 bit", "1/2 bit", "1 bit", "2 bits" }, 2 },

    { "Conditionnement d'une matrice sym. def. positive :",
      { "rapport val. propres", "tr / det",
        "1 / det", "norme Frobenius" }, 0 },

    { "FFT sur N points, complexite :",
      { "O(N)", "O(N log N)", "O(N^2)", "O(N^3)" }, 1 },

    { "Un filtre RIF d'ordre N a combien de coefficients :",
      { "N", "N+1", "2N", "log N" }, 1 },

    { "Distance minimale d'un code (n,k,d) corrige :",
      { "d", "d-1", "(d-1)/2", "d/2" }, 2 },

    { "Loi gaussienne : densite proportionnelle a :",
      { "exp(-x)", "exp(-x^2/2)", "1/x", "exp(x^2)" }, 1 },

    { "Theoreme central limite : la moyenne tend vers :",
      { "loi uniforme", "loi de Poisson",
        "loi normale", "loi exponentielle" }, 2 },

    { "Schema d'Euler explicite, ordre :",
      { "1", "2", "3", "4" }, 0 },

    { "Methode de Newton pour racines, ordre :",
      { "1 (lineaire)", "2 (quadratique)", "3 (cubique)", "0" }, 1 },

    { "Decomposition LU existe si la matrice est :",
      { "symetrique", "inversible et mineurs ppaux non nuls",
        "creuse", "tridiagonale" }, 1 },

    { "Equation de Poisson en 2D, condition limite Dirichlet :",
      { "u = 0 au bord", "du/dn = 0", "lap u = 0", "div u = 0" }, 0 },

    { "Variance d'une somme de n var. iid de variance v :",
      { "v", "n*v", "v/n", "n^2 v" }, 1 },

    { "Critere de Nyquist, freq. echantillonnage > :",
      { "freq. signal", "2 * freq. max", "freq. max / 2", "1 Hz" }, 1 },

    { "Markov d'ordre 1 : depend de :",
      { "tout le passe", "etat precedent",
        "deux etats prec.", "etat futur" }, 1 }
};

#endif
