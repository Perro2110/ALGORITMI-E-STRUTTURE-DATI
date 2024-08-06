#include <stdio.h>
#include <stdlib.h>

// Strutture Grafi

typedef struct {
  int a, b;
  long peso;
} tris;

// Struttura insiemi disgiunti

typedef struct dis {
  int parent;
  int rank;
} dis;

// Insiemi disgiunti

void MakeSet(dis *nodes, int x) {
  nodes[x].parent = x;
  nodes[x].rank = 0;
}

int FindSet(dis *nodes, int x) {
  if (x != nodes[x].parent) {
    nodes[x].parent =
        FindSet(nodes, nodes[x].parent); // Compressione del percorso
  }
  return nodes[x].parent;
}

void Union(dis *nodes, int x, int y) {
  int xRoot = FindSet(nodes, x);
  int yRoot = FindSet(nodes, y);
  if (nodes[xRoot].rank > nodes[yRoot].rank) {
    nodes[yRoot].parent = xRoot;
  }
  if (nodes[xRoot].rank <= nodes[yRoot].rank) {
    nodes[xRoot].parent = yRoot;
    if (nodes[xRoot].rank == nodes[yRoot].rank)
      nodes[yRoot].rank++;
  }
}

// SORT per divertirmi

void swap(tris *a, tris *b) {
  tris temp = *a;
  *a = *b;
  *b = temp;
}

int Partition(tris *vet, int p, int r) {
  long x = vet[r].peso; // il pivot
  int i = p - 1;        // L'indice più alto nella sezione bassa
  int j;
  for (j = p; j <= r - 1; j++) { // Tratta ogni elemento che non è il pivot
    if (vet[j].peso <=
        x) {     // Questo elemento appartiene alla sezione bassa ? chiedere
      i = i + 1; // Indice di nuova posizione nella sezione bassa
      swap(&(vet[i]),
           &(vet[j])); // Inserisce questo elemento nella nuova posizione
    }
  }
  swap(&(vet[i + 1]), &(vet[r])); // Il pivot si trova nella posizione destra
                                  // della sezione bassa
  return (i + 1);                 // nuovo indice del pivot
}

void QuickSort(tris *vet, int p, int r) {
  int q;
  if (p < r) {
    // Partiziono il sottoArray intorno al pivot, che finisce in A[q]
    q = Partition(vet, p, r);
    QuickSort(vet, p, q - 1); // "Ordina ricorsivamente la sezione bassa"
    QuickSort(vet, q + 1, r); // "Ordina ricorsivamente la sezione alta"
  }
}

int main() {
  /* INIZIALIZZAZIONE */
  FILE *fp1, *fp2;
  int N, M;
  int i = 0;
  long sum = 0;
  int a, b;
  long w; // u v peso m

  fp1 = fopen("input.txt", "r");
  fscanf(fp1, "%d%d", &N, &M);

  tris Ge[M];

  for (i = 0; i < M;
       i++) { // aggiungo archi e genero lista da ordinare poi di G.e
    fscanf(fp1, "%d%d%ld", &a, &b, &w);
    Ge[i].a = a;
    Ge[i].b = b;
    Ge[i].peso = w;
  }
  fclose(fp1);

  dis d[N]; // creo gli N make set di Vertici

  for (int i = 0; i < N; i++) {
    d[i].parent = i;
    d[i].rank = 0;
  }
  QuickSort(Ge, 0, M - 1);

  /*
            //Per debug
  for (int i = 0; i < M; i++) {
    printf("%d %d %ld\n", Ge[i].a, Ge[i].b, Ge[i].peso);
  }*/

  for (int i = 0; i < M; i++) {
    if (FindSet(d, Ge[i].a) != FindSet(d, Ge[i].b)) {
      Union(d, Ge[i].a, Ge[i].b);
      sum += Ge[i].peso;
    }
  }
  // for (int i = 0; i < M; i++) {
  //   printf("%d %d %ld\n", Ge[i].a, Ge[i].b, Ge[i].peso);
  //
  // }
  printf("%ld", sum);
  fp2 = fopen("output.txt", "w");
  fprintf(fp2, "%ld", sum);
  fclose(fp2);
}