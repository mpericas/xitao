#ifndef INIT_TAO_H
#define INIT_TAO_H

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
class InitTAO : public AssemblyTask 
{
public: 
  InitTAO(uint32_t _size, int _width): AssemblyTask(_width) {   
    dim_size = _size;
    block_size = dim_size / (_width * PSLACK);
    if(block_size == 0) block_size = 1;
    block_index = 0;
    uint32_t elem_count = dim_size * dim_size;
    A = new real_t[elem_count];
    B = new real_t[elem_count];
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
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > block_count) return;
      // assume B is transposed, so that you can utilize the performance of transposed matmul 
      for (int i = row_block_id * block_size; i < dim_size && i < ((row_block_id + 1 ) * block_size); ++i) { 
        for (int j = 0; j < dim_size; j++) {
          A[i*dim_size+j] = 0;
          B[i*dim_size+j] = 0;
          C[i*dim_size+j] = 0;
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
