#ifndef TRIAD_TAO
#define TRIAD_TAO

#include "xitao.h"
#include "dtypes.h"
#include <chrono>
#include <iostream>
#include <atomic>
#include <cmath>
#include <stdio.h>
#define PSLACK 8  

class TriadTAO : public AssemblyTask 
{
public: 

  TriadTAO(uint32_t _size, int _width, real_t *_input): AssemblyTask(_width), input(_input) {   
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
    int _dim = dim_size;
    int _block_count = block_count;
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > _block_count) return;
      int row_block_start =  row_block_id      * block_size;
      int row_block_end   = (row_block_id + 1) * block_size;
      int end = (_dim < row_block_end) ? _dim : row_block_end; 
      for (int i = row_block_start; i < end; ++i) { 
        for(int j = 0; j < _dim; ++j) {
          output[i + j] = 1.0;
          output[i + j] = PSLACK * input[i + j] + output[i + j];
         // std::copy(input + (i * _dim), input + (i * _dim) + _dim, output + i * _dim);
        }
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
