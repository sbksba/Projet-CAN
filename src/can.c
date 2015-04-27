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

/* ####################################################### */
/* LISTE */
/* ####################################################### */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Initialise une nouvelle liste 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *nouvelleListe(noeud *noeud, liste_noeud *l)
{
  if (!noeud)
    {
      return NULL;
    }
  liste_noeud *liste = (liste_noeud *)malloc(sizeof(liste_noeud));
  liste->n = noeud;
  liste->suivant = l;
  return liste;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Ajoute un noeud dans la liste 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *ajouterNoeud(liste_noeud *liste, noeud *n)
{
  return nouvelleListe(n, liste);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Supprimer un noeud dans la liste 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *supprimerNoeud(liste_noeud *liste, noeud *n)
{
  if (!liste)
    return NULL;

  liste_noeud *prec=NULL, *cour=liste;
  while (cour)
    {
      if (cour->n == n)
	{
	  if (prec)
	    {
	      prec->suivant=cour->suivant;
	    }
	  else if (!liste->suivant)
	    {
	      return NULL;
	    }
	  free(cour);
	  break;
	}
      prec = cour;
      cour = cour->suivant;
    }
  
  if (prec)
    return liste;
  return liste->suivant;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 * Copie une liste 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *copieListe(liste_noeud *liste)
{
  if (!liste)
    return NULL;
  return nouvelleListe(liste->n, copieListe(liste->suivant));
}

/* ####################################################### */
/* INSERTION */
/* ####################################################### */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne le sens de decoupe d'un espace 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int maxEspace(espace *es)
{  
  if ((es->ap.x - es->a.x) >= (es->a.y - es->b.y))
	return VERTICAL;
    else
	return HORIZONTAL;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Renvoie TRUE si b est dans l'espace de a
 * Renvoie FALSE sinon 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne l'espace decoupé du noeud en 
 * paramètre.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
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
	  a->es->a.x = gauche->ap.x;
	  a->es->b.x = gauche->bp.x;
	  free(droit);
	  return gauche;
	}
      else
	{
	  a->es->ap.x = droit->a.x;
	  a->es->bp.x = droit->b.x;
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
      bas->a.y = a->es->a.y - (h/2);
      bas->b = a->es->b;
      bas->ap = a->es->ap;
      bas->ap.y = a->es->ap.y - (h/2);
      bas->bp = a->es->bp;

      if (estDansEspace(haut, a) == TRUE)
	{
	  a->es->b.y = a->es->b.y + (h/2);
	  a->es->bp.y = a->es->bp.y + (h/2);
	  bas->a = a->es->b;
	  bas->ap = a->es->bp;
	  free(haut);
	  return bas;
	}
      else
	{
	  a->es->a.y = a->es->a.y - (h/2);
	  a->es->ap.y = a->es->ap.y - (h/2);
	  haut->b = a->es->a;
	  haut->bp = a->es->ap;
	  free(bas);
	  return haut;
	}
    }
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne la position du noeud dans l'espace
 * en paramètre.
 * Tant que celui-ci n'est pas dans cet espace
 * on réitère l'opération.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
void aleatoireDansEspace (espace *espace, noeud *noeud)
{
  while (estDansEspace(espace, noeud) == FALSE)
    {
      printf("#%d : (%d,%d)\n",noeud->id, noeud->p->x, noeud->p->y);
      if (noeud->p->x <= espace->a.x || noeud->p->x >= espace->ap.x)
	noeud->p->x = aleatoire(espace->a.x, espace->ap.x);
      
      if (noeud->p->y >= espace->a.y || noeud->p->y <= espace->b.y)
	noeud->p->y = aleatoire(espace->a.y, espace->b.y);
    }
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne TRUE si le point p est sur le 
 * segment [a,b].
 * Sinon retourne FALSE.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int estPointDansSegment(point p, point a, point b) {
    //Segment vertical
    if (p.x == a.x && p.x == b.x)
      {
	if (p.y <= a.y && p.y >= b.y)
	  {
	    return TRUE;
	  }
      }
    //Segment horizontal
    else 
      { 
    	if (p.x >= a.x && p.x < b.x)
	  {
	    return TRUE;
	  }
      }
    return FALSE;
}

int appartient(point m, point a, point b)
{
  point p1, p2, p3, p4;
  p1.x = (m.x-a.x); p1.y = (m.y-a.y); /*AM*/
  p2.x = (b.x-a.x); p2.y = (b.y-a.y); /*AB*/
  p3.x = (a.x-m.x); p2.y = (a.y-m.y); /*CA*/
  p4.x = (b.x-m.x); p2.y = (b.y-m.y); /*CB*/
  int tmp1 = (p1.x * p2.y)-(p2.x * p1.y);
  int tmp2 = (p3.x * p4.x)-(p3.y * p4.y);

  /* M est sur la droite AB */
  if (tmp1 == 0)
    {
      /* M est sur le segment AB */
      if (tmp2 <= 0)
	{
	  return TRUE;
	}
    }
  return FALSE;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne TRUE si le segment source [a+1,b-1]
 * est sur le cible segment [a,b].
 * Le sens indique la direction testée
 * exemple : si sens == BAS on teste si la cible
 * est un voisin bas.
 * Sinon retourne FALSE.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int estDansSegment(noeud *source, noeud *cible, int sens)
{
  point tmp1, tmp2;
  if (sens == BAS)
    {
      tmp1.x = source->es->a.x+1;  tmp1.y = source->es->a.y;
      tmp2.x = source->es->ap.x-1; tmp2.y = source->es->ap.y;
      if ( appartient(tmp1, cible->es->b, cible->es->bp) == TRUE
	 ||appartient(tmp2, cible->es->b, cible->es->bp) == TRUE)
	return TRUE;
    }
  else if (sens == HAUT)
    {
      tmp1.x = source->es->b.x+1;  tmp1.y = source->es->b.y;
      tmp2.x = source->es->bp.x-1; tmp2.y = source->es->bp.y;
      if ( estPointDansSegment(tmp1, cible->es->a, cible->es->ap) == TRUE
	 ||estPointDansSegment(tmp2, cible->es->a, cible->es->ap) == TRUE)
	return TRUE;
    }
  else if (sens == DROITE)
    {
      tmp1.x = source->es->a.x; tmp1.y = source->es->a.y-1;
      tmp2.x = source->es->b.x; tmp2.y = source->es->b.y+1;
      if ( estPointDansSegment(tmp1, cible->es->ap, cible->es->bp) == TRUE
	 ||estPointDansSegment(tmp2, cible->es->ap, cible->es->bp) == TRUE)
	return TRUE;
    }
  else if (sens == GAUCHE)
    {
      tmp1.x = source->es->ap.x; tmp1.y = source->es->ap.y-1;
      tmp2.x = source->es->bp.x; tmp2.y = source->es->bp.y+1;
      if ( estPointDansSegment(tmp1, cible->es->a, cible->es->ap) == TRUE
	 ||estPointDansSegment(tmp2, cible->es->a, cible->es->ap) == TRUE)
	return TRUE;
    }
  
  return FALSE;
}

/* Met à jour la liste des noeuds bas du nouveau noeud*/
liste_noeud *estToujoursVoisinB(noeud *ancien, noeud *nouveau) {
      //Pour tous les voisins bas de l'ancien noeud  

      while(ancien->bas)
	{
           if (!(estPointDansSegment(ancien->bas->n->es->a, nouveau->es->b, nouveau->es->bp)
		 && estPointDansSegment(ancien->bas->n->es->ap, nouveau->es->b, nouveau->es->bp))) {
		//On supprime le noeud qui n'est plus connecté
		printf("Noeud à supprimer : %d chez : %d\n", nouveau->id, ancien->bas->n->id);
		supprimerNoeud(ancien->bas->n->haut, nouveau);
		
		printf("Suppression de %d\n", ancien->bas->n->id);
		supprimerNoeud(nouveau->bas, ancien->bas->n);
	   }

	   ancien->bas = ancien->bas->suivant;
	}

	return nouveau->bas;
}

/* Met à jours la liste des noeuds hauts du nouveau noeud */
liste_noeud *estToujoursVoisinH(noeud *ancien, noeud *nouveau) {
      //Pour tous les voisins haut de l'ancien noeud
      while(ancien->haut)
	{

           if (!(estPointDansSegment(ancien->haut->n->es->b, nouveau->es->a, nouveau->es->ap)
               && estPointDansSegment(ancien->haut->n->es->bp, nouveau->es->a, nouveau->es->ap))) {
		//On supprime le noeud qui n'est plus connecté
		supprimerNoeud(nouveau->haut, ancien->haut->n);
		//On supprime le noeud de qui on est plus connecté
		supprimerNoeud(ancien->haut->n->bas, nouveau);
            }

	   ancien->haut = ancien->haut->suivant;
	}
	return nouveau->haut;
}


liste_noeud *estToujoursVoisinD(noeud *ancien, noeud *nouveau) {
      //Pour tous les voisins droits
      while(ancien->droite)
	{

           if (!(estPointDansSegment(ancien->droite->n->es->a, nouveau->es->a, nouveau->es->b)
               && estPointDansSegment(ancien->droite->n->es->b, nouveau->es->a, nouveau->es->b))) {
		//On supprime le noeud qui n'est plus connecté
		supprimerNoeud(nouveau->droite, ancien->droite->n);
		//On supprime le noeud de qui on est plus connecté
		supprimerNoeud(ancien->droite->n->gauche, nouveau);
            }

	   ancien->droite = ancien->droite->suivant;
	}
	return nouveau->droite;
}


liste_noeud *estToujoursVoisinG(noeud *ancien, noeud *nouveau) {
      //Pour tous les voisins gauches
      while(ancien->gauche)
	{

           if (!(estPointDansSegment(ancien->gauche->n->es->a, nouveau->es->a, nouveau->es->b)
               && estPointDansSegment(ancien->gauche->n->es->b, nouveau->es->a, nouveau->es->b))) {
		//On supprime le noeud qui n'est plus connecté
		supprimerNoeud(nouveau->gauche, ancien->gauche->n);
		//On supprime le noeud de qui on est plus connecté
		supprimerNoeud(ancien->gauche->n->droite, nouveau);
            }

	   ancien->gauche = ancien->gauche->suivant;
	}
	return nouveau->gauche;
}

/* C'est le noeud qui est dans l'espace qui se découpe qui
   notifie les autres (noeudA ici) */
void majVoisins(noeud *noeudA, noeud *noeudB, espace *origine)
{
  /* B est a droite*/
  if (noeudA->es->a.x == origine->a.x)
    {
      noeudB->droite = noeudA->droite;
      noeudB->haut = noeudA->haut;
      noeudB->bas = noeudA->bas;
      ajouterNoeud(noeudB->gauche, noeudA);
      ajouterNoeud(noeudA->droite, noeudB);

	//Pour tous les voisins droits de A
      while (noeudA->droite != NULL)
	{
	  //On ajoute B dans leur liste de voisins gauches
	  ajouterNoeud(noeudA->droite->n->gauche, noeudB);
	  noeudA->droite = noeudA->droite->suivant;
	  //On supprime A de leur liste de voisins gauches
          supprimerNoeud(noeudB->droite->n->gauche, noeudA);
	}
    }
  /* B est a gauche*/
  else if (noeudA->es->a.x > origine->a.x)
    {
      noeudB->gauche = noeudA->gauche;
      noeudB->haut = noeudA->haut;
      noeudB->bas = noeudA->bas;
      ajouterNoeud(noeudB->droite, noeudA);
      ajouterNoeud(noeudA->gauche, noeudB);

	//Pour tous les voisins gauches de A
      while (noeudA->gauche != NULL)
	{
	  //On ajoute B dans leur liste de voisins droits
	  ajouterNoeud(noeudA->gauche->n->droite, noeudB);
	  noeudA->droite = noeudA->droite->suivant;
	  //On supprime A de leur liste de voisins droits
          supprimerNoeud(noeudB->gauche->n->droite, noeudA);
	}
    }
  /* B est en bas*/
  else if (noeudA->es->a.y == origine->a.y)
    {
      
    }
  /* B est en haut*/
  else
    {
    }
}


/***********************************************************************************************/
/* TEST */
/***********************************************************************************************/
void printListeNoeud(liste_noeud *liste)
{
  while(liste)
    {
      printf("-> #%d ",liste->n->id);
      liste = liste->suivant;
    }
  printf("\n");
}

void printListe(noeud *noeud)
{
  printf("HAUT : ");
  printListeNoeud(noeud->haut);
  printf("BAS : ");
  printListeNoeud(noeud->bas);
  printf("DROITE : ");
  printListeNoeud(noeud->droite);
  printf("GAUCHE : ");
  printListeNoeud(noeud->gauche);
}

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
  noeud *c = initNoeud(3);
  noeud *d = initNoeud(4);
  noeud *e = initNoeud(5);
  noeud *f = initNoeud(6);
  noeud *g = initNoeud(7);

  a->p->x = 2;  a->p->y = 15;
  b->p->x = 14; b->p->y = 18;
  c->p->x = 12; c->p->y = 2;
  d->p->x = 19; d->p->y = 3;
  e->p->x = 19; e->p->y = 11;
  f->p->x = 17; f->p->y = 16;
  g->p->x = 17; g->p->y = 11;
  
  espace *esB = decoupe(a);
  b = attributionEspace(b, esB);
  aleatoireDansEspace(esB, b);

  espace *esC = decoupe(b);
  c = attributionEspace(c, esC);
  aleatoireDansEspace(esC, c);
  
  espace *esD = decoupe(c);
  d = attributionEspace(d, esD);
  aleatoireDansEspace(esD, d);
  
  espace *esE = decoupe(b);
  e = attributionEspace(e, esE);
  aleatoireDansEspace(esE, e);

  espace *esF = decoupe(e);
  f = attributionEspace(f, esF);
  aleatoireDansEspace(esF, f);
  
  espace *esG = decoupe(e);
  g = attributionEspace(g, esG);
  aleatoireDansEspace(esG, g);
    
  a->droite = ajouterNoeud(a->droite, b);
  a->droite = ajouterNoeud(a->droite, c);
  
  b->bas = ajouterNoeud(b->bas, c);
  b->bas = ajouterNoeud(b->bas, d);
  b->gauche = ajouterNoeud(b->gauche, a);
  
  c->gauche = ajouterNoeud(c->gauche, a);
  c->haut = ajouterNoeud(c->haut, b);
  c->droite = ajouterNoeud(c->droite, d);

  d->haut = ajouterNoeud(d->haut, e);
  d->haut = ajouterNoeud(d->haut, b);
  d->gauche = ajouterNoeud(d->gauche, c);
 
  c->haut = ajouterNoeud(c->haut, e); 
 
  e->bas = copieListe(b->bas);
  e->haut = copieListe(b->haut);
  e->droite = copieListe(b->droite);
  e->gauche = copieListe(b->gauche);
  e->gauche = ajouterNoeud(e->gauche, b);
  
  f->bas = copieListe(e->bas);
  f->haut = copieListe(e->haut);
  f->droite = copieListe(e->droite);
  f->gauche = copieListe(e->gauche);
  f->bas = ajouterNoeud(f->bas, e);

  printEspace(a);
  printEspace(b);
  printEspace(c);
  printEspace(d);
  printEspace(e);
  printEspace(f);
  printEspace(g);

  printf("\nA noeud %d de coordonnées (%d, %d)\n", a->id, a->p->x, a->p->y);
  printf("B noeud %d de coordonnées (%d, %d)\n", b->id, b->p->x, b->p->y);
  printf("C noeud %d de coordonnées (%d, %d)\n", c->id, c->p->x, c->p->y);
  printf("D noeud %d de coordonnées (%d, %d)\n", d->id, d->p->x, d->p->y);
  printf("E noeud %d de coordonnées (%d, %d)\n", e->id, e->p->x, e->p->y);
  printf("F noeud %d de coordonnées (%d, %d)\n", f->id, f->p->x, f->p->y);
  printf("G noeud %d de coordonnées (%d, %d)\n\n", g->id, g->p->x, g->p->y);

  if (estDansSegment(d, e, BAS) == TRUE) 
    printf("D est un voisin (bas) de E\n");
  else
    printf("D n'est pas un voisin (bas) de E\n");

  if (estDansSegment(d, g, BAS) == TRUE) 
    printf("D est un voisin (bas) de G\n");
  else
    printf("D n'est pas un voisin (bas) de G\n");

  if (estDansSegment(e, d, HAUT) == TRUE) 
    printf("E est un voisin (haut) de D\n");
  else
    printf("E n'est pas un voisin (haut) de D\n");

  if (estDansSegment(g, d, HAUT) == TRUE) 
    printf("G est un voisin (haut) de D\n");
  else
    printf("G n'est pas un voisin (haut) de D\n");

  if (estDansSegment(c, e, BAS) == TRUE) 
    printf("C est un voisin (bas) de E\n");
  else
    printf("C n'est pas un voisin (bas) de E\n");

  if (estDansSegment(c, g, BAS) == TRUE) 
    printf("C est un voisin (bas) de G\n");
  else
    printf("C n'est pas un voisin (bas) de G\n");
 
  if (estDansSegment(c, b, BAS) == TRUE) 
    printf("C est un voisin (bas) de B\n");
  else
    printf("C n'est pas un voisin (bas) de B\n");
  
  if (estDansSegment(b, d, HAUT) == TRUE) 
    printf("B est un voisin (haut) de D\n");
  else
    printf("B n'est pas un voisin (haut) de D\n");

  if (estDansSegment(a, b, GAUCHE) == TRUE) 
    printf("A est un voisin (gauche) de B\n");
  else
    printf("A n'est pas un voisin (gauche) de B\n");
  
  if (estDansSegment(a, c, GAUCHE) == TRUE) 
    printf("A est un voisin (gauche) de C\n");
  else
    printf("A n'est pas un voisin (gauche) de C\n");

  if (estDansSegment(b, f, GAUCHE) == TRUE) 
    printf("B est un voisin (gauche) de F\n");
  else
    printf("B n'est pas un voisin (gauche) de F\n");

  if (estDansSegment(b, g, GAUCHE) == TRUE) 
    printf("B est un voisin (gauche) de G\n");
  else
    printf("B n'est pas un voisin (gauche) de G\n");

  if (estDansSegment(f, b, DROITE) == TRUE) 
    printf("F est un voisin (droite) de B\n");
  else
    printf("F n'est pas un voisin (droite) de B\n");

  if (estDansSegment(g, b, DROITE) == TRUE) 
    printf("G est un voisin (droite) de B\n");
  else
    printf("G n'est pas un voisin (droite) de B\n");
  
  if (estDansSegment(e, b, DROITE) == TRUE) 
    printf("E est un voisin (droite) de B\n");
  else
    printf("E n'est pas un voisin (droite) de B\n");

  if (estDansSegment(g, e, GAUCHE) == TRUE) 
    printf("G est un voisin (gauche) de E\n");
  else
    printf("G n'est pas un voisin (gauche) de E\n");
  
  if (estDansSegment(e, g, DROITE) == TRUE) 
    printf("E est un voisin (droite) de G\n");
  else
    printf("E n'est pas un voisin (droite) de G\n");

  if (estDansSegment(c, a, DROITE) == TRUE) 
    printf("C est un voisin (droite) de A\n");
  else
    printf("C n'est pas un voisin (droite) de A\n");

  /*
  printf("\n");
  printRect(a);
  printRect(b);
  printRect(c);
  printRect(d);
  printRect(e);
  printRect(f);
  printRect(g);

  printf("%d %d A\n", a->p->x, a->p->y);
  printf("%d %d B\n", b->p->x, b->p->y);
  printf("%d %d C\n", c->p->x, c->p->y);
  printf("%d %d D\n", d->p->x, d->p->y);
  printf("%d %d E\n", e->p->x, e->p->y);
  printf("%d %d F\n", f->p->x, f->p->y);
  printf("%d %d G\n", g->p->x, g->p->y);
  */

  return EXIT_SUCCESS;
}

