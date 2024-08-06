#include <stdio.h>
#include <stdlib.h>

typedef struct duple {
  int id;
  int num;
} t_dubple;

void swap(t_dubple *a, t_dubple *b) {
  t_dubple temp = *a;
  *a = *b;
  *b = temp;
}

int Partition(t_dubple *vet, int p, int r) {
  int x = vet[r].num; // il pivot
  int i = p - 1;      // L'indice più alto nella sezione bassa
  int j;
  for (j = p; j <= r - 1; j++) { // Tratta ogni elemento che non è il pivot
    if (vet[j].num <=
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

t_dubple *QuickSort(t_dubple *vet, int p, int r) {
  int q;
  if (p < r) {
    // Partiziono il sottoArray intorno al pivot, che finisce in A[q]
    q = Partition(vet, p, r);
    QuickSort(vet, p, q - 1); // "Ordina ricorsivamente la sezione bassa"
    QuickSort(vet, q + 1, r); // "Ordina ricorsivamente la sezione alta"
  }
}

int RecursiveBinarySearch(t_dubple *dup, int low, int high, int da_cercare,
                          FILE *fp2) {
  if (low > high) {
    fprintf(fp2, "NULL ");
    return -1;
  }
  int mid = (high + low) / 2;
  // printf("mid: %d \n",mid);
  if (dup[mid].num == da_cercare) {
    fprintf(fp2, "%d ", dup[mid].id);
    return 1;
  }
  if (dup[mid].num < da_cercare) {
    return RecursiveBinarySearch(dup, mid + 1, high, da_cercare, fp2);
  }
  if (dup[mid].num > da_cercare) {
    return RecursiveBinarySearch(dup, low, mid - 1, da_cercare, fp2);
  }
}

int main(int argc, char const *argv[]) {
  /*********************
     INIZIALIZZAZIONE
  *********************/

  FILE *fp1, *fp2;
  int N; // N = numero dei file presenti nell'archivio
  int Q; // Q = numero richieste a cui rispondere
  int usati = 0, rimasto = 0;
  int da_cercare;
  int i, j;
  int id_tmp;
  int s_tmp;
  t_dubple *dup;
  // t_dubple dup [1000000]; ALLOCAZIONE ATERNATIVA

  /*********************
      BUSINESS LOGIC
  *********************/

  // dup = (t_dubple *) malloc(sizeof(t_dubple) * 1000000); ALLOCAZIONE
  // ATERNATIVA

  // LEGGO N & Q
  fp1 = fopen("input.txt", "r");
  fscanf(fp1, "%d%d", &N, &Q);

  // LEGGO LE N RIGHE
  for (i = 0; i < N; i++) {
    fscanf(fp1, "%d%d", &id_tmp, &s_tmp);
    usati += s_tmp - 1;

    if (i == 0) {
      dup = (t_dubple *)malloc(sizeof(t_dubple) * (s_tmp - 1));
    } else {
      dup = (t_dubple *)realloc(dup, sizeof(t_dubple) * usati);
    }

    for (j = 0; j < (s_tmp - 1); j++, rimasto++) {
      dup[rimasto].id = id_tmp;
      fscanf(fp1, "%d", &dup[rimasto].num);
    }
  }
  // RIMASTO = LUNGHEZZA VETTORE

  /*************
   * DEBUG
   *
   printf("\n");
   for (i=0;i < rimasto;i++) {
       //printf("| id: %ld num: %ld |",dup[i].id,dup[i].num);
       printf("%d ",dup[i].num);
   }
   printf("\n");
   **************/

  /*********************
      ORDINAMENTO
  *********************/
  /* Insertion sort
  for (j = 1;j < rimasto;j++){
       t_dubple key = dup[j];
       i = j - 1;
       while ((i >= 0) && (dup[i].num > key.num)){
           dup[i + 1] = dup [i];
           i = i - 1;
       }
       dup[i + 1] = key;
  }
   */
  /*Quick sort*/
  // printf("%d",rimasto); per debug
  QuickSort(dup, 0, rimasto - 1);

  /*************
   * DEBUG
   *
   printf("\n");
   for (i=0;i < rimasto;i++) {
       //printf("| id: %ld num: %ld |",dup[i].id,dup[i].num);
       printf("%d ",dup[i].num);
   }
   printf("\n");
   **************/

  /*********************
      RICERCA
  *********************/

  fp2 = fopen("output.txt", "w");
  for (i = 0; i < Q; i++) {
    fscanf(fp1, "%d", &da_cercare);
    // printf(" \n %ld \n",da_cercare);
    j = RecursiveBinarySearch(dup, 0, rimasto - 1, da_cercare,
                              fp2); // j giusto per debug
  }
  return 0;
}
