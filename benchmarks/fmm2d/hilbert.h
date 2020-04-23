#ifndef HILBERT_KEY_H
#define HILBERT_KEY_H
#include <assert.h>
//+++++++++++++++++++++++++++ PUBLIC-DOMAIN SOFTWARE ++++++++++++++++++++++++++
// Functions: TransposetoAxes AxestoTranspose
// Purpose: Transform in-place between Hilbert transpose and geometrical axes
// Example: b=5 bits for each of n=3 coordinates.
// 15-bit Hilbert integer = A B C D E F G H I J K L M N O is stored
// as its Transpose
// X[0] = A D G J M X[2]|
// X[1] = B E H K N <-------> | /X[1]
// X[2] = C F I L O axes |/
// high low 0------ X[0]
// Axes are stored conventially as b-bit integers.
// Author: John Skilling 20 Apr 2001 to 11 Oct 2003
//-----------------------------------------------------------------------------
#define cast_coord(V) static_cast<int>(V)
template <typename float_type>
int* getIntCoords(float_type* coords, float_type* min, float_type* max, int nbins, int dim) {
  int* intCoords = new int[dim];
  for(int i = 0; i < dim; ++i){
    float_type diameter = max[i] - min[i];
    intCoords[i] =  cast_coord((coords[i] - min[i]) / diameter * nbins);
  }
  return intCoords;
}

//! get linear Hilbert address given the x,y,z coordinates
//! migrated from grouptargets.h, but not tested yet
uint64_t getHilbert(int* coords, int nbits, int dim) {
  assert(dim >= 1 && dim <=3);
  int iX[3];
  iX[0] = coords[0];
  iX[1] = (dim > 1)? coords[1] : 0;
  iX[2] = (dim > 2)? coords[2] : 0;
  const int octantMap[8] = {0, 1, 7, 6, 3, 2, 4, 5};
  int mask = 1 << (nbits - 1);
  uint64_t key = 0;
  for (int i = 0; i < nbits; i++) {
    const int ix = (iX[0] & mask) ? 1 : 0;
    const int iy = (iX[1] & mask) ? 1 : 0;
    const int iz = (iX[2] & mask) ? 1 : 0;
    const int octant = (ix << 2) + (iy << 1) + iz;
    if (octant == 0) {
      std::swap(iX[1], iX[2]);
    } else if (octant == 1 || octant == 5) {
      std::swap(iX[0], iX[1]);
    } else if (octant == 4 || octant == 6) {
      iX[0] = (iX[0]) ^ (-1);
      iX[2] = (iX[2]) ^ (-1);
    } else if (octant == 3 || octant == 7) {
      iX[0] = (iX[0]) ^ (-1);
      iX[1] = (iX[1]) ^ (-1);
      std::swap(iX[0], iX[1]);
    } else {
      iX[1] = (iX[1]) ^ (-1);
      iX[2] = (iX[2]) ^ (-1);
      std::swap(iX[1], iX[2]);
    }
    key = (key << 3) + octantMap[octant];
    mask >>= 1;
  }
  return key;
}

template <typename float_type>
uint64_t getHilbert(float_type* coords, float_type* min, float_type* max, int order, int dim) {
  auto intCoords = getIntCoords(coords, min, max, order, dim);
  return getHilbert(intCoords, order, dim);
}

#endif