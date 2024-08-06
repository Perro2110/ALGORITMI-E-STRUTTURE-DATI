#include <stdio.h>
#include <stdlib.h>

int f(int *vet, int dim, int pos) // indietro
{
  int i;
  int j = 0;
  for (i = dim; i >= 0; i--) {
    j++;
    if (vet[i] < pos) {
      j--;
      // printf("quelli prima: || %d || ",j);
      return j;
    }
  }
  // printf("quelli prima: || %d || ",j);
  return j;
}

int ff(int *vet, int dim, int fin, int pos) // avanti
{
  int i;
  int j = 0;
  for (i = dim; i < fin; i++) {
    j++;
    if (vet[i] < pos) {
      j--;
      // printf("quelli dopo: || %d || ",j);
      return j;
    }
  }
  // printf("quelli dopo: || %d || ",j);
  return j;
}

int main() {
  int *vet;
  // int vet[10000];
  int dim;
  int a = 0;
  int amax = 0;
  int j = 0;
  FILE *fp;

  fp = fopen("input.txt", "r");
  fscanf(fp, "%d", &dim);

  vet = (int *)malloc(sizeof(int) * dim);

  for (int i = 0; i < dim; i++) {
    fscanf(fp, "%d", &vet[i]);
  }
  fclose(fp);

  int i;
  for (i = 0; i < dim - 1; i++) {
    j = i + 1;

    if (vet[j] == 0) {
      a = vet[i];
    } else if (vet[i] == 0) {
      a = 0;
    } else {
      if (vet[i] > amax) {
        amax = vet[i];
      }

      if (vet[i] > vet[j])
        a = vet[j] * (f(vet, j, vet[j])) +
            vet[j] * (ff(vet, i, dim, vet[j]) - 2);
      else
        a = vet[i] * (f(vet, j, vet[i])) +
            vet[i] * (ff(vet, i, dim, vet[i]) - 2);
    }
    if (amax <= a)
      amax = a;
    // printf("i= %d a = %d a_max = %d \n",i,a,amax);
  }

  if (vet[i] > amax) {
    amax = vet[i];
  }
  a = vet[i] * (f(vet, i, vet[i]));
  // printf("\n\nil a max = %d \n",amax);
  if (amax <= a)
    amax = a;
  // printf("i= %d a = %d a_max = %d \n",i,a,amax);
  fp = fopen("output.txt", "w");
  fprintf(fp, "%d\n", amax);

  return 0;
}