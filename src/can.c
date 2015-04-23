#include "can.h"

/* ************ */
/*  PRIMITIVES  */
/* ************ */

/* POINT */
/* Tire aléatoirement un point entre a et b
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

point *alea(int a, int b)
{
  point *p = (point *)malloc(sizeof(point));
  
  p->x = aleatoire(a ,b);
  p->y = aleatoire(a ,b);

 return p;
}

/* NOEUD */

/*Initialise un noeud grâce à un identifiant*/
noeud *initNoeud(int id)
{
  noeud *n = (noeud *)malloc(sizeof(noeud));
  n->id = id;
  n->p = (point *)malloc(sizeof(point));
  n->p  = alea(COORD_MIN, COORD_MAX);
  n->es = (espace *)malloc(sizeof(espace));

  if (id == 1) {
     n->es->a.x = 0;
     n->es->a.y = 1000;
      
     n->es->ap.x = 1000;
     n->es->ap.y = 1000;

     n->es->b.x = 0;
     n->es->b.y = 0;

     n->es->bp.x = 1000;
     n->es->bp.y = 0;
  }

  n->nord = nouvelleListe();
  n->sud = nouvelleListe();
  n->est = nouvelleListe();
  n->ouest = nouvelleListe();

  return n;
}

/* LISTE */

/* Initialise une nouvelle liste */
liste_noeud *nouvelleListe()
{
  liste_noeud *liste = (liste_noeud *)malloc(sizeof(liste_noeud));
  liste->n = NULL;
  liste->suivant = NULL;
  return liste;
}

/* Ajoute un noeud dans la liste */
void ajouterNoeud(liste_noeud *liste, noeud *n)
{
  liste->n = n;
  liste->suivant = NULL;
}

/* INSERTION */

/* Retourne le sens de decoupe d'un espace */
int maxEspace(espace *es)
{  
  if ((es->ap.x - es->a.x) >= (es->a.y - es->b.y))
	return VERTICAL;
    else
	return HORIZONTAL;
}

/*
  Renvoie TRUE si b est dans l'espace de a
  Renvoie FALSE sinon 
*/
int estDansEspace(espace *es, noeud *b) {
   //Si le point correspondant à b est inclus dans l'espace
   //correspondant à a :
 
   if ((es->a.x < b->p->x) && (b->p->x < es->ap.x)
    && (es->b.x < b->p->x) && (b->p->x < es->bp.x)
    && (es->a.y > b->p->y) && (b->p->y < es->ap.y)
    && (es->b.y < b->p->y) && (b->p->y > es->bp.y))
	return TRUE;
   else
	return FALSE;
}

espace *decoupe(noeud *a)
{
  int l,h;
  if (maxEspace(a->es) == VERTICAL)
    {
      espace *droit  = malloc(sizeof(espace));
      espace *gauche = malloc(sizeof(espace));
      l = (a->es->ap.x - a->es->a.x);
           
      droit->ap = a->es->ap;
      droit->bp = a->es->bp;
      droit->a = a->es->a;
      droit->b = a->es->b;
      droit->a.x = a->es->a.x + (l/2);
      droit->b.x = a->es->b.x + (l/2);
      
      gauche->a = a->es->a;
      gauche->b = a->es->b;
      gauche->ap = a->es->ap;
      gauche->bp = a->es->bp;
      gauche->ap.x = a->es->ap.x - (l/2);
      gauche->bp.x = a->es->bp.x - (l/2);

      if (estDansEspace(droit, a) == TRUE)
	{
	  a->es->a.x = a->es->a.x + (l/2);
	  a->es->b.x = a->es->b.x + (l/2);
	  free(droit);
	  return gauche;
	}
      else
	{
	  a->es->ap.x = a->es->a.x + (l/2);
	  a->es->bp.x = a->es->b.x + (l/2);
	  free(gauche);
	  return droit;
	}
    }
  else
    {
      espace *haut  = malloc(sizeof(espace));
      espace *bas = malloc(sizeof(espace));
      h = (a->es->a.y - a->es->b.y);
      
      haut->ap = a->es->ap;
      haut->bp = a->es->bp;
      haut->bp.y = a->es->bp.y + (h/2);
      haut->a = a->es->a;
      haut->b = a->es->b;
      haut->b.y = a->es->b.y + (h/2);
      
      bas->a = a->es->a;
      bas->a.y = (h/2);
      bas->b = a->es->b;
      bas->ap = a->es->ap;
      bas->ap.y = (h/2);
      bas->bp = a->es->bp;

      if (estDansEspace(haut, a) == TRUE)
	{
	  a->es->b.y = a->es->b.y + (h/2);
	  a->es->bp.y = a->es->bp.y + (h/2);
	  free(haut);
	  return bas;
	}
      else
	{
	  a->es->a.y = (h/2);
	  a->es->ap.y = (h/2);
	  free(bas);
	  return haut;
	}
    }
}

