#ifndef exafmm_h
#define exafmm_h
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <limits>

namespace exafmm {
  //! Basic type definitions
  typedef double real_t;                                        //!< Floating point type is single precision
  typedef std::complex<real_t> complex_t;                       //!< Complex type

  //! Structure of bodies
  struct Body {
    real_t X[2];                                                //!< Position
    real_t q;                                                   //!< Charge
    real_t p;                                                   //!< Potential
    real_t F[2];                                                //!< Force
  };
  typedef std::vector<Body> Bodies;                             //!< Vector of bodies
  //! Struction of Max/Min bounding box
  struct Box {
    real_t minX[2];
    real_t maxX[2];
    Box() { 
      minX[0] = std::numeric_limits<real_t>::max();          
      minX[1] = std::numeric_limits<real_t>::max();
      maxX[0] = std::numeric_limits<real_t>::min();
      maxX[1] = std::numeric_limits<real_t>::min();
    }
  };
  //! Structure of cells
  struct Cell {
    int NCHILD;                                                 //!< Number of child cells
    int NBODY;                                                  //!< Number of descendant bodies
    Cell * CHILD;                                               //!< Pointer of first child cell
    Body * BODY;                                                //!< Pointer of first body
    real_t X[2];                                                //!< Cell center
    real_t R;                                                   //!< Cell radius
#if EXAFMM_LAZY
    std::vector<Cell*> listM2L;                                 //!< M2L interaction list
    std::vector<Cell*> listP2P;                                 //!< P2P interaction list
#endif
    std::vector<complex_t> M;                                   //!< Multipole expansion coefficients
    std::vector<complex_t> L;                                   //!< Local expansion coefficients
    size_t STA;                                                 //!< The software topology address
  };
  typedef std::vector<Cell> Cells;                              //!< Vector of cells

  //! Global variables
  int P;                                                        //!< Order of expansions
  int ncrit;                                                    //!< Number of bodies per leaf cell
  real_t theta;                                                 //!< Multipole acceptance criterion
  real_t dX[2];                                                 //!< Distance vector
  Box bounds;                                                   //!< Declare the bounding box
#if NUMA_AWARE
  int numa_count;
#endif
#pragma omp threadprivate(dX)                                   //!< Make global variables private
}

#endif
