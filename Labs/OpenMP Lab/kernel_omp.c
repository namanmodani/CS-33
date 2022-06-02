/* 
 * CS 33: OpenMP Lab
 * Name: Naman Modani
 * UCLA ID: 005816643
 * Email: modani@ucla.edu
 */

#include <omp.h>
#include "kernel.h"

void kernel_omp(int* input, int* ref, int64_t rows, int64_t cols, int penalty) {
  int64_t blockedColumns = cols / 32;
  int64_t diagonalCount = (rows / 32) + blockedColumns - 1;
  int64_t upwardDiagonalCount = (diagonalCount + 1) / 2;
  int64_t diag = 0;

  for (; diag != 24; ++diag) {
    for (int64_t col = 0; col != diag + 1; ++col) {
      int startRow = 1 + 32 * (diag - col);
      int startColumn = 1 + 32 * col;
      int64_t endRow = startRow + 32;
      int64_t endColumn = startColumn + 32;

      int64_t index;
      int64_t indexNW;
      int64_t indexN;
      int64_t indexW;
  
      int r;
      int inputNW;
      int inputW;
      int inputN;

      for (; startRow != endRow; ++startRow) {
        for (int64_t col = startColumn; col != endColumn; ++col) {
          index = startRow * cols + col;
          indexNW = index - cols - 1;
          indexN = index - cols;
          indexW = index - 1;
      
          r = ref[index];
          inputNW = input[indexNW];
          inputW = input[indexW];
          inputN = input[indexN];
      
          input[index] = maximum(inputNW + r, inputW - penalty, inputN - penalty);
        }
      }
            
    }
  }

  for (; diag != upwardDiagonalCount; ++diag) {
    #pragma omp parallel for default(none) firstprivate(input, ref, cols, diag, penalty)
    for (int64_t col = 0; col < diag + 1; ++col) {
      int startRow = 1 + 32 * (diag - col);
      int startColumn = 1 + 32 * col;
      int64_t endRow = startRow + 32;
      int64_t endColumn = startColumn + 32;

      int64_t index;
      int64_t indexNW;
      int64_t indexN;
      int64_t indexW;
  
      int r;
      int inputNW;
      int inputW;
      int inputN;

      for (; startRow != endRow; ++startRow) {
        for (int64_t col = startColumn; col != endColumn; ++col) {
          index = startRow * cols + col;
          indexNW = index - cols - 1;
          indexN = index - cols;
          indexW = index - 1;
      
          r = ref[index];
          inputNW = input[indexNW];
          inputW = input[indexW];
          inputN = input[indexN];
      
          input[index] = maximum(inputNW + r, inputW - penalty, inputN - penalty);
        }
      }
    }
  }

  for (; diag != diagonalCount; ++diag) {
    #pragma omp parallel for default(none) firstprivate(input, ref, cols, diag, penalty, upwardDiagonalCount, blockedColumns)
    for (int64_t col = diag - upwardDiagonalCount + 1; col < blockedColumns; ++col) {
      int startRow = 1 + 32 * (diag - col);
      int startColumn = 1 + 32 * col;
      int64_t endRow = startRow + 32;
      int64_t endColumn = startColumn + 32;

      int64_t index;
      int64_t indexNW;
      int64_t indexN;
      int64_t indexW;
  
      int r;
      int inputNW;
      int inputW;
      int inputN;

      for (; startRow != endRow; ++startRow) {
        for (int64_t col = startColumn; col != endColumn; ++col) {
          index = startRow * cols + col;
          indexNW = index - cols - 1;
          indexN = index - cols;
          indexW = index - 1;
      
          r = ref[index];
          inputNW = input[indexNW];
          inputW = input[indexW];
          inputN = input[indexN];
      
          input[index] = maximum(inputNW + r, inputW - penalty, inputN - penalty);
        }
      }
    }
  }
}