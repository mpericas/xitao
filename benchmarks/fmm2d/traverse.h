#ifndef traverse_h
#define traverse_h
#include "exafmm.h"
#include "hilbert.h"
#include <assert.h>
#ifdef USE_XITAO
#include "xitao.h"
#include <stack>
#include <set>
#ifdef NUMA_AWARE
#include "numa_utils.h"
#endif
#endif
namespace exafmm {
  //! Recursive call to post-order tree traversal for upward pass
  void upwardPass(Cell * Ci) {
    for (Cell * Cj=Ci->CHILD; Cj!=Ci->CHILD+Ci->NCHILD; Cj++) { // Loop over child cells
#pragma omp task untied if(Cj->NBODY > 100)                     //  Start OpenMP task if large enough task
      upwardPass(Cj);                                           //  Recursive call for child cell
    }                                                           // End loop over child cells
#pragma omp taskwait                                            // Synchronize OpenMP tasks
    Ci->M.resize(P, 0.0);                                   // Allocate and initialize multipole coefs
    Ci->L.resize(P, 0.0);                                   // Allocate and initialize local coefs
    if(Ci->NCHILD==0) P2M(Ci);                                  // P2M kernel
    M2M(Ci);                                                    // M2M kernel
  }

  //! Upward pass interface
  void upwardPass(Cells & cells) {
#pragma omp parallel                                            // Start OpenMP
#pragma omp single nowait                                       // Start OpenMP single region with nowait
    upwardPass(&cells[0]);                                      // Pass root cell to recursive call
  }
#ifdef USE_XITAO
#ifdef USE_DATA_PARALLEL
  //! Recursive call to dual tree traversal for list construction
  void getList(Cell * Ci, Cell * Cj) {
    for (int d=0; d<2; d++) dX[d] = Ci->X[d] - Cj->X[d];        // Distance vector from source to target
    real_t R2 = norm(dX) * theta * theta;                       // Scalar distance squared
    if (R2 > (Ci->R + Cj->R) * (Ci->R + Cj->R)) {               // If distance is far enough
      Ci->listM2L.push_back(Cj);                                //  Add to M2L list
    } else if (Ci->NCHILD == 0 && Cj->NCHILD == 0) {            // Else if both cells are leafs
      Ci->listP2P.push_back(Cj);                                //  Add to P2P list
    } else if (Cj->NCHILD == 0 || (Ci->R >= Cj->R && Ci->NCHILD != 0)) {// Else if Cj is leaf or Ci is larger
      for (Cell * ci=Ci->CHILD; ci!=Ci->CHILD+Ci->NCHILD; ci++) {// Loop over Ci's children
        getList(ci, Cj);                                        //   Recursive call to target child cells
      }                                                         //  End loop over Ci's children
    } else {                                                    // Else if Ci is leaf or Cj is larger
      for (Cell * cj=Cj->CHILD; cj!=Cj->CHILD+Cj->NCHILD; cj++) {//  Loop over Cj's children
        getList(Ci, cj);                                        //   Recursive call to source child cells
      }                                                         //  End loop over Cj's children
    }                                                           // End if for leafs and Ci Cj size
  }

  //! Evaluate M2L, P2P kernels
  void evaluate(Cells& cells) {
   int i = 0;
   int tao_width = 1;
   int N = cells.size();
 __xitao_vec_multiparallel_region(tao_width, i, N, xitao_vec_dynamic, 10, 
     {                     // Loop over cells
    
      for (size_t j=0; j<cells[i].listM2L.size(); j++) {        //  Loop over M2L list
        M2L(&cells[i],cells[i].listM2L[j]);                     //   M2L kernel
      }                                                         //  End loop over M2L list
      for (size_t j=0; j<cells[i].listP2P.size(); j++) {        //  Loop over P2P list
        P2P(&cells[i],cells[i].listP2P[j]);                     //   P2P kernel
      }                                                         //  End loop over P2P list
   }                                                           // End loop over cells
  );

  }

