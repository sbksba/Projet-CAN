#include "can.h"

/* ************ */
/*  PRIMITIVES  */
/* ************ */

/* ####################################################### */
/* POINT */
/* ####################################################### */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 * Tire aléatoirement un point entre min et max 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 */
int aleatoire(int min, int max) {

  int low_num=0, hi_num=0;

  if(min < max)
    {
      low_num = min;
      hi_num = max +1;
    }
  else
    {
      low_num = max + 1;
      hi_num = min;
    }

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);

  /* using nano-seconds instead of seconds */
  srand((time_t)ts.tv_nsec);

  return (rand()%(hi_num-low_num)) + low_num;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne un point aux positions tirés 
 * aléatoirement entre a et b.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 */
point *alea(int a, int b)
{
  point *p = (point *)malloc(sizeof(point));
  
  p->x = aleatoire(a ,b);
  p->y = aleatoire(a ,b);

  return p;
}

/* ####################################################### */
/* NOEUD */
/* ####################################################### */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Initialise un noeud grâce à un identifiant
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
noeud *initNoeud(int id)
{
  noeud *n = (noeud *)malloc(sizeof(noeud));
  n->id = id;
  n->p = alea(COORD_MIN, COORD_MAX);
  n->es = (espace *)malloc(sizeof(espace));

  if (id == 1) {
    n->es->a.x = COORD_MIN;
    n->es->a.y = COORD_MAX;
      
    n->es->ap.x = COORD_MAX;
    n->es->ap.y = COORD_MAX;

    n->es->b.x = COORD_MIN;
    n->es->b.y = COORD_MIN;

    n->es->bp.x = COORD_MAX;
    n->es->bp.y = COORD_MIN;
  }

  n->haut = NULL;
  n->bas = NULL;
  n->droite = NULL;
  n->gauche = NULL;

  return n;
}
