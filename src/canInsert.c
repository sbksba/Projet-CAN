#include "canInsert.h"

/* ####################################################### */
/* INSERTION */
/* ####################################################### */

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

void maj(noeud *maj)
{
  maj->bas    = estToujoursVoisinB(maj);
  maj->haut   = estToujoursVoisinH(maj);
  maj->droite = estToujoursVoisinD(maj);
  maj->gauche = estToujoursVoisinG(maj);;
}

void insertD(noeud *cible, noeud *insert)
{
  liste_noeud *liste;
  printf("insertD : #%d\n",insert->id);
  insert->gauche = ajouterNoeud(insert->gauche, cible);
  insert->droite = copieListe(cible->droite);
  insert->haut = copieListe(cible->haut);
  insert->bas = copieListe(cible->bas);
  
  if (cible->haut != NULL)
    {
      liste = copieListe(cible->haut);
      while(liste)
	{
	  printf("H Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->haut->n->bas = ajouterNoeud(liste->n->bas, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->bas != NULL)
    {
      liste = copieListe(cible->bas);
      while(liste != NULL)
	{
	  printf("B Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->bas->n->haut = ajouterNoeud(liste->n->haut, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->droite != NULL)
    {
      liste = copieListe(cible->droite);
      while(liste)
	{
	  printf("D Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->droite->n->gauche = ajouterNoeud(liste->n->gauche, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->gauche != NULL)
    {
      liste = copieListe(cible->gauche);
      while(liste)
	{
	  printf("G Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->gauche->n->droite = ajouterNoeud(liste->n->droite, insert);
	  liste = liste->suivant;
	}
    }

  printf("\n");
  cible->droite = ajouterNoeud(cible->droite, insert);
}

void insertG(noeud *cible, noeud *insert)
{
  liste_noeud *liste;
  printf("insertG : #%d\n",insert->id);
  insert->gauche = copieListe(cible->gauche);
  insert->droite = ajouterNoeud(insert->droite, cible);
  insert->haut = copieListe(cible->haut);
  insert->bas = copieListe(cible->bas);

  if (cible->haut != NULL)
    {
      liste = copieListe(cible->haut);
      while(liste)
	{
	  printf("H Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->haut->n->bas = ajouterNoeud(liste->n->bas, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->bas != NULL)
    {
      liste = copieListe(cible->bas);
      while(liste)
	{
	  printf("B Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->bas->n->haut = ajouterNoeud(liste->n->haut, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->droite != NULL)
    {
      liste = copieListe(cible->droite);
      while(liste)
	{
	  printf("D Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->droite->n->gauche = ajouterNoeud(liste->n->gauche, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->gauche != NULL)
    {
      liste = copieListe(cible->gauche);
      while(liste)
	{
	  printf("G Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->gauche->n->droite = ajouterNoeud(liste->n->droite, insert);
	  liste = liste->suivant;
	}
    }

  printf("\n");
  cible->gauche = ajouterNoeud(cible->gauche, insert);
}

void insertH(noeud *cible, noeud *insert)
{
  liste_noeud *liste;
  printf("insertH : #%d\n",insert->id);
  insert->gauche = copieListe(cible->gauche);
  insert->droite = copieListe(cible->droite);
  insert->haut = copieListe(cible->haut);
  insert->bas = ajouterNoeud(insert->bas, cible);
 
  if (cible->haut != NULL)
    {
      liste = copieListe(cible->haut);
      while(liste)
	{
	  printf("H Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->haut->n->bas = ajouterNoeud(liste->n->bas, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->bas != NULL)
    {
      liste = copieListe(cible->bas);
      while(liste)
	{
	  printf("B Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->bas->n->haut = ajouterNoeud(liste->n->haut, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->droite != NULL)
    {
      liste = copieListe(cible->droite);
      while(liste)
	{
	  printf("D Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->droite->n->gauche = ajouterNoeud(liste->n->gauche, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->gauche != NULL)
    {
      liste = copieListe(cible->gauche);
      while(liste)
	{
	  printf("G Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->gauche->n->droite = ajouterNoeud(liste->n->droite, insert);
	  liste = liste->suivant;
	}
    }

  printf("\n");
  cible->haut = ajouterNoeud(cible->haut, insert);
}

void insertB(noeud *cible, noeud *insert)
{
  liste_noeud *liste;
  printf("insertB : #%d\n",insert->id);
  insert->gauche = copieListe(cible->gauche);
  insert->droite = copieListe(cible->droite);
  insert->haut = ajouterNoeud(insert->haut, cible);
  insert->bas = copieListe(cible->bas);
  
  if (cible->haut != NULL)
    {
      liste = copieListe(cible->haut);
      while(liste)
	{
	  printf("H Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->haut->n->bas = ajouterNoeud(liste->n->bas, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->bas != NULL)
    {
      liste = copieListe(cible->bas);
      while(liste)
	{
	  printf("B Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->bas->n->haut = ajouterNoeud(liste->n->haut, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->droite != NULL)
    {
      liste = copieListe(cible->droite);
      while(liste)
	{
	  printf("D Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->droite->n->gauche = ajouterNoeud(liste->n->gauche, insert);
	  liste = liste->suivant;
	}
    }

  if (cible->gauche != NULL)
    {
      liste = copieListe(cible->gauche);
      while(liste)
	{
	  printf("G Ajout de %d dans %d\n",insert->id, liste->n->id);
	  cible->gauche->n->droite = ajouterNoeud(liste->n->droite, insert);
	  liste = liste->suivant;
	}
    }

  printf("\n");
  cible->bas = ajouterNoeud(cible->bas, insert);
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Insert un noeud dans la zone du noeud cible.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
void insertion(noeud *cible, noeud *insert, int dir)
{
  espace *esNoeud = decoupe(cible);
  insert = attributionEspace(insert, esNoeud);
  aleatoireDansEspace(esNoeud, insert);
  
  switch(dir)
    {
    case DROITE:
      /* Insertion du noeud a la droite de la cible */
      insertD(cible, insert);
      
      break;
    case GAUCHE:
      /* Insertion du noeud a la gauche de la cible */
      insertG(cible, insert);
      
      break;
    case HAUT:
      /* Insertion du noeud en haut de la cible */
      insertH(cible, insert);
      
      break;
    case BAS:
      /* Insertion du noeud en bas de la cible */
      insertB(cible, insert);
      
      break;
    }
}

/* ####################################################### */
/* RECHERCHE */
/* ####################################################### */

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne la direction de déplacement.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int direction(noeud *courant, noeud *recherche)
{
  int tab[2];
  if (courant->p->x < recherche->p->x && courant->droite != NULL)
    {
      tab[0] = DROITE;
    }
  else if (courant->p->x > recherche->p->x && courant->gauche != NULL)
    {
      tab[0] = GAUCHE;
    }
  
  if (courant->p->y < recherche->p->y && courant->haut != NULL)
    {
      tab[1] = HAUT;
    }
  else if (courant->p->y > recherche->p->y && courant->bas != NULL)
    {
      tab[1] = BAS;
    }

  return tab[aleatoire(0,1)];
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne TRUE si le noeud a été trouvé.
 * Retourne FALSE sinon.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int recherche (noeud *noeud)
{
  
  return FALSE;
}
