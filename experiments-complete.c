#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Vedi `struct configuration`.
 */
#define MAX_ALGO 16
#define MAX_ALGO_BUFF 64

/**

 * Soglia di ibridazione per `hybrid merge sort`.
 */
#define HYBRID_MS_K 90

/**
 * Soglia di ibridazione per `hybrid quick sort`.
 */
#define HYBRID_QS_K 55

/**
 * Parametri di un esperimento.
 *
 * min_size: dimensione dell'array nel primo esperimento.
 * max_size: dimensione dell'array nell'ultimo esperimento.
 * step: incremento di dimensione dell'array tra un esperimento e il successivo.
 * repetitions: numero di volte in cui un esperimento è ripetuto,
 *              al fine di ottenere tempi statisticamente validi.
 * seed: seme del generatore di numeri pseudocasuali,
 *              al fine di garantire la riproducibilità degli esperimenti.
 *
 * Contiene anche un array di stringhe, che rappresentano
 * gli algoritmi di ordinamento protagonisti dell'esperimento.
 */
struct configuration {
  int min_size;
  int max_size;
  int step;
  int repetitions;
  int seed;

  int n_algorithms;
  char algorithms[MAX_ALGO][MAX_ALGO_BUFF];
};

/**
 * Inizializza i parametri di un esperimento.
 * Vedi anche: `struct configuration`.
 */
void init(struct configuration *config) {

  printf("Inserire min_size (ad esempio, 10): ");
  scanf("%d", &config->min_size);

  printf("Inserire max_size (ad esempio, 1000): ");
  scanf("%d", &config->max_size);

  printf("Inserire step (ad esempio, 10): ");
  scanf("%d", &config->step);

  printf("Inserire repetitions (ad esempio, 50): ");
  scanf("%d", &config->repetitions);

  printf("Inserire seed (ad esempio, 362372): ");
  scanf("%d", &config->seed);

  int n_algorithms = 8;
  char *algos[] = {"INSERTION", "MERGE",     "HYBRIDMERGE", "QUICK",
                   "MOTQUICK",  "TAILQUICK", "HEAPSORT",    "INTROSORT"};

  config->n_algorithms = n_algorithms;
  for (int algo_idx = 0; algo_idx < n_algorithms; algo_idx++) {
    strcpy(config->algorithms[algo_idx], algos[algo_idx]);
  }
}

/**
 * Funzione antagonista: controlla che un array sia ordinato.
 */
int check(int *arr, int size) {
  for (int i = 1; i < size; i++) {
    if (arr[i] < arr[i - 1]) {
      return -1;
    }
  }
  return 1;
}

/**
 * Implementazione di `insertion sort`.
 */
void insertion_sort(int *arr, int start, int end) {
  int key, i;

  for (int j = start; j < end + 1; j++) {
    key = arr[j];
    i = j - 1;

    while (i >= start && arr[i] > key) {
      arr[i + 1] = arr[i];
      i--;
    }
    arr[i + 1] = key;
  }
}

/**
 * Sottoprocedura di `merge_sort` per unire due range.
 */
void merge(int *arr, int start, int mid, int end) {
  int n1 = mid - start + 1;
  int n2 = end - mid;

  int i, j, left[n1], right[n2];

  for (i = 0; i < n1; i++) {
    left[i] = arr[start + i];
  }

  for (j = 0; j < n2; j++) {
    right[j] = arr[mid + 1 + j];
  }

  i = j = 0;

  for (int k = start; k <= end; k++) {
    if (i < n1) {
      if (j < n2) {
        arr[k] = (left[i] <= right[j]) ? left[i++] : right[j++];
      } else {
        arr[k] = left[i++];
      }
    } else {
      arr[k] = right[j++];
    }
  }
}

/**
 * Implementazione di `merge sort`.
 */
void merge_sort(int *arr, int start, int end) {
  if (start < end) {
    int mid = start + (end - start) / 2;
    merge_sort(arr, start, mid);
    merge_sort(arr, mid + 1, end);
    merge(arr, start, mid, end);
  }
}

/**
 * Implementazione di `merge sort` ibridato con `insertion sort`. logica di chi
 * performa meglio anche dipendentemente dalla chahe oltre che al valore della
 * "c"
 */
void hybrid_merge_sort(int *arr, int start, int end) {
  if (end - start > HYBRID_MS_K) {
    int mid = start + (end - start) / 2;
    hybrid_merge_sort(arr, start, mid);
    hybrid_merge_sort(arr, mid + 1, end);
    merge(arr, start, mid, end);
  } else {
    insertion_sort(arr, start, end);
  }
}

