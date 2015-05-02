#ifndef __CANINSERT_H_
#define __CANINSERT_H_

#include <canEspace.h> 

/* PRIMITIVES MPI */

int sizeList(liste_noeud *liste);

int *convertirNoeud(noeud *noeud);
noeud *convertirTab(int *tab);

int envoyer(int id_destinataire, int tag);
int recevoir(int emetteur, int tag);

int envoyerNoeud(int id_destinataire, int tag, noeud * noeud);
noeud *recevoirNoeud(int tag);
int envoyerVoisins(int id_destinataire, noeud *emetteur);
int recevoirVoisins(noeud *destinataire);

/* INSERTION */

void majVoisins(noeud *noeudA, noeud *noeudB, espace *origine);
void insertD(noeud *cible, noeud *insert);
void insertG(noeud *cible, noeud *insert);
void insertH(noeud *cible, noeud *insert);
void insertB(noeud *cible, noeud *insert);
void insertion(noeud *cible, noeud *insert);

/* RECHERCHE */
int direction(noeud *courant, noeud *recherche);
int recherche (noeud *noeud, int tag);

#endif // __CANINSERT_H_
