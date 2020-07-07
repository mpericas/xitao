#include "matmultao.h"
#include "streamtao.h"
#include "inittao.h"
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <atomic>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace xitao;
using namespace std;

void buildCriticalPath(AssemblyTask* current, vector<AssemblyTask*>& path, vector<real_t*> data, int dim, int wid, int current_depth, int depth, bool toggle) {
  AssemblyTask* next;
  if(++current_depth < depth) {
    if(toggle) {
      next = new MatMulTAO(dim, wid, data[0], data[1]);
      data[2] = ((MatMulTAO*)next)->C;
    } else {
      next = new StreamTAO(dim, wid, data[2]);
    }
    next->clone_sta(current);
    path.push_back(next);
    current->make_edge(next);
    buildCriticalPath(next, path, data, dim, wid, current_depth, depth, !toggle);
  }
}

void buildDAG(AssemblyTask* current, vector<AssemblyTask*>& path, vector<real_t*> data, int dim, int wid, int current_depth, int depth, bool toggle) {
  AssemblyTask* next;
  if(++current_depth < depth) {
    if(toggle) {
      next = new MatMulTAO(dim, wid, data[0], data[1]);
      data[2] = ((MatMulTAO*)next)->C;
    } else {
      next = new StreamTAO(dim, wid, data[2]);
    }
    next->clone_sta(current);
    //path.push_back(next);
    assert(path.size() > current_depth);
    current->make_edge(path[current_depth]);
    path[current_depth]->make_edge(next);
    buildCriticalPath(next, path, data, dim, wid, current_depth, depth, !toggle);
  }
}

int main(int argc, char *argv[]) {
  if(argc < 6) {
    std::cout << "./numabench <Dim Size> <Resource Width> <Degree of Parallelism> <Depth> <Is Fixed Width>" << std::endl; 
    return 0;
  }
  // MatMulTAO(uint32_t _size, int _width, real_t *_A, real_t *_B)
	// StreamTAO(uint32_t _size, int _width, real_t *_input)
 

  int dim_size = atoi(argv[1]);
  int resource_width = atoi(argv[2]);
  int dop = atoi(argv[3]);
  int depth = atoi(argv[4]);
  bool is_fixed_width = atoi(argv[5]) != 0; // mostly for testing NUMA aware scheduling
  if(is_fixed_width) {
    assert((dop % resource_width) == 0);
  } else {
    resource_width = 1;
  }

  int matmul_counter = depth * (dop / resource_width) / 2;
  int stream_counter = depth * (dop / resource_width) / 2; 
  uint32_t elem_count = dim_size * dim_size;
  
  std::cout << "Dim size: " << dim_size << std::endl;
  std::cout << "Resource width: " << resource_width << std::endl;
  std::cout << "Degree of Parallelism: " << dop << std::endl;
  std::cout << "Depth: " << depth << std::endl;
  std::cout << "Is fixed width: " << is_fixed_width << std::endl;

  // vector<MatMulTAO*> matmulTAOs(matmul_counter);
  // vector<StreamTAO*> streamTAOs(stream_counter);
  gotao_init();
  vector<AssemblyTask*> headTAOs;
  bool toggle = false;
  for(int j = 0; j < dop; j+=resource_width) { 
    InitTAO* tao = new InitTAO(dim_size, resource_width);
    tao->set_sta(j / float(dop));
    headTAOs.push_back(tao);
  }
  // build critical path
  InitTAO* tao = dynamic_cast<InitTAO*>(headTAOs[0]);
  assert(tao != NULL);
  vector<AssemblyTask*> critical_path;
  critical_path.push_back(tao);
  buildCriticalPath(tao, critical_path, {tao->A, tao->B, tao->C} ,dim_size, resource_width, 0, depth, false);
  
  for(int i = 1; i < headTAOs.size(); ++i) {
    InitTAO* tao = dynamic_cast<InitTAO*>(headTAOs[i]);
    assert(tao != NULL);
    buildDAG(tao, critical_path, {tao->A, tao->B, tao->C} ,dim_size, resource_width, 0, depth, false);
  }
  for(int i = 0; i < headTAOs.size(); ++i) { 
    gotao_push(headTAOs[i]);
  }

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  auto start1_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(start);
  auto epoch1 = start1_ms.time_since_epoch();

  goTAO_start();

  goTAO_fini();

  end = std::chrono::system_clock::now();

  auto end1_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(end);
  auto epoch1_end = end1_ms.time_since_epoch();
  std::chrono::duration<double> elapsed_seconds = end-start;

// #if defined(CRIT_PERF_SCHED)  
//   xitao_ptt::print_table<Synth_MatMul>("MatMul");
//   xitao_ptt::print_table<Synth_MatCopy>("MaCopy");
//   xitao_ptt::print_table<Synth_MatStencil>("MatStencil");
// #endif
  auto total_taos = matmul_counter + stream_counter;
  std::cout << total_taos <<" Tasks completed in "<< elapsed_seconds.count() << "s\n";
  std::cout << "Assembly Throughput: " << (total_taos) / elapsed_seconds.count() << " A/sec\n"; 
  std::cout << "Total number of steals: " <<  tao_total_steals << "\n";
}
