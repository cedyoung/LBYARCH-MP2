#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

extern void daxpy_asm(long long n, double A, double* X, double* Y, double* Z);

void run_c_kernel(long long length, double constant, double* A, double* B, double* Dest) {
    long long idx;
    for (idx = 0; idx < length; idx++) {
        Dest[idx] = constant * A[idx] + B[idx];
    }
}

int main() {
    // our test sizes: 2^20, 2^24, and 2^29
    // Using explicit bit shifts
    long long vector_sizes[] = { 1LL << 20, 1LL << 24, 1LL << 29 };
    int total_tests = 3;
    
    // Constant 'A' for the equation
    double scalar_val = 2.0;

    // Variables for timing
    LARGE_INTEGER freq, t_start, t_end;
    double elapsed_time;

    printf("=== MP2 Project: x86-64 Assembly vs C Interface ===\n");
    printf("Comparisons of DAXPY kernel performance.\n\n");

    for (int t = 0; t < total_tests; t++) {
        long long count = vector_sizes[t];
        printf("----------------------------------------\n");
        printf("Processing Vector Length: %lld elements\n", count);

        // Using 'calloc' for result vectors to ensure clean memory start, 'malloc' for others
        double* vec_X = (double*)malloc(count * sizeof(double));
        double* vec_Y = (double*)malloc(count * sizeof(double));
        double* res_ASM = (double*)malloc(count * sizeof(double));
        double* res_C = (double*)malloc(count * sizeof(double));

        if (vec_X == NULL || vec_Y == NULL || res_ASM == NULL || res_C == NULL) {
            printf("[Error] Not enough memory to allocate vectors.\n");
            if(vec_X) free(vec_X);
            if(vec_Y) free(vec_Y);
            if(res_ASM) free(res_ASM);
            if(res_C) free(res_C);
            break;
        }

        for (long long k = 0; k < count; k++) {
            vec_X[k] = (double)rand() / RAND_MAX;
            vec_Y[k] = (double)rand() / RAND_MAX;
        }

        // --- VALIDATION PHASE ---
        printf(">> Verifying logic correctness...\n");
        
        run_c_kernel(count, scalar_val, vec_X, vec_Y, res_C);
        daxpy_asm(count, scalar_val, vec_X, vec_Y, res_ASM);

        int is_valid = 1;
        int err_count = 0;
        for (long long k = 0; k < 10; k++) {
            if (fabs(res_C[k] - res_ASM[k]) > 1e-9) {
                is_valid = 0;
                err_count++;
                printf("   Mismatch at index %lld: C=%.5f vs ASM=%.5f\n", k, res_C[k], res_ASM[k]);
            }
        }

        if (is_valid && err_count == 0) {
            printf("   [SUCCESS] x86-64 Kernel output matches C version.\n");
        } else {
            printf("   [FAILURE] Output mismatch detected.\n");
        }

        // --- BENCHMARKING PHASE ---
        printf(">> Measuring performance (Average of 30 iterations)...\n");
        
        QueryPerformanceFrequency(&freq);
        double time_accumulator = 0.0;

        for (int iter = 0; iter < 30; iter++) {
            QueryPerformanceCounter(&t_start);
            
            // Only time the assembly function
            daxpy_asm(count, scalar_val, vec_X, vec_Y, res_ASM);
            
            QueryPerformanceCounter(&t_end);
            
            double single_run = (double)(t_end.QuadPart - t_start.QuadPart) / freq.QuadPart;
            time_accumulator += single_run;
        }

        printf("   Average Time: %.6f seconds\n", time_accumulator / 30.0);

        // Clean up heap memory
        free(vec_X);
        free(vec_Y);
        free(res_ASM);
        free(res_C);
    }

    printf("\nBenchmark Complete. Press Enter to close.");
    getchar();
    return 0;
}