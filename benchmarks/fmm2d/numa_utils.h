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
float getRelativeAddress(void* ptr, int count) {
  if(numa_available() == -1) {
  	std::cout << "Error: NUMA library is undefined" << std::endl;
  	exit(0);
  } 

  int numa_node = -1;
  get_mempolicy(&numa_node, NULL, 0, ptr, MPOL_F_NODE | MPOL_F_ADDR);
  return float(numa_node)/float(count);
//  return numa_node;
}

#endif