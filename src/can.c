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
     n->es->a.x = COORD_MIN;
     n->es->a.y = COORD_MAX;
      
     n->es->ap.x = COORD_MAX;
     n->es->ap.y = COORD_MAX;

     n->es->b.x = COORD_MIN;
     n->es->b.y = COORD_MIN;

     n->es->bp.x = COORD_MAX;
     n->es->bp.y = COORD_MIN;
  }

  n->haut = nouvelleListe();
  n->bas = nouvelleListe();
  n->droite = nouvelleListe();
  n->gauche = nouvelleListe();

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
liste_noeud *ajouterNoeud(liste_noeud *liste, noeud *n)
{
  if (!liste->n)
    {
      liste->n = n;
      return liste;
    }
  liste_noeud *l = (liste_noeud *)malloc(sizeof(liste_noeud));
  l->n = n;
  l->suivant = liste;
  
  return l;
}

/* Supprimer un noeud dans la liste */
liste_noeud *supprimerNoeud(liste_noeud *liste, noeud *n)
{
  liste_noeud *prec=NULL, *cour=liste;
  while (cour)
    {
      if (cour->n == n)
	{
	  if (prec)
	    {
	      prec->suivant=cour->suivant;
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

/* */
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


int estPointDansSegment(point p, point a, point b) {
    //Segment vertical
    if (p.x == a.x && p.x == b.x){
	if (p.y >= a.y && p.y <= b.y)
           return TRUE;
    }

    //Segment horizontal
    else {  
       if (p.x >= a.x && p.x <= b.x)
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
		//On supprime le noeud de qui on est plus connecté
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
/***********************************************************************************************/
void printListeNoeud(liste_noeud *liste)
{
  if (!liste->n)
    {
      printf("empty\n");
    }
  else
    {
      while(liste)
	{
	  printf("-> #%d ",liste->n->id);
	  liste = liste->suivant;
	}
      printf("\n");
    }
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

  /*
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
  */

  espace *esB = decoupe(a);
  b = attributionEspace(b, esB);
  aleatoireDansEspace(esB, b);
  printf("b : (%d, %d)n",b->p->x, b->p->y);
  printEspace(b);
  
  espace *esC = decoupe(b);
  c = attributionEspace(c, esC);
  aleatoireDansEspace(esC, c);
  
  espace *esD = decoupe(c);
  d = attributionEspace(d, esD);
  aleatoireDansEspace(esD, d);
  
  espace *esE = decoupe(b);
  e = attributionEspace(e, esE);
  aleatoireDansEspace(esE, e);
  
  printEspace(a);
  printEspace(b);
  printEspace(c);
  printEspace(d);
  printEspace(e);

    printRect(a);
    printRect(b);
    printRect(c);
    printRect(d);
    printRect(e);
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
 
  e->droite = b->droite;
  e->bas = b->bas;
  e->gauche = b->gauche;
  e->haut = b->haut;
  

  printf("\nA noeud %d de coordonnées (%d, %d)\n", a->id, a->p->x, a->p->y);
  printListe(a);
  printf("B noeud %d de coordonnées (%d, %d)\n", b->id, b->p->x, b->p->y);
  printListe(b);
  printf("C noeud %d de coordonnées (%d, %d)\n", c->id, c->p->x, c->p->y);
  printListe(c);
  printf("D noeud %d de coordonnées (%d, %d)\n", d->id, d->p->x, d->p->y);
  printListe(d);
  printf("E noeud %d de coordonnées (%d, %d)\n", e->id, e->p->x, e->p->y);
  printListe(e);
  
  e->bas = estToujoursVoisinB(b, e);

  
  printf("\nA noeud %d de coordonnées (%d, %d)\n", a->id, a->p->x, a->p->y);
  printListe(a);
  printf("B noeud %d de coordonnées (%d, %d)\n", b->id, b->p->x, b->p->y);
 // printListe(b);
  printf("C noeud %d de coordonnées (%d, %d)\n", c->id, c->p->x, c->p->y);
  printListe(c);
  printf("D noeud %d de coordonnées (%d, %d)\n", d->id, d->p->x, d->p->y);
  printListe(d);
  printf("E noeud %d de coordonnées (%d, %d)\n", e->id, e->p->x, e->p->y);
  printListe(e);

 /*printListe(b);*/
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

