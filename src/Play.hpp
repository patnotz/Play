// Copyright 2011 Patrick Notz
#ifndef SRC_PLAY_HPP_
#define SRC_PLAY_HPP_

#include <iostream>

namespace play {

struct ArrayR1 {
  ArrayR1()
    : m_ndim1(0) {}
  ArrayR1(
      const int ndim1)
    : m_ndim1(ndim1) {
    resize(ndim1);
  }
  void resize(const int ndim1) {
    m_ndim1 = ndim1;
    m_array.resize(ndim1);
  }
  double & operator()(const int i) {
    return *(m_array.begin() + i);
  }
  int m_ndim1;
  std::vector<double> m_array;
};

struct ArrayR2 {
  ArrayR2()
    : m_ndim1(0),
      m_ndim2(0) {}
  ArrayR2(
      const int ndim1,
      const int ndim2)
    : m_ndim1(ndim1),
      m_ndim2(ndim2) {
    resize(ndim1,ndim2);
  }
  void resize(const int ndim1, const int ndim2) {
    m_ndim1 = ndim1;
    m_ndim2 = ndim2;
    m_array.resize(ndim1*ndim2);
  }
  double & operator()(const int i, const int j) {
    return *(m_array.begin() + i*m_ndim1 + j);
  }
  int m_ndim1;
  int m_ndim2;
  std::vector<double> m_array;
};

template <int NDIM1>
struct ArrayC1 {
  ArrayC1() {}
  ArrayC1(
      const int ndim1) {
    resize(ndim1);
  }
  void resize(const int ndim1) {
    if (ndim1 != NDIM1) {
      throw "can't change dims on compile time array";
    }
  }
  double & operator()(const int i) {
    return m_array[i];
  }
  double m_array[NDIM1];
};

template <int NDIM1, int NDIM2>
struct ArrayC2 {
  ArrayC2() {}
  ArrayC2(
      const int ndim1,
      const int ndim2) {
    resize(ndim1,ndim2);
  }
  void resize(const int ndim1, const int ndim2) {
    if (ndim1 != NDIM1 || ndim2 != NDIM2) {
      throw "can't change dims on compile time array";
    }
  }
  double & operator()(const int i, const int j) {
    return m_array[i][j];
  }
  double m_array[NDIM1][NDIM2];
};

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
  typedef ArrayC1<NDIM1> ARRAY_1;
  typedef ArrayC2<NDIM1,NDIM2> ARRAY_12;
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
  typedef ArrayR1 ARRAY_1;
  typedef ArrayR2 ARRAY_12;
};
int TraitR::ndim1 = 0;
int TraitR::ndim2 = 0;

// A templated class that uses traits to get dimensions.
// Question: if T is such that ndimX are known at compile
// time will the optimizer unroll loops, etc.?
template <class T>
struct Expr {
  void set_dims(const int dim1, const int dim2) {
    T::set_dims(dim1, dim2);
    values.resize(dim1);
    vector.resize(dim1,dim2);
  }
  int eval(const int dim1, const int dim2) {
    set_dims(dim1, dim2);
    int result = 0;
    for(int i=0; i < T::ndim1; ++i) {
      values(i) = 0;
      for(int j=0; j < T::ndim2; ++j) {
        vector(i,j) = j+1;
        values(i) += vector(i,j) * vector(i,j);
      }
    }
    return result;
  }
  typename T::ARRAY_1 values;
  typename T::ARRAY_12 vector;
};

}  // namespace play

#endif  // SRC_PLAY_HPP_
