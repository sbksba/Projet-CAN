#ifndef __CANLISTE_H_
#define __CANLISTE_H_

#include <can.h>

/* LISTE */
liste_noeud *nouvelleListe(noeud *noeud, liste_noeud *l);
liste_noeud *ajouterNoeud(liste_noeud *liste, noeud *n);
liste_noeud *supprimerNoeud(liste_noeud *liste, noeud *n);
liste_noeud *copieListe(liste_noeud *liste);
void supprimerListe(liste_noeud *liste);

void printListeNoeud(liste_noeud *liste);
void printListe(noeud *noeud);

#endif // __CANLISTE_H_
