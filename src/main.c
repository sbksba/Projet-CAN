#include "canInsert.h"

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
  
  insertion(a,b, DROITE);
  insertion(b,c, BAS);
  insertion(c,d, DROITE);
  insertion(b,e, DROITE);
     
  c->bas = estToujoursVoisinB(c);
  c->haut = estToujoursVoisinH(c);
  c->gauche = estToujoursVoisinG(c);
  c->droite = estToujoursVoisinD(c);

  b->bas = estToujoursVoisinB(b);
  b->haut = estToujoursVoisinH(b);
  b->gauche = estToujoursVoisinG(b);
  b->droite = estToujoursVoisinD(b);

  a->bas = estToujoursVoisinB(a);
  a->haut = estToujoursVoisinH(a);
  a->gauche = estToujoursVoisinG(a);
  a->droite = estToujoursVoisinD(a);
  
  /*
  maj(a);
  maj(b);
  maj(c);
  maj(d);
  maj(e);

  insertion(e,f, HAUT);
  insertion(e,g, GAUCHE);
  */
  
  printEspace(a);
  printEspace(b);
  printEspace(c);
  printEspace(d);
  printEspace(e);

  /*
  printEspace(e);
  printEspace(f);
  printEspace(g);
  */
  
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

  /*
  printf("F noeud %d de coordonnées (%d, %d)\n", f->id, f->p->x, f->p->y);
  printf("G noeud %d de coordonnées (%d, %d)\n\n", g->id, g->p->x, g->p->y);
  
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

