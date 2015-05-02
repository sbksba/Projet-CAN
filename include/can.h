#ifndef __CAN_H_
#define __CAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

#define COORD_MIN 0
#define COORD_MAX 1000

#define SIZE_NOEUD  11

#define HAUT   0
#define BAS    1
#define GAUCHE 2
#define DROITE 3

#define HORIZONTAL 0
#define VERTICAL   1

#define TRUE  1
#define FALSE 0

#define N            2
#define BOOTSTRAP    1
#define COORDINATEUR 0

#define TAG_INSERS_TOI  50
#define TAG_ESPACE      51
#define TAG_AYAI        52 /* ... JE SUIS DEDANS */
#define TAG_TOI_INSERE  53
#define TAG_VOIS_BAS    54
#define TAG_VOIS_HAUT   55
#define TAG_VOIS_GAUCHE 56
#define TAG_VOIS_DROITE 57
#define TAG_NOEUD       58
#define DEB             59
#define FIN             60

typedef struct _point {
  int x;
  int y;
} point;

typedef struct _espace {
  point a;
  point b;
  point ap;
  point bp;
} espace;

typedef struct _noeud {
  int id;
  point *p;
  espace *es;
  struct _liste_noeud *haut;
  struct _liste_noeud *bas;
  struct _liste_noeud *droite;
  struct _liste_noeud *gauche;
} noeud;

typedef struct _liste_noeud {
  noeud *n;
  struct _liste_noeud *suivant;
} liste_noeud;

/* ************ */
/*  PRIMITIVES  */
/* ************ */

/* POINT */
int aleatoire(int min, int max);
point *alea(int a, int b);

/* NOEUD */
noeud *initNoeud(int id);

#endif //__CAN_H_
