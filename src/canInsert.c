#include "canInsert.h"

/* ####################################################### */
/* PRIMITIVES MPI */
/* ####################################################### */

int sizeList(liste_noeud *liste)
{
  int size=0;
  liste_noeud *l = liste;
  while(l != NULL)
    {
      size++;
      l = l->suivant;
    }
  free(l);
  return size;
}

/* Convertit un noeud sous forme d'un tableau pour l'envoi */
int *convertirNoeud(noeud *noeud)
{
  int *tab = malloc(sizeof(int)*SIZE_NOEUD);
  tab[0]  = noeud->id;
  tab[1]  = noeud->p->x;
  tab[2]  = noeud->p->y;
  tab[3]  = noeud->es->a.x;
  tab[4]  = noeud->es->a.y;
  tab[5]  = noeud->es->ap.x;
  tab[6]  = noeud->es->ap.y;
  tab[7]  = noeud->es->b.x;
  tab[8]  = noeud->es->b.y;
  tab[9]  = noeud->es->bp.x;
  tab[10] = noeud->es->bp.y;

  return tab;
}

/* Convertit un tableau en un noeud pour la reception */
noeud *convertirTab(int *tab)
{
  noeud *noeud    = initNoeud(tab[0]);
  noeud->p->x     = tab[1];
  noeud->p->y     = tab[2];
  noeud->es->a.x  = tab[3];
  noeud->es->a.y  = tab[4];
  noeud->es->ap.x = tab[5];
  noeud->es->ap.y = tab[6];
  noeud->es->b.x  = tab[7];
  noeud->es->b.y  = tab[8];
  noeud->es->bp.x = tab[9];
  noeud->es->bp.y = tab[10];

  return noeud;
}

int envoyer(int id_destinataire, int tag)
{
  int msg = 0;
  return MPI_Send(&msg, 1, MPI_INT, id_destinataire, tag, MPI_COMM_WORLD);
}

int recevoir(int emetteur, int tag)
{
  int msg;
  MPI_Status status;
  return MPI_Recv(&msg, 1, MPI_INT, emetteur, tag, MPI_COMM_WORLD, &status);
}

int envoyerNoeud(int id_destinataire, int tag, noeud * noeud)
{
  int *tabNoeud = convertirNoeud(noeud);
  return MPI_Send(tabNoeud, SIZE_NOEUD, MPI_INT, id_destinataire, tag, MPI_COMM_WORLD);
}

