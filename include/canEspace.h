#ifndef __CANESPACE_H_
#define __CANESPACE_H_

#include <canListe.h>

int maxEspace(espace *es);
int estDansEspace(espace *es, noeud *b);
espace *decoupe(noeud *a);
void aleatoireDansEspace (espace *espace, noeud *noeud);
noeud *attributionEspace(noeud *noeud, espace *espace);
int appartient(point m, point a, point b);
int estDansSegment(noeud *source, noeud *cible, int sens);
liste_noeud *estToujoursVoisinB(noeud *noeud);
liste_noeud *estToujoursVoisinH(noeud *noeud);
liste_noeud *estToujoursVoisinD(noeud *noeud);
liste_noeud *estToujoursVoisinG(noeud *noeud);

void printEspace(noeud *noeud);
void printRect(noeud *noeud);

#endif // __CANESPACE_H
