/* 
 * CS 33: OpenMP Lab
 * Naman Modani
 * modani@ucla.edu
 * kernel_omp.c
 */

#include <omp.h>
#include <stdio.h>

#include "kernel.h"

void kernel_omp(int *input, int *ref, int64_t rows, int64_t cols, int penalty) 
{
    int blockSize = 64;
    int unrollType = 8;

    for (int x = 1; x < 2 * rows - blockSize; x += blockSize) 
    {
        int overflow = (x >= rows) ? (x - rows + blockSize) : 0;
        #pragma omp parallel for
        for (int y = overflow; y <= x - overflow; y += blockSize) 
        {
            int a = x - y;
            int b = y + 1;

            for (int i = a; i < a + blockSize; i++) 
            {
                for (int j = b; j < b + blockSize; j += unrollType) 
                {
                    int64_t idx = i * cols + j;
                    int64_t idxNW = idx - cols - 1;
                    int64_t idxN = idx - cols;
                    int64_t idxW = idx - 1;
                    int r = ref[idx];

                    int inputNW = input[idxNW];
                    int inputW = input[idxW];
                    int inputN = input[idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);

                    r = ref[++idx];
                    inputNW = input[++idxNW];
                    inputW = input[++idxW];
                    inputN = input[++idxN];
                    input[idx] = maximum(inputNW + r, inputW - penalty, inputN - penalty);
                }
            }
        }
    }
}