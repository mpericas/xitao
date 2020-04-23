#ifndef numa_utils_h
#define numa_utils_h
#include <numaif.h>
#include <numa.h>
#include <iostream>
int getNUMACount(){
  if(numa_available() == -1) {
  	std::cout << "Error: NUMA library is undefined" << std::endl;
  	exit(0);
  } 
  return numa_num_configured_nodes();
}
float getRelativeAddress(void* ptr, int numa_count, int thread_count) {
  if(numa_available() == -1) {
  	std::cout << "Error: NUMA library is undefined" << std::endl;
  	exit(0);
  } 
  int numa_node = -1;
  get_mempolicy(&numa_node, NULL, 0, ptr, MPOL_F_NODE | MPOL_F_ADDR);
  float lower = numa_node/float(numa_count);
  float upper = (numa_node+1)/float(numa_count);
  float lowerThreadIndex = lower * thread_count; 
  float upperThreadIndex = upper * thread_count;
  int diff = upperThreadIndex - lowerThreadIndex; 
  return((rand()%diff + lowerThreadIndex)/thread_count);
}

#endif