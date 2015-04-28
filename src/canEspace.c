#include "canEspace.h"

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
 * Attribut a un noeud son espace. 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
noeud *attributionEspace(noeud *noeud, espace *espace)
{
  noeud->es->a = espace->a;
  noeud->es->ap = espace->ap;
  noeud->es->b = espace->b;
  noeud->es->bp = espace->bp;

  return noeud;
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
 * Retourne TRUE si le point m est sur le 
 * segment [a,b].
 * Sinon retourne FALSE.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
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
 * Retourne TRUE si le noeud source est sur le 
 * segment [a,b] du noeud cible.
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
      if ( appartient(tmp1, cible->es->a, cible->es->ap) == TRUE
	   ||appartient(tmp2, cible->es->a, cible->es->ap) == TRUE)
	return TRUE;
    }
  else if (sens == DROITE)
    {
      tmp1.x = source->es->a.x; tmp1.y = source->es->a.y-1;
      tmp2.x = source->es->b.x; tmp2.y = source->es->b.y+1;
      if ( appartient(tmp1, cible->es->ap, cible->es->bp) == TRUE
	   ||appartient(tmp2, cible->es->ap, cible->es->bp) == TRUE)
	return TRUE;
    }
  else if (sens == GAUCHE)
    {
      tmp1.x = source->es->ap.x; tmp1.y = source->es->ap.y-1;
      tmp2.x = source->es->bp.x; tmp2.y = source->es->bp.y+1;
      if ( appartient(tmp1, cible->es->a, cible->es->b) == TRUE
	   ||appartient(tmp2, cible->es->a, cible->es->b) == TRUE)
	return TRUE;
    }
  
  return FALSE;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Met à jour la liste des noeuds bas de noeud
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *estToujoursVoisinB(noeud *noeud)
{
  liste_noeud *liste = copieListe(noeud->bas);
  printListe(noeud);
  
  //Pour tous les voisins bas de noeud
  while(liste)
    {
      if (estDansSegment(liste->n, noeud, BAS) == FALSE)
	{
	  printf("EST B : %d\n",liste->n->id);
	  //On supprime le noeud qui n'est plus connecté
	  noeud->bas->n->haut = supprimerNoeud(noeud->bas->n->haut, noeud);
	  noeud->bas = supprimerNoeud(noeud->bas, liste->n);
	}
      liste = liste->suivant;
    }
  
  return noeud->bas;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Met à jour la liste des noeuds haut de noeud
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *estToujoursVoisinH(noeud *noeud)
{
  liste_noeud *liste = copieListe(noeud->haut);
  //Pour tous les voisins haut de noeud
  while(liste)
    {
      if (estDansSegment(liste->n, noeud, HAUT) == FALSE)
	{
	  //On supprime le noeud qui n'est plus connecté
	  noeud->haut->n->bas = supprimerNoeud(noeud->haut->n->bas, noeud);
	  noeud->haut = supprimerNoeud(noeud->haut, liste->n);
	}
      liste = liste->suivant;
    }
  
  return noeud->haut;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Met à jour la liste des noeuds droit de noeud
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *estToujoursVoisinD(noeud *noeud)
{
  liste_noeud *liste = copieListe(noeud->droite);
  //Pour tous les voisins droit de noeud
  while(liste)
    {
      if (estDansSegment(liste->n, noeud, DROITE) == FALSE)
	{
	  //On supprime le noeud qui n'est plus connecté
	  noeud->droite->n->gauche = supprimerNoeud(noeud->droite->n->gauche, noeud);
	  noeud->droite = supprimerNoeud(noeud->droite, liste->n);
	}
      liste = liste->suivant;
    }
  
  return noeud->droite;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Met à jour la liste des noeuds gauche de noeud
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
liste_noeud *estToujoursVoisinG(noeud *noeud)
{
  liste_noeud *liste = copieListe(noeud->gauche);
  //Pour tous les voisins gauche de noeud
  while(liste)
    {
      if (estDansSegment(liste->n, noeud, GAUCHE) == FALSE)
	{
	  //On supprime le noeud qui n'est plus connecté
	  noeud->gauche->n->droite = supprimerNoeud(noeud->gauche->n->droite, noeud);
	  noeud->gauche = supprimerNoeud(noeud->gauche, liste->n);
	}
      liste = liste->suivant;
    }
  
  return noeud->gauche;
}

void printEspace(noeud *noeud)
{
  printf("#%d   [ (%d,%d) (%d,%d) (%d,%d) (%d,%d) ]\n",noeud->id,noeud->es->a.x,noeud->es->a.y,noeud->es->ap.x,noeud->es->ap.y,noeud->es->b.x,noeud->es->b.y,noeud->es->bp.x,noeud->es->bp.y);
}


void printRect(noeud *noeud)
{
  printf("set object %d rect from %d,%d to %d,%d fc rgb \"white\"\n",noeud->id, noeud->es->b.x, noeud->es->b.y, noeud->es->ap.x, noeud->es->ap.y);
}
