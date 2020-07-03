#ifndef SYNTH_COPY
#define SYNTH_COPY

#include "xitao.h"
#include "dtypes.h"
#include <chrono>
#include <iostream>
#include <atomic>
#include <cmath>
#include <stdio.h>
#define PSLACK 8  

class StreamTAO : public AssemblyTask 
{
public: 

  StreamTAO(uint32_t _size, int _width, real_t *_input): AssemblyTask(_width), input(_input) {   
    dim_size = _size;
    block_size = dim_size / (_width * PSLACK);
    if(block_size == 0) block_size = 1;
    block_index = 0;
    uint32_t elem_count = dim_size * dim_size;
    output = new real_t[elem_count];
    block_count = dim_size / block_size;
  }

  void cleanup(){ 
    //delete[] A;
    //delete[] B;
  }

  // this assembly can work totally asynchronously
  void execute(int threadid) {
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > block_count) return;
      int row_block_start =  row_block_id      * block_size;
      int row_block_end   = (row_block_id + 1) * block_size;
      int end = (dim_size < row_block_end) ? dim_size : row_block_end; 
      for (int i = row_block_start; i < end; ++i) { 
         std::copy(input + (i * dim_size), input + (i * dim_size) + dim_size, output + i * dim_size);
      }
    }
  }
  
  real_t* output;

private:
  std::atomic<int> block_index; 
  int block_size; 
  int dim_size;
  int block_count;
  real_t *input;
};


#endif