noeud *recevoirNoeud(int tag)
{
  int *msg = malloc(sizeof(int)*SIZE_NOEUD);
  MPI_Status status;

  MPI_Recv(msg, SIZE_NOEUD, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
  noeud *n = convertirTab(msg);
  //Appel des fonctions d'insertion
  return n;
}

int envoyerVoisins(int id_destinataire, noeud *emetteur)
{
  envoyer(id_destinataire, DEB);
  
  while(emetteur->haut != NULL)
    {
      envoyerNoeud(id_destinataire, TAG_VOIS_HAUT,emetteur->haut->n);
      emetteur->haut = emetteur->haut->suivant;
    }
  
  while(emetteur->bas != NULL)
    {
      envoyerNoeud(id_destinataire, TAG_VOIS_BAS,emetteur->bas->n);
      emetteur->bas = emetteur->bas->suivant;
    }
  
  while(emetteur->gauche != NULL)
    {
      envoyerNoeud(id_destinataire, TAG_VOIS_GAUCHE,emetteur->gauche->n);
      emetteur->gauche = emetteur->gauche->suivant;
    }

  while(emetteur->droite != NULL)
    {
      envoyerNoeud(id_destinataire, TAG_VOIS_DROITE,emetteur->droite->n);
      emetteur->droite = emetteur->droite->suivant;
    }
  
  envoyer(id_destinataire, FIN);

  return EXIT_SUCCESS;
}

int recevoirVoisins(noeud *destinataire)
{
  MPI_Status status;
  int *msg = malloc(sizeof(int)*SIZE_NOEUD);
  MPI_Recv(msg, 1 , MPI_INT, MPI_ANY_SOURCE, DEB, MPI_COMM_WORLD, &status);
  
  while (status.MPI_TAG != FIN)
    {
      MPI_Recv(msg, SIZE_NOEUD, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      noeud *n = convertirTab(msg);

      if (status.MPI_TAG == TAG_VOIS_HAUT)
	destinataire->haut = ajouterNoeud(destinataire->haut, n);
      
      if (status.MPI_TAG == TAG_VOIS_BAS)
	destinataire->bas = ajouterNoeud(destinataire->bas, n);

      if (status.MPI_TAG == TAG_VOIS_GAUCHE)
	destinataire->gauche = ajouterNoeud(destinataire->gauche, n);

      if (status.MPI_TAG == TAG_VOIS_DROITE)
	destinataire->droite = ajouterNoeud(destinataire->droite, n);
    }
  
  return EXIT_SUCCESS;
}

/* ####################################################### */
/* INSERTION */
/* ####################################################### */

/* C'est le noeud qui est dans l'espace qui se découpe qui
   notifie les autres (noeudA ici) */
void majVoisins(noeud *noeudA, noeud *noeudB, espace *origine){}
void maj(noeud *maj){}
void insertD(noeud *cible, noeud *insert){}
void insertG(noeud *cible, noeud *insert){}
void insertH(noeud *cible, noeud *insert){}
void insertB(noeud *cible, noeud *insert){}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Insert un noeud dans la zone du noeud cible.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
void insertion(noeud *cible, noeud *insert)
{
  /* Si le noeud qui veut s'insérer est dans mon espace,
     je découpe cet espace,
     je lui affecte "localement", puis je lui envoie
     je lui envoie tous mes voisins
     je mets à jour mes voisins
     je lui confirme qu'il est inséré.
   */
  if (estDansEspace(cible->es, insert) == TRUE)
    {
      espace *esNoeud = decoupe(cible);
      insert = attributionEspace(insert, esNoeud);
      aleatoireDansEspace(esNoeud, insert);
      printf("ATTRIBUTION : #%d (%d, %d)\n",insert->id,insert->p->x,insert->p->y);
      
      envoyerNoeud(insert->id, TAG_NOEUD, insert);
      /*
      envoyerVoisins(insert->id, cible);

      cible->haut = estToujoursVoisinH(cible);
      cible->bas = estToujoursVoisinB(cible);
      cible->gauche = estToujoursVoisinG(cible);
      cible->droite = estToujoursVoisinD(cible);
      */

      envoyer(insert->id, TAG_TOI_INSERE);
    }

  /* Si le noeud qui veut s'insérer n'est pas dans mon espace, 
     Je cherche la direction vers qui envoyer,
     J'envoie la requête de ce noeud dans cette direction.
   */
  else
    {
      int dir = direction(cible, insert);
      printf("DIR %d\n",dir);
      if (dir == HAUT)
	envoyerNoeud(cible->haut->n->id, TAG_ESPACE, insert);
      else if (dir == BAS)
	envoyerNoeud(cible->bas->n->id, TAG_ESPACE, insert);
      else if (dir == GAUCHE)
	envoyerNoeud(cible->gauche->n->id, TAG_ESPACE, insert);
      else if (dir == DROITE)
	envoyerNoeud(cible->droite->n->id, TAG_ESPACE, insert);
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
  int choix;

  printf("\tC(%d,%d) et R(%d,%d)\n",courant->p->x,courant->p->y,recherche->p->x,recherche->p->y);
  
  if (recherche->p->x > courant->p->x &&
      recherche->p->y == courant->p->y)
    return DROITE;
  
  if (recherche->p->x < courant->p->x &&
      recherche->p->y == courant->p->y)
    return GAUCHE;

  if (recherche->p->x == courant->p->x &&
      recherche->p->y > courant->p->y)
    return HAUT;

  if (recherche->p->x == courant->p->x &&
      recherche->p->y < courant->p->y)
    return BAS;

  /*si noeud recherche est en haut a droite*/
  if (recherche->p->x > courant->p->x &&
      recherche->p->y > courant->p->y)
    {
      if ((choix = aleatoire(0, 1)) == 0)
	return DROITE;
      return HAUT;
    }
    
  /*si noeud recherche est en haut a gauche*/
  if (recherche->p->x < courant->p->x &&
      recherche->p->y > courant->p->y)
    {
      if ((choix = aleatoire(0, 1)) == 0)
	return GAUCHE;
      return HAUT;
    }
    
  /*si noeud recherche est en bas a droite*/
  if (recherche->p->x > courant->p->x &&
      recherche->p->y < courant->p->y)
    {
      if ((choix = aleatoire(0, 1)) == 0)
	return DROITE;
      return BAS;
    }
    
  /*si noeud recherche est en bas a gauche*/
  if (recherche->p->x < courant->p->x &&
      recherche->p->y < courant->p->y)
    {
      if ((choix = aleatoire(0, 1)) == 0)
	return GAUCHE;
      return BAS;
    }

  return EXIT_FAILURE;
}

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * Retourne TRUE si le noeud a été trouvé.
 * Retourne FALSE sinon.
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */
int recherche (noeud *noeud, int tag)
{
  
  return FALSE;
}
