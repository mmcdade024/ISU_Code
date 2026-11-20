#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double calculate_average(int *arr, int size) {
    if (size == 0) return 0;
    long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return (double)sum / size;
}

int main() {
    clock_t start, end;
    int sizes[] = {10, 1000, 1000000};

    for (int test = 0; test < 3; test++) {
        int N = sizes[test];
        int *arr = malloc(N * sizeof(int));
        if (!arr) {
            printf("Memory allocation failed\n");
            return 1;
        }

        for (int i = 0; i < N; i++) {
            arr[i] = rand() % 100;
        }

        start = clock();
        double avg = calculate_average(arr, N);
        end = clock();

        printf("Array Size: %d, Average: %.2f, Execution Time: %.5f seconds\n",
               N, avg, (double)(end - start) / CLOCKS_PER_SEC);

        free(arr);
    }
    
    return 0;
}
