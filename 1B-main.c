#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int Partition(int *vet, int p, int r) {
  int x = vet[r]; // il pivot
  int i = p - 1;  // L'indice più alto nella sezione bassa
  int j;
  for (j = p; j <= r - 1; j++) { // Tratta ogni elemento che non è il pivot
    if (vet[j] <=
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

int *QuickSort(int *vet, int p, int r) {
  int q;
  if (p < r) {
    // Partiziono il sottoArray intorno al pivot, che finisce in A[q]
    q = Partition(vet, p, r);
    QuickSort(vet, p, q - 1); // "Ordina ricorsivamente la sezione bassa"
    QuickSort(vet, q + 1, r); // "Ordina ricorsivamente la sezione alta"
  }
}

int RecursiveBinarySearch(int *vet, int low, int high, int da_cercare) {
  if (low > high) {
    return low;
  } else {
    int mid = (high + low) / 2;
    if (vet[mid] <= da_cercare) {
      return RecursiveBinarySearch(
          vet, mid + 1, high,
          da_cercare); // Se vet[mid] è minore o uguale di da_cercare, continua
                       // la ricerca a destra
    } else if (vet[mid] > da_cercare) {
      return RecursiveBinarySearch(
          vet, low, mid - 1,
          da_cercare); // Se vet[mid] è maggiore di da_cercare, continua la
                       // ricerca a sinistra
    }
  }
}

int main(int argc, char const *argv[]) {
  /*********************
     INIZIALIZZAZIONE
  *********************/

  int N, Q;            // righe_numero di pianeti e richieste
  long int x, y, z;    // coords || le moltiplicazioni andrebbero in overflow ?
  int dist_da_cercare; // distanza
  // int x2,y2,z2;     // superfluo ma per lettura
  FILE *fin, *fout;    // file in e out
  int i;               // utile per cicli
  // int *vet_dist;    // vettore di distanze
 
  /*********************
      BUSINESS LOGIC
  *********************/

  // LEGGO N & Q
  fin = fopen("input.txt", "r");
  fscanf(fin, "%d%d", &N, &Q);
  // vet_dist = (int *) malloc(sizeof(int) * (N));
  int vet_dist[N];
  // LEGGO LE N RIGHE
  for (i = 0; i < N; i++) {
    fscanf(fin, "%ld%ld%ld", &x, &y, &z);
    /* potenze
    x2 = x * x;
    y2 = y * y;
    z2 = z * z;
    */
    /*
    printf("\nx^2 = %d\n",x2);
    printf("y^2 = %d\n",y2);
    printf("z^2 = %d\n",z2);
    */
    // calcolo distanza e inserimento in vettore
    vet_dist[i] =
        ceil(sqrt((x * x) + (y * y) +
                  (z * z))); // arrotonda verso l'alto, potrebbe subire da
                             // errore macchina printf("%d",vet_dist[i]);
  }
  /*
      N = lunghezza vett
  */

  /*************
   * DEBUG
   **
   printf("\n");
   for (i = 0;i < N;i++) {
       printf("%d ",vet_dist[i]);
   }
   printf("\n");
   **************/

  /*********************
      ORDINAMENTO
  *********************/

  QuickSort(vet_dist, 0, N - 1);

  /*************
  * DEBUG
  **
  printf("\n");
  for (i = 0;i < N;i++) {
      printf("%d ",vet_dist[i]);
  }
  printf("\n");
  **************/

  /*********************
      RICERCA
  *********************/

  fout = fopen("output.txt", "w");
  for (i = 0; i < Q; i++) {
    fscanf(fin, "%d", &dist_da_cercare);
    fprintf(fout, "%d \n",
            RecursiveBinarySearch(vet_dist, 0, N - 1, dist_da_cercare));
  }

  return 0;
}
