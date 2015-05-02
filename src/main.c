#include "canInsert.h"

void coordinateur (int nb_proc)
{
  int i, msg;
  MPI_Status status;
  for (i=1; i<nb_proc; i++)
    {
      envoyer(i, TAG_INSERS_TOI);
      printf("[COORD] #%d, insère toi !\n",i);
      if (recevoir(i, TAG_AYAI) != 0)
	printf("#%d erreur ayai\n",i);
      printf("[COORD] #%d est inséré.\n", i);
    }
}

void makeNode(int rang, int nb_proc)
{
  noeud *n = initNoeud(rang);
  printf("Je suis #%d\n",rang);
  recevoir(COORDINATEUR, TAG_INSERS_TOI);
  printf("%d a reçu le 'insère toi !'\n", rang);

  if (rang == BOOTSTRAP)
    {
      envoyer(COORDINATEUR, TAG_AYAI);
      printf("[BOOT]\n");
      int i;
      for (i = 2; i < nb_proc; i++)
	{
	  printf("[NODE] %d\n", i);
	  noeud *insert = recevoirNoeud(TAG_ESPACE);
	  insertion(n, insert);
	}
    }
  else
    {
      int msg;
      MPI_Status status;
      printf("MAKE #%d (%d, %d)\n",n->id,n->p->x, n->p->y);
      printf("#%d envoi TAG_ESPACE\n", rang);

      //Suis-je dans ton espace ?
      envoyerNoeud(BOOTSTRAP, TAG_ESPACE, n);

      //Je reçois mon nouvel espace
      n = recevoirNoeud(TAG_NOEUD);
      printf("N : #%d (%d, %d)\n", n->id, n->p->x, n->p->y);

      //Je reçois mes voisins
      /*recevoirVoisins(n);*/

      //Je reçois que je suis inséré
      MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, TAG_TOI_INSERE, MPI_COMM_WORLD, &status);

      //Je dis au coordinateur que je suis inséré
      envoyer(COORDINATEUR, TAG_AYAI);

      //Je peux recevoir des requêtes à tout moment...
      while(TRUE)
	{
	  MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, TAG_ESPACE, MPI_COMM_WORLD, &status);
	  noeud *insert = recevoirNoeud(TAG_ESPACE);
	  insertion(n, insert);
	}
    }
}

int main (int argc, char **argv)
{
  int nb_proc, rang;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rang);

  if (nb_proc < N+1)
    {
      printf("Nombre de processus incorrect !\n");
      MPI_Finalize();
      exit(2);
    }

  if (rang == 0)
    {
      coordinateur(nb_proc);
    }
  else
    {
      makeNode(rang, nb_proc); 	
    }

  if (rang == 0)
    printf("FIN COORDINATEUR\n");
  else if (rang == 1)
    printf("FIN BOOTSTRAP\n");
  else
    printf("FIN #%d\n",rang);

  MPI_Finalize();

  /*
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