/* Retourne les coordonnées du noeud dans le nouvel espace */
void aleatoireDansEspace (espace *espace, noeud *noeud)
{
  while (estDansEspace(espace, noeud) == FALSE)
    {
      if (noeud->p->x <= espace->a.x || noeud->p->x >= espace->ap.x)
	noeud->p->x = aleatoire(espace->a.x, espace->ap.x);
      
      if (noeud->p->y >= espace->a.y || noeud->p->y <= espace->b.y)
	noeud->p->y = aleatoire(espace->a.y, espace->b.y);
    }
}

/*
int envoyer(int id_recepteur, int type, void *message, int id_emetteur) {}
int recevoir(int id_recepteur, int type, int message, int id_emetteur) {}
*/

void printEspace(noeud *noeud)
{
  printf("#%d   [ (%d,%d) (%d,%d) (%d,%d) (%d,%d) ]\n",noeud->id,noeud->es->a.x,noeud->es->a.y,noeud->es->ap.x,noeud->es->ap.y,noeud->es->b.x,noeud->es->b.y,noeud->es->bp.x,noeud->es->bp.y);
}

noeud *attributionEspace(noeud *noeud, espace *espace)
{
  noeud->es->a = espace->a;
  noeud->es->ap = espace->ap;
  noeud->es->b = espace->b;
  noeud->es->bp = espace->bp;

  return noeud;
}

void printRect(noeud *noeud)
{
  printf("set object %d rect from %d,%d to %d,%d fc rgb \"white\"\n",noeud->id, noeud->es->b.x, noeud->es->b.y, noeud->es->ap.x, noeud->es->ap.y);
}

int main (int argc, char **argv) {

  noeud *a = initNoeud(1);
  noeud *b = initNoeud(2);

  //Si b est dans l'espace du BOOTSTRAP
  if (estDansEspace(a->es, b) == TRUE)
    {
      printf("#%d est dans l'espace de #%d\n",b->id, a->id);
      espace *newEspace = decoupe(a);
      b = attributionEspace(b, newEspace);
      printf("A : (%d, %d)\n",a->p->x, a->p->y);
      printf("B : (%d, %d)\n",b->p->x, b->p->y);
      printEspace(a);
      printEspace(b);
      aleatoireDansEspace(newEspace, b);
    }
  printf("\nA noeud %d de coordonnées (%d, %d)\n", a->id, a->p->x, a->p->y);
  printf("B noeud %d de coordonnées (%d, %d)\n", b->id, b->p->x, b->p->y);
/*
  noeud *c = initNoeud(3); 
  noeud *d = initNoeud(4);
  noeud *e = initNoeud(5);
    
  printf("C noeud %d de coordonnées (%d, %d)\n", c->id, c->p->x, c->p->y);
  printf("D noeud %d de coordonnées (%d, %d)\n", d->id, d->p->x, d->p->y);
  printf("E noeud %d de coordonnées (%d, %d)\n", e->id, e->p->x, e->p->y);
    
  espace *espaceB = malloc(sizeof(espace));
  espace *espaceC = malloc(sizeof(espace));
  espace *espaceD = malloc(sizeof(espace));
  espace *espaceE = malloc(sizeof(espace));
    
  espaceB = decoupe(a);
  b = attributionEspace(b,espaceB);
  
  espaceC = decoupe(b);
  c = attributionEspace(c,espaceC);
    
  espaceD = decoupe(a);
  d = attributionEspace(d,espaceD);
  
  espaceE = decoupe(b);
  e = attributionEspace(e,espaceE);
        
    printRect(a);
    printRect(b);
    printRect(c);
    printRect(d);
    printRect(e);
    
  */
  return EXIT_SUCCESS;
}

