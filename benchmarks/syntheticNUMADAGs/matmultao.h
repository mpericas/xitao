#ifndef SYNTH_MUL
#define SYNTH_MUL

#include "xitao.h"
#include "dtypes.h"

#include <chrono>
#include <iostream>
#include <atomic>
#include <cmath>
#include <stdio.h>
#include <string.h>
#define PSLACK 8  

// Matrix multiplication, tao groupation on written value
class MatMulTAO : public AssemblyTask 
{
public: 
  MatMulTAO(uint32_t _size, int _width, real_t *_A, real_t *_B): AssemblyTask(_width), A(_A), B(_B) {   
    dim_size = _size;
    block_size = dim_size / (_width * PSLACK);
    if(block_size == 0) block_size = 1;
    block_index = 0;
    uint32_t elem_count = dim_size * dim_size;
    C = new real_t[elem_count];
    block_count = dim_size / block_size;
  }

  void cleanup() { 
  //  delete[] A;
  //  delete[] B;
  //  delete[] C;
  }

  // this assembly can work totally asynchronously
  void execute(int threadid) {
    int _dim = dim_size;
    int _block_count = block_count;
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > _block_count) return;
      // assume B is transposed, so that you can utilize the performance of transposed matmul 
      for (int i = row_block_id * block_size; i < _dim && i < ((row_block_id + 1 ) * block_size); ++i) { 
        for (int j = 0; j < _dim; j++) {
          real_t res  = 0;
          for (int k = 0; k < _dim; k++) {
            res += A[i*_dim+k]*B[j*_dim+k];
          }
          C[i*_dim+j] = res;
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