  //! Horizontal pass interface
  void horizontalPass(Cells & icells, Cells & jcells) {
    getList(&icells[0], &jcells[0]);                            // Pass root cell to recursive call
    gotao_init();
    gotao_start();  
    evaluate(icells);                                           // Evaluate M2L & P2P kernels
    gotao_fini();
  }
#else
  typedef std::stack<std::pair<Cell*, Cell*>> TraversalStack;
  void continueHorizontalPass(TraversalStack* recursive_stack);
  class horizontalPass_TAO : public AssemblyTask {
  public:
    Cell* Ci;
    Cell* Cj;
    horizontalPass_TAO(Cell* _ci, Cell* _cj) : AssemblyTask(1), Ci(_ci), Cj(_cj){
    }
    void execute(int nthread) {
      if(nthread != leader) return;
      TraversalStack* recursive_stack = new TraversalStack();
      recursive_stack->emplace(std::make_pair(Ci, Cj));
      continueHorizontalPass(recursive_stack);
    }
    void cleanup() { }
  };
  
  class horizontalPassContinuation_TAO : public AssemblyTask {
  public:
    TraversalStack* recursive_stack;
    horizontalPassContinuation_TAO(TraversalStack* st) : AssemblyTask(1), recursive_stack(st){
    }
    void execute(int nthread) {
      if(nthread != leader) return;
      continueHorizontalPass(recursive_stack);
    }
    void cleanup() { }
  };

  void continueHorizontalPass(TraversalStack* recursive_stack){
      while(!recursive_stack->empty()) {
      std::vector<horizontalPass_TAO*> children;
      auto top = recursive_stack->top();
      auto Ci = top.first; 
      auto Cj = top.second;
      recursive_stack->pop();
      for (int d=0; d<2; d++) dX[d] = Ci->X[d] - Cj->X[d];        // Distance vector from source to target
      real_t R2 = norm(dX) * theta * theta;                       // Scalar distance squared
      if (R2 > (Ci->R + Cj->R) * (Ci->R + Cj->R)) {               // If distance is far enough
        M2L(Ci, Cj);                                              //  M2L kernel
      } else if (Ci->NCHILD == 0 && Cj->NCHILD == 0) {            // Else if both cells are leafs
        P2P(Ci, Cj);                                              //  P2P kernel
      }
      else if (Cj->NCHILD == 0 || (Ci->R >= Cj->R && Ci->NCHILD != 0)) {// If Cj is leaf or Ci is larger
      for (Cell * ci=Ci->CHILD; ci!=Ci->CHILD+Ci->NCHILD; ci++) {// Loop over Ci's children
        if(ci->NBODY > 100) {
          horizontalPass_TAO* tao = new horizontalPass_TAO(ci, Cj);
          children.push_back(tao);
#if STA_AWARE_STEALING
          tao->workload_hint = Cj->STA;
#endif
#if NUMA_AWARE
          tao->set_sta(getRelativeAddress(ci, numa_count, gotao_nthreads));
#endif
        } else {
          recursive_stack->emplace(std::make_pair(ci, Cj));
        }
      }                                                         //  End loop over Ci's children
    } else {                                                    // Else if Ci is leaf or Cj is larger
      for (Cell * cj=Cj->CHILD; cj!=Cj->CHILD+Cj->NCHILD; cj++) {// Loop over Cj's children
        recursive_stack->emplace(std::make_pair(Ci, cj));
      }                                                         //  End loop over Cj's children
    }                
      if(children.size() > 0) {
        horizontalPassContinuation_TAO* contTAO = new horizontalPassContinuation_TAO(recursive_stack);
        for(auto child : children) {
          child->make_edge(contTAO);
          gotao_push(child);
        }
        return;
      }
    }
  }
  void horizontalPass(Cells & icells, Cells & jcells) {
    gotao_init();
    gotao_start(); 
    horizontalPass_TAO* parent = new horizontalPass_TAO(&icells[0], &jcells[0]);
    gotao_push(parent, 0);
    gotao_fini();
#ifdef TRACK_STA
  std::set<size_t> visited_sta;
  for(size_t ind = 0; ind < icells.size(); ++ind) {
    auto current_sta = icells[ind].STA;
    if(visited_sta.find(current_sta) == visited_sta.end()) {
      xitao_ptt::print_schedule_map<horizontalPass_TAO>(current_sta);
      xitao_ptt::print_schedule_map<horizontalPassContinuation_TAO>(current_sta);
    }
  }
#endif
  }
#endif
#else
  //! Recursive call to dual tree traversal for horizontal pass
  void horizontalPass(Cell * Ci, Cell * Cj) {
    for (int d=0; d<2; d++) dX[d] = Ci->X[d] - Cj->X[d];        // Distance vector from source to target
    real_t R2 = norm(dX) * theta * theta;                       // Scalar distance squared
    if (R2 > (Ci->R + Cj->R) * (Ci->R + Cj->R)) {               // If distance is far enough
      M2L(Ci, Cj);                                              //  M2L kernel
    } else if (Ci->NCHILD == 0 && Cj->NCHILD == 0) {            // Else if both cells are leafs
      P2P(Ci, Cj);                                              //  P2P kernel
    } else if (Cj->NCHILD == 0 || (Ci->R >= Cj->R && Ci->NCHILD != 0)) {// If Cj is leaf or Ci is larger
      for (Cell * ci=Ci->CHILD; ci!=Ci->CHILD+Ci->NCHILD; ci++) {// Loop over Ci's children
#pragma omp task untied if(ci->NBODY > 100)                     //   Start OpenMP task if large enough task
        horizontalPass(ci, Cj);                                 //   Recursive call to target child cells
      }                                                         //  End loop over Ci's children
    } else {                                                    // Else if Ci is leaf or Cj is larger
      for (Cell * cj=Cj->CHILD; cj!=Cj->CHILD+Cj->NCHILD; cj++) {// Loop over Cj's children
        horizontalPass(Ci, cj);                                 //   Recursive call to source child cells
      }                                                         //  End loop over Cj's children
    }                                                           // End if for leafs and Ci Cj size
 #pragma omp taskwait                                            // Synchronize OpenMP tasks
  }

