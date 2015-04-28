#include "canListe.h"

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

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
 * Retourne un liste vide. 
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
void supprimerListe(liste_noeud *liste)
{
  if (liste)
    {
      supprimerListe(liste->suivant);
      free(liste);
    }  
}

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
  printf("\n");
}
