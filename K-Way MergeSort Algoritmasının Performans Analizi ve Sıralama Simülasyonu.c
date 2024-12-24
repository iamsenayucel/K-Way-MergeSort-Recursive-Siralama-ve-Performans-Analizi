#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_ITER 10 // Her N deðeri için 10 farklý dizi
#define K_VALUES 9  // k=2,3,...,10 için 9 farklý k deðeri

// QuickSort algoritmasý ile sýralama
void quickSort(int *arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[right];
    int i = left - 1;
    int j;
    for (j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    arr[right] = arr[i + 1];
    arr[i + 1] = pivot;
    quickSort(arr, left, i);
    quickSort(arr, i + 2, right);
}

// k-way Merge fonksiyonu
void kWayMerge(int **subArrays, int *sizes, int k, int *mergedArray, int totalSize) {
    int i, j;
    int *indices = (int *)calloc(k, sizeof(int));
    for (i = 0; i < totalSize; i++) {
        int minIdx = -1;
        for (j = 0; j < k; j++) {
            if (indices[j] < sizes[j] && (minIdx == -1 || subArrays[j][indices[j]] < subArrays[minIdx][indices[minIdx]])) {
                minIdx = j;
            }
        }
        mergedArray[i] = subArrays[minIdx][indices[minIdx]];
        indices[minIdx]++;
    }
    free(indices);
}

// Recursive MergeSort ile parçalama ve sýralama iþlemi
void recursiveSortAndMerge(int *arr, int n, int k) {
    int i;
    if (n <= k) {
        quickSort(arr, 0, n - 1);   // Eðer k'dan küçük bir boyutta dizi elde edildiyse diziyi sýralar
        return;
    }

    int partSize = n / k;
    int remaining = n % k;
    int **subArrays = (int **)malloc(k * sizeof(int *));
    int *sizes = (int *)malloc(k * sizeof(int));
    int startIdx = 0;

	// Diziyi k parçaya böl ve her alt dizi için recursiveSortAndMerge fonksiyonunu çaðýr
    for (i = 0; i < k; i++) {
        sizes[i] = (i < remaining) ? partSize + 1 : partSize;
        subArrays[i] = arr + startIdx;
        recursiveSortAndMerge(subArrays[i], sizes[i], k);
        startIdx += sizes[i];
    }

    int *mergedArray = (int *)malloc(n * sizeof(int));   // Geçici olarak birleþtirilen dizi
    kWayMerge(subArrays, sizes, k, mergedArray, n);  // Alt dizileri birleþtir
    memcpy(arr, mergedArray, n * sizeof(int));  

    free(subArrays);
    free(sizes);
    free(mergedArray);
}

// Fisher-Yates Shuffle algoritmasý ile rastgele dizi oluþturma
void generateRandomArray(int *arr, int n) {
    int i;
    for (i = 0; i < n; i++) {   // 1'den N'e kadar sýrayla ekle
        arr[i] = i + 1;
    }
    for (i = n - 1; i > 0; i--) {   // Sýralý elemanlarý karýþtýr
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


// Ana program
int main() {
    int n, iter, k, i, j;
    int N = 10;

    // N için farklý deðerlerde tekrarlar yapýlacak
    for (n = 0; n < 6; n++) {   //N için 6 farklý deðer
        N = N * 10;

        double **durations = (double **)malloc(K_VALUES * sizeof(double *));
        for (i = 0; i < K_VALUES; i++) {
            durations[i] = (double *)malloc(MAX_ITER * sizeof(double));   // Süreleri matriste tutma
        }

		// Ayný N deðeri için 10 farklý dizi oluþturma döngüsü
        for (iter = 0; iter < MAX_ITER; iter++) {
            int *originalArray = (int *)malloc(N * sizeof(int));
            generateRandomArray(originalArray, N);
            
			// Her k deðeri için sýralama iþlemini yap
            for (k = 2; k <= 10; k++) {
                int *arrayCopy = (int *)malloc(N * sizeof(int));
                memcpy(arrayCopy, originalArray, N * sizeof(int));

                clock_t start = clock();  // Baþlangýç zamaný
                recursiveSortAndMerge(arrayCopy, N, k);
                clock_t end = clock();  // Bitiþ zamaný
                durations[k - 2][iter] = (double)(end - start) / CLOCKS_PER_SEC;  // Süreyi matrise kaydediyoruz
                
                free(arrayCopy);
            }

            free(originalArray);
        }

        // Ayný N deðeri için 10 farklý diziden elde edilen süreleri yazdýrma
        printf("N = %d icin sure matrisi (saniye cinsinden):\n", N);
        for (i = 0; i < K_VALUES; i++) {
            for (j = 0; j < MAX_ITER; j++) {
                printf("%f ", durations[i][j]);
            }
            printf("\n");
        }
        printf("\n");

        // durations matrisini serbest býrakýyoruz
        for (i = 0; i < K_VALUES; i++) {
            free(durations[i]);
        }
        free(durations);
    }

    return 0;
}