  //! Horizontal pass interface
  void horizontalPass(Cells & icells, Cells & jcells) {
#pragma omp parallel                                            // Start OpenMP
#pragma omp single nowait                                       // Start OpenMP single region with nowait
    horizontalPass(&icells[0], &jcells[0]);                     // Pass root cell to recursive call
  }
#endif
  //! Recursive call to pre-order tree traversal for downward pass
  void downwardPass(Cell * Cj) {
    L2L(Cj);                                                    // L2L kernel
    if (Cj->NCHILD==0) L2P(Cj);                                 // L2P kernel
    for (Cell * Ci=Cj->CHILD; Ci!=Cj->CHILD+Cj->NCHILD; Ci++) { // Loop over child cells
#pragma omp task untied if(Ci->NBODY > 100)                     //  Start OpenMP task if large enough task
      downwardPass(Ci);                                         //  Recursive call for child cell
    }                                                           // End loop over chlid cells
#pragma omp taskwait                                            // Synchronize OpenMP tasks
  }

  //! Downward pass interface
  void downwardPass(Cells & cells) {
#pragma omp parallel                                            // Start OpenMP
#pragma omp single nowait                                       // Start OpenMP single region with nowait
    downwardPass(&cells[0]);                                    // Pass root cell to recursive call
  }

  //! Direct summation
  void direct(Bodies & bodies, Bodies & jbodies) {
    Cells cells(2);                                             // Define a pair of cells to pass to P2P kernel
    Cell * Ci = &cells[0];                                      // Allocate single target
    Cell * Cj = &cells[1];                                      // Allocate single source
    Ci->BODY = &bodies[0];                                      // Iterator of first target body
    Ci->NBODY = bodies.size();                                  // Number of target bodies
    Cj->BODY = &jbodies[0];                                     // Iterator of first source body
    Cj->NBODY = jbodies.size();                                 // Number of source bodies
    P2P(Ci, Cj);                                                // Evaluate P2P kenrel
  }
}
#endif
