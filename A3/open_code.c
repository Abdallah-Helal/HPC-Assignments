#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main() {
    int i, threads_num=2, size=8192;
    double sum = 0.0, cum = 0.0, mean, var = 0.0, sd;

    printf("Enter the number of threads: ");
    scanf_s("%d", &threads_num);

    printf("Enter the size of the array: ");
    scanf_s("%d", &size);

    double* numbers = (double*)malloc(size * sizeof(double));

    for (i = 0; i < size; i++)  scanf_s("%lf", &numbers[i]);

    omp_set_num_threads(threads_num);
    double start = omp_get_wtime();


#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < size; i++) sum += numbers[i];

    mean = sum / size;

#pragma omp parallel for reduction(+:cum)
    for (i = 0; i < size; i++) cum += pow(numbers[i] - mean, 2);

    var = cum / size;
    sd = sqrt(var);

    printf("Mean: %.2f\n", mean);
    printf("Variance: %.2f\n", var);
    printf("Standard Deviation: %.2f\n", sd);

    double end = omp_get_wtime();
    printf("\nElapsed time is %lf in Seconds.", end - start);

    // Free the dynamically allocated memory
    free(numbers);

    return 0;
}