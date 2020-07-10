#ifndef INIT_TAO_H
#define INIT_TAO_H

#include "xitao.h"
#include "dtypes.h"
#include <stdlib.h>
#include <chrono>
#include <iostream>
#include <atomic>
#include <cmath>
#include <stdio.h>
#include <string.h>
#define PSLACK 8  

// Matrix multiplication, tao groupation on written value
class InitTAO : public AssemblyTask 
{
public: 
  InitTAO(uint32_t _size, int _width): AssemblyTask(_width) {   
    dim_size = _size;
    block_size = dim_size / (_width * PSLACK);
    if(block_size == 0) block_size = 1;
    block_index = 0;
    uint32_t elem_count = dim_size * dim_size;
   // A = new real_t[elem_count];
   // B = new real_t[elem_count];
   // C = new real_t[elem_count];
    A = (real_t*) aligned_alloc(4096, elem_count * sizeof(real_t));
    B = (real_t*) aligned_alloc(4096, elem_count * sizeof(real_t));
    C = (real_t*) aligned_alloc(4096, elem_count * sizeof(real_t));
    block_count = dim_size / block_size;
  }

  void cleanup() { 
  //  delete[] A;
  //  delete[] B;
  //  delete[] C;
  }

  // this assembly can work totally asynchronously
  void execute(int threadid) {
 //   if(threadid == leader) {
 //     memset(A, 0, dim_size * dim_size * sizeof(real_t));
 //     memset(B, 0, dim_size * dim_size * sizeof(real_t));
 //     memset(C, 0, dim_size * dim_size * sizeof(real_t));
 //   }
    int _dim = dim_size;
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > block_count) return;
      // assume B is transposed, so that you can utilize the performance of transposed matmul 
      for (int i = row_block_id * block_size; i < _dim && i < ((row_block_id + 1 ) * block_size); ++i) { 
        int offset_row = i * _dim; 
        for (int j = 0; j < _dim; j++) {
          int offset_col = offset_row + j;
          A[offset_col] = 0;
          B[offset_col] = 0;
          C[offset_col] = 0;
        }
      }
    }
  }
  real_t* C;
  real_t* A;
  real_t* B;

private:
  std::atomic<int> block_index; 
  int dim_size;
  int block_count;
  int block_size;
 
};

#endif
