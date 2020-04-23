#ifndef traverse_h
#define traverse_h
#include "exafmm.h"
#include "hilbert.h"
#include <assert.h>
#ifdef USE_XITAO
#include "xitao.h"
#ifdef NUMA_AWARE
#include "numa_utils.h"
#endif
namespace exafmm { 

  class upward_pass: public AssemblyTask { 
  public:  
    Cell* Ci;
    upward_pass(Cell* _ci) : AssemblyTask(1), Ci(_ci){ 
      no_mold = true;
      criticality = 0;
    }
    void execute (int nthread) { 
      assert(width == 1);
      Ci->M.resize(P, 0.0);                                   // Allocate and initialize multipole coefs
      Ci->L.resize(P, 0.0);                                   // Allocate and initialize local coefs
      if(Ci->NCHILD==0) P2M(Ci);                                  // P2M kernel
      M2M(Ci);     
    }
    void cleanup() { } 
  };

  upward_pass* upwardPass(Cell* Ci) { 
    upward_pass* current = new upward_pass(Ci); 
    for (Cell * Cj=Ci->CHILD; Cj!=Ci->CHILD+Ci->NCHILD; Cj++) { // Loop over child cells
      upward_pass* child = upwardPass(Cj);
      child->make_edge(current);
      if(Cj->NCHILD==0) gotao_push(child, rand()%gotao_nthreads);
    }
    return current; 
  } 
   void upwardPass(Cells& cells) {
    Cell* Ci = &cells[0];
    gotao_init();
    upwardPass(Ci);
    gotao_start();
    gotao_fini();
  }

  class horizontalPass_TAO : public AssemblyTask {
  public:
    Cell* Ci;
    Cell* Cj;
    bool spawn_task;
    horizontalPass_TAO(Cell* _ci, Cell* _cj, bool _spawn) : AssemblyTask(1, _spawn), Ci(_ci), Cj(_cj), spawn_task(_spawn) {
      //no_mold = true;
    }
    void execute(int nthread) {
      if(spawn_task && nthread != leader) return;
      for (int d=0; d<2; d++) dX[d] = Ci->X[d] - Cj->X[d];        // Distance vector from source to target
        real_t R2 = norm(dX) * theta * theta;                       // Scalar distance squared
        if (R2 > (Ci->R + Cj->R) * (Ci->R + Cj->R)) {               // If distance is far enough
          M2L(Ci, Cj);
        } else if (Ci->NCHILD == 0 && Cj->NCHILD == 0) {            // Else if both cells are leafs
          P2P(Ci, Cj);
        } else {
          if (Cj->NCHILD == 0 || (Ci->R >= Cj->R && Ci->NCHILD != 0)) {// If Cj is leaf or Ci is larger                     
            for (Cell * ci=Ci->CHILD; ci!=Ci->CHILD+Ci->NCHILD; ci++) {// Loop over Ci's children
              if(ci->NBODY > 100){
                horizontalPass_TAO* tao = new horizontalPass_TAO(ci, Cj, true);
#if STA_AWARE_STEALING
                tao->workload_hint = Cj->STA;
#endif
#if NUMA_AWARE
                std::cout << "NUMA Node" << getRelativeAddress(Cj, numa_count) << std::endl;

#endif
                gotao_push(tao, rand()%gotao_nthreads);
              }
              else {
                horizontalPass_TAO* tao = new horizontalPass_TAO(ci, Cj, false);
                tao->execute(nthread);
              }

            }                                                         //  End loop over Ci's children
          } else {        
            for (Cell * cj=Cj->CHILD; cj!=Cj->CHILD+Cj->NCHILD; cj++) {// Loop over Cj's children
              horizontalPass_TAO* tao = new horizontalPass_TAO(Ci, cj, false);
              tao->execute(nthread);
            }                                                         //  End loop over Cj's children
          }                                                           // End if for leafs and Ci Cj size
      }
    }
    void cleanup() { }
  };

  void horizontalPass(Cells & icells, Cells & jcells) {
    gotao_init();
    horizontalPass_TAO* parent = new horizontalPass_TAO(&icells[0], &jcells[0], true);
    gotao_start(); 
    gotao_push(parent, 0);
    gotao_fini();
  }

  class downwardPass_TAO : public AssemblyTask {
  public:
    Cell* Cj;
    bool spawn_task;
    downwardPass_TAO(Cell* _cj, bool _spawn) : AssemblyTask(1, _spawn), Cj(_cj), spawn_task(_spawn) {
      //no_mold = true;
    }
    void execute(int nthread) {
      if(spawn_task && nthread != leader) return;
      L2L(Cj);                                                    // L2L kernel
      if (Cj->NCHILD==0) L2P(Cj);                                 // L2P kernel
      for (Cell * Ci=Cj->CHILD; Ci!=Cj->CHILD+Cj->NCHILD; Ci++) { // Loop over child cells
        if(Ci->NBODY > 100) {
          downwardPass_TAO* tao = new downwardPass_TAO(Ci, true);
          gotao_push(tao, rand()%gotao_nthreads);
        } else {
          downwardPass_TAO* tao = new downwardPass_TAO(Ci, false);
          tao->execute(nthread);
        }
      }  
    }
    void cleanup() { }
  };

  void downwardPass(Cells & cells) {
    gotao_init();
    downwardPass_TAO* parent = new downwardPass_TAO(&cells[0], true);
    gotao_start(); 
    gotao_push(parent, 0);
    gotao_fini();
  }

}
#endif
namespace exafmm {
  #ifndef USE_XITAO
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
#endif
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
