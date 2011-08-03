// Copyright 2011 Patrick Notz
#ifndef SRC_PLAY_HPP_
#define SRC_PLAY_HPP_

#include <iostream>

namespace play {

// This "trait" class has the dimensions fixed at compile time
template <int NDIM1, int NDIM2>
struct TraitC {
  static void set_dims(const int dim1, const int dim2) {
    if (dim1 != NDIM1 || dim2 != NDIM2) {
      throw "can't change dims on compile time traits";
    }
  }
  static const int ndim1 = NDIM1;
  static const int ndim2 = NDIM2;
};

// This "trait" class allows you to change the dimensions
// via static members at runtime
struct TraitR {
  static void set_dims(const int dim1, const int dim2) {
    ndim1 = dim1;
    ndim2 = dim2;
  }
  static int ndim1;
  static int ndim2;
};
int TraitR::ndim1 = 0;
int TraitR::ndim2 = 0;

// A templated class that uses traits to get dimensions.
// Question: if T is such that ndimX are known at compile
// time will the optimizer unroll loops, etc.?
template <class T>
struct Expr {
  int product() {
    return T::ndim1 * T::ndim2;
  }
  int product(const int dim1, const int dim2) {
    T::set_dims(dim1, dim2);
    return product();
  }
};

}  // namespace play

#endif  // SRC_PLAY_HPP_
