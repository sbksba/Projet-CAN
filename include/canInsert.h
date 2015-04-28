#ifndef __CANINSERT_H_
#define __CANINSERT_H_

#include <canEspace.h> 

/* INSERTION */

void insertD(noeud *cible, noeud *insert);
void insertG(noeud *cible, noeud *insert);
void insertH(noeud *cible, noeud *insert);
void insertB(noeud *cible, noeud *insert);
void insertion(noeud *cible, noeud *insert, int dir);
int direction(noeud *courant, noeud *recherche);

void majVoisins(noeud *noeudA, noeud *noeudB, espace *origine);

#endif // __CANINSERT_H_
