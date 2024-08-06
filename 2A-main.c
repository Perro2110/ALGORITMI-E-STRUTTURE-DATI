/*
    Dimostrare se grafo bipartito
    mostrando che cicli (se vi sono) sono dispari (# archi)
*/

#include <stdio.h>
#include <stdlib.h>

int snap = 0; // globale

// Struttura per rappresentare un nodo della lista di adiacenza
typedef struct node {
  int data;
  struct node *next;
} Node;

typedef struct {
  int colore; // 1 = bianco | 2 = grigio | 3 = nero
  int tempo_scoperta;
  struct node *next;
} Lista;

int DepthVisitCycle(Lista adjList[], Node *u, int pos, int cycle) {

  adjList[pos].colore = 2;             // ora sono grigio
  adjList[pos].tempo_scoperta = cycle; // del precedente
  cycle++;

  for (Node *t = u; t != NULL; t = t->next) {
    if (adjList[t->data].colore == 1) { // se è bianco
      DepthVisitCycle(adjList, adjList[t->data].next, t->data, cycle);
    } else if (adjList[t->data].colore == 2) { // se è grigio
      snap = cycle - adjList[t->data].tempo_scoperta;
      if (snap % 2 != 0)
        return snap;
    }
  }
  adjList[pos].colore = 3; // ora nero
  return snap;
}

int CycleDet(int N, Lista adjList[N]) {
  int i;
  int *vet;
  int cycle;
  vet = (int *)calloc(N, sizeof(int));

  for (i = 0; i < N; i++) {
    adjList[i].colore = 1;
  }

  for (i = 0; i < N; i++) {
    if (adjList[i].colore == 1) {
      cycle = 0;
      snap = 0;
      vet[i] = DepthVisitCycle(adjList, adjList[i].next, i, cycle);
    }
  }
  for (i = 0; i < N; i++)
    if (vet[i] != 0 && vet[i] % 2 != 0) {
      return 1;
    }
  return 0;
}

// Funzione per creare un nuovo nodo
Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

// Funzione per aggiungere un arco alla lista di adiacenza
void creaArco(Lista adjList[], int src, int dest) {
  // Aggiungere dest alla lista di adiacenza di src e viceversa
  
  Node *newNode = createNode(dest);
  newNode->next = adjList[src].next;
  adjList[src].next = newNode;

  newNode = createNode(src);
  newNode->next = adjList[dest].next;
  adjList[dest].next = newNode;
}

// Funzione per stampare la lista di adiacenza
void print(Node *adjList[], int vertices) {
  printf("Lista di adiacenza del grafo:\n");
  for (int i = 0; i < vertices; i++) {
    Node *temp = adjList[i];
    printf("Vertice %d:", i);
    while (temp != NULL) {
      printf(" -> %d", temp->data);
      temp = temp->next;
    }
    printf("\n");
  }
}

int main() {
  /***************************
     INIZIALIZZAZIONE
  ***************************/

  FILE *fp1, *fp2;
  int N; // N = numero totale di pugili
  int M; // M = numero di rivalita tra pugili
  int i;
  int mario, luigi; // appoggio per 'mario vs luigi'
  fp1 = fopen("input.txt", "r");
  fscanf(fp1, "%d%d", &N, &M);

  Lista adjList[N]; // la lista avra quindi N pugili

  for (i = 0; i < N; i++) { // aggiungo archi
    adjList[i].colore = 1;
    adjList[i].next = NULL;
  }

  for (i = 0; i < M; i++) { // aggiungo archi
    fscanf(fp1, "%d%d", &mario, &luigi);
    creaArco(adjList, mario, luigi);
  }

  // Stampa la lista di adiacenza * debug
  // print(adjList, N);
  // printf("\n _________________ \n");

  /**************************
     BusinessLogiq and Out
  ***************************/
  fp2 = fopen("output.txt", "w");
  if (CycleDet(N, adjList) == 1) {
    fprintf(fp2, "FALSE");
  } else {
    fprintf(fp2, "TRUE");
  }
  return 0;
}