void swap(int *arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

/**
 * Sottoprocedura principale di `quick_sort`.
 */
int partition(int *arr, int start, int end) {
  int pivot = arr[end];
  int i = start - 1;

  for (int j = start; j < end; j++) {
    if (arr[j] <= pivot) {
      swap(arr, ++i, j);
    }
  }

  swap(arr, ++i, end);
  return i;
}

/**
 * Implementazione di `quick sort`.
 */
void quick_sort(int *arr, int start, int end) {
  if (start < end) {
    int mid = partition(arr, start, end);
    quick_sort(arr, start, mid - 1);
    quick_sort(arr, mid + 1, end);
  }
}

int median_of_three(int *arr, int a, int b, int c) {
  if (arr[a] > arr[b]) {
    if (arr[b] > arr[c])
      return b;
    else if (arr[a] < arr[c])
      return a;
    else
      return c;
  } else {
    if (arr[a] > arr[c])
      return a;
    else if (arr[b] < arr[c])
      return b;
    else
      return c;
  }
}

/**
 * Come `partition` ma il pivot viene scelto con mediana di tre valori.
 */
int mot_partition(int *arr, int start, int end) {
  int median = median_of_three(arr, start, end, start + (end - start) / 2);


  int pivot = arr[median];
  int i = start - 1;

  for (int j = start; j < end; j++) {
    if (arr[j] <= pivot) {
      swap(arr, ++i, j);
    }
  }
  swap(arr, ++i, end);
  return i;
  
}

/**
 * Implementazione di `quick sort` con mediana di tre.
 */
void mot_quick_sort(int *arr, int start, int end) {
  if (start < end) {
    int mid = mot_partition(arr, start, end);
    mot_quick_sort(arr, start, mid - 1);
    mot_quick_sort(arr, mid + 1, end);
  }
}

/*
  QUICKSORT NON è IN PLACE, CHIAMATE RICORSIVE FANNO IN MODO CHE NON SIA COSI
  !!! PUO' ESSERLO DEFINENDOLO IN CODA CON UNA TAIL RECURSION
*/
/**
 * Implementazione di `quick sort` con ricorsione in coda.
 * Al più O(N) chiamate ricorsive in contemporanea sullo stack.
 */
void naive_tail_quick_sort(int *arr, int start, int end) {
  while (start < end) {
    int mid = mot_partition(arr, start, end);
    naive_tail_quick_sort(arr, start, mid - 1);
    start = mid + 1;
  }
}

/**
 * Implementazione di `quick sort` con ricorsione in coda furba.
 * Al più O(logN) chiamate ricorsive in contemporanea sullo stack.

                            QUIKERSORT
Alla scelta del pivot con mediana di tre e alla ricorsione in coda, ibridiamo
QuickSort con InsertionSort per ordinare gli array sotto una certa soglia di
lunghezza. Stabiliamo la soglia sperimentalmente, proprio come avevamo
fatto con HybridMergeSort.
Abbiamo così ottenuto l'implementazione uciale del QuickSort in C,
chiamata in gergo QuickerSort (implementazione uciale disponibile qui) e
invocabile con il comando qsort.

 */
void tail_quick_sort(int *arr, int start, int end) {
  if (end - start <= HYBRID_QS_K) {
    insertion_sort(arr, start, end);
    return;
  }

  while (start < end) {
    int mid = mot_partition(arr, start, end);

    if (mid - start > end - mid) {
      tail_quick_sort(arr, start, mid - 1);
      start = mid + 1;
    } else {
      tail_quick_sort(arr, mid + 1, end);
      end = mid - 1;
    }
  }
}

void max_heapify(int *arr, int n, int root) {
  int largest = root;
  int left = 2 * root + 1;
  int right = 2 * root + 2;

  if (left < n && arr[left] > arr[largest]) {
    largest = left;
  }

  if (right < n && arr[right] > arr[largest]) {
    largest = right;
  }

  if (largest != root) {
    swap(arr, root, largest);
    max_heapify(arr, n, largest);
  }
}

/**
 * Implementazione di `heap sort`.
 * Vedi anche `intro sort`.
 */
void heap_sort(int *arr, int start, int end) {
  int n = end - start + 1;

  // Build the heap
  for (int i = n / 2 - 1; i >= 0; i--) {
    max_heapify(arr, n, i);
  }

  for (int i = n - 1; i > 0; i--) {
    swap(arr, start, start + i);
    max_heapify(arr, i, start);
  }
}

/**
 * Vedi `intro sort`.
 * Questa è la sua sottoroutine principale.
 * DI BASE QUICKSORT QUANDO RICORRE TROPPO (2LOG(n)) PASSA A HEAPSORT
 * NON DEGENERA MAI TUTTI GLI ALTRI SI !!!
 **/

void _intro_sort(int *arr, int start, int end, int depth_limit) {
  if (end - start <= HYBRID_QS_K) {
    insertion_sort(arr, start, end);
  } else if (depth_limit == 0) {
    heap_sort(arr, start, end);
  } else {
    while (start < end) {
      int mid = mot_partition(arr, start, end);
      // TAIL INTELIGENTE
      if (mid - start > end - mid) {
        _intro_sort(arr, start, mid - 1, depth_limit - 1);
        start = mid + 1;
      } else {
        _intro_sort(arr, mid + 1, end, depth_limit - 1);
        end = mid - 1;
      }
    }
  }
}

/**
 * Implementazione di `intro sort`.
 * Qui, si calcola la soglia di chiamate ricorsive del `quick sort`
 * che verrà chiamato internamente.
 * Vedi `_intro_sort(int *, int, int, int)`.
 */
void intro_sort(int *arr, int start, int end) {
  return _intro_sort(arr, start, end, 2 * log2(end - start + 1));
}

/**
 * Interfaccia per una generica funzione di ordinamento,
 * che ha come argomenti (in ordine):
 * 1) il riferimento al primo elemento di un array
 * 2) l'inizio del range da ordinare nell'array
 * 3) la fine del range da ordinare nell'array
 */
typedef void (*algo_ptr)(int *, int, int);

/**
 * Seleziona un algoritmo di ordinamento scegliendolo
 * in base alla stringa passata come argomento.
 * Vedi `algo_ptr`.
 */
algo_ptr select_algorithm(char *algo_name) {
  if (strcmp(algo_name, "INSERTION") == 0) {
    return &insertion_sort;
  } else if (strcmp(algo_name, "MERGE") == 0) {
    return &merge_sort;
  } else if (strcmp(algo_name, "HYBRIDMERGE") == 0) {
    return &hybrid_merge_sort;
  } else if (strcmp(algo_name, "QUICK") == 0) {
    return &quick_sort;
  } else if (strcmp(algo_name, "MOTQUICK") == 0) {
    return &mot_quick_sort;
  } else if (strcmp(algo_name, "TAILQUICK") == 0) {
    return &tail_quick_sort;
  } else if (strcmp(algo_name, "HEAPSORT") == 0) {
    return &heap_sort;
  } else if (strcmp(algo_name, "INTROSORT") == 0) {
    return &intro_sort;
  } else {
    printf("Errore - l'algoritmo selezionato non è disponibile.");
    exit(-1);
  }
}

/**
 * Esegue un numero di misurazioni di tempi pari a `repetitions`.
 * Ogni misurazione riguarda l'applicazione di un certo algoritmo
 * di ordinamento ad un array di dimensione `size`, inizializzato
 * con numeri pseudocasuali.
 * Restituisce un valore `double`, contenente il tempo medio di esecuzione.
 */

double run(int size, int repetitions, algo_ptr algorithm) {
  double elapsed_time = 0.0;

  for (int i = 0; i < repetitions; i++) {
    int arr[size];
    for (int j = 0; j < size; j++) {
      arr[j] = rand();
    }

    clock_t start, end;
    start = clock(); // cpu_time non il vero tempo nel mondo esterno (whole
                     // clock time)
    algorithm(arr, 0, size - 1);
    end = clock();

    if (check(arr, size) != 1) {
      printf("ERRORE: L'ordinamento è scorretto.\n");
      exit(-1);
    }
    elapsed_time += (end - start) / (double)CLOCKS_PER_SEC;
  }

  return elapsed_time / repetitions;
}

/**
 * Manager degli esperimenti.
 *
 * Chiama un esperimento più volte, gestendo di volta in volta
 * l'incremento della dimensione dell'array soggetto degli esperimenti.
 *
 * Si occupa anche di inizializzare e variare il generatore di numeri
 * pseudocasuali, e di stampare a video i risultati.
 */
void run_experiments(struct configuration config, FILE *report) {
  // Stampa quello che, nel grafico finale, sarà l'asse delle x.
  for (int i = config.min_size; i <= config.max_size; i += config.step) {
    fprintf(report, "%d ", i);
  }
  fprintf(report, "\n");

  // Ogni esperimento partirà da questo seed per generare numeri pseudocasuali.
  int initial_seed = config.seed;

  // Per ogni algoritmo caricato nella configurazione...
  for (int algo_idx = 0; algo_idx < config.n_algorithms; algo_idx++) {

    // Per evitare favoritismi o sfavoritismi, voglio generare
    // sempre gli stessi numeri per tutti gli algoritmi.
    // Questo seme cambierà, dimensione dopo dimensione,
    // per inizializzare nuovi generatori sempre allo stesso modo
    // per ogni algoritmo.
    int mutable_seed = initial_seed;

    // Esegui un esperimento, e stampa passo dopo passo l'asse delle y.
    for (int i = config.min_size; i <= config.max_size; i += config.step) {
      srand(mutable_seed);
      double elapsed_time = run(i, config.repetitions,
                                select_algorithm(config.algorithms[algo_idx]));
      mutable_seed++;

      fprintf(report, "%g ", elapsed_time);
    }

    fprintf(report, "\n");
  }
}

/**
 * Entry point per gli esperimenti.
 *
 * Dopo aver chiesto all'utente di inserire una configurazione,
 * esegue gli esperimenti.
 */
int main(void) {
  struct configuration config;
  init(&config);

  FILE *report = fopen("report.txt", "w");
  if (report == NULL) {
    printf("Errore apertura file di report.");
    return -1;
  }

  run_experiments(config, report);

  if (fclose(report) != 0) {
    printf("Errore chiusura file di report.");
  }

  return 0;
}
