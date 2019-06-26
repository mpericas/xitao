#ifndef SYNTH_MUL
#define SYNTH_MUL

#include "tao.h"
#include <chrono>
#include <iostream>
#include <atomic>
#include <cmath>

#define PSLACK 8  

// Matrix multiplication, tao groupation on written value
class Synth_MatMul : public AssemblyTask 
{
public: 
// initialize static parameters
#if defined(CRIT_PERF_SCHED)
  static float time_table[][GOTAO_NTHREADS];
#endif

  Synth_MatMul(uint32_t _size, int _width): AssemblyTask(_width) {   
    mat_size = _size;
    block_size = mat_size / (_width * PSLACK);
    if(block_size == 0) block_size = 1;
    block_index = 0;
    uint32_t elem_count = mat_size * mat_size;
    A = new double[elem_count]; 
    B_Trans = new double[elem_count];
    C = new double[elem_count];
    block_count = mat_size / block_size;
  }

  int cleanup() { 
    delete[] A;
    delete[] B_Trans;
    delete[] C;
  }

  // this assembly can work totally asynchronously
  int execute(int threadid) {
    while(true) {
      int row_block_id = block_index++;
      if(row_block_id > block_count) return 0;
      // assume B is transposed, so that you can utilize the performance of transposed matmul 
      for (int i = row_block_id * block_size; i < mat_size && i < ((row_block_id + 1 ) * block_size); ++i) { 
        for (int j = 0; j < mat_size; j++) {
          double res  = 0;
          for (int k = 0; k < mat_size; k++) {
            res += A[i*mat_size+k]*B_Trans[j*mat_size+k];
          }
          C[i*mat_size+j] = res;
        }
      }
    }
  }

#if defined(CRIT_PERF_SCHED)
  int set_timetable(int threadid, float ticks, int index) {
    time_table[index][threadid] = ticks;
  }

  float get_timetable(int threadid, int index) { 
    float time=0;
    time = time_table[index][threadid];
    return time;
  }
#endif
private:
  std::atomic<int> block_index; 
  int mat_size;
  int block_count;
  int block_size;
  double* A, *B_Trans, *C;
};

#endif