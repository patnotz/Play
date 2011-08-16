#include <gtest/gtest.h>
#include <Play.hpp>

TEST(Play, CompileTimeTraits)
{
  typedef play::TraitC<2,3> TraitC;
  play::Expr<TraitC> e;
  ASSERT_NO_THROW(TraitC::set_dims(2, 3));
  ASSERT_EQ(6, e.product());
  ASSERT_NE(8, e.product());
  ASSERT_ANY_THROW(TraitC::set_dims(2, 2));
}

TEST(Play, RunTimeTraits)
{
  typedef typename play::TraitR TraitR;
  play::Expr<TraitR> e;
  ASSERT_EQ(0, e.product());

  ASSERT_NO_THROW(TraitR::set_dims(2, 3));
  ASSERT_EQ(6, e.product());
  ASSERT_NE(8, e.product());

  ASSERT_NO_THROW(TraitR::set_dims(2, 2));
  ASSERT_EQ(4, e.product());
}

TEST(Play, CompileTimeTraits2)
{
  typedef play::TraitC<2,3> TraitC;
  play::Expr<TraitC> e;
  ASSERT_EQ(6, e.product(2, 3));
  ASSERT_ANY_THROW(e.product(2, 4));
}

TEST(Play, RunTimeTraits2)
{
  typedef typename play::TraitR TraitR;
  play::Expr<TraitR> e;
  ASSERT_EQ(6, e.product(2, 3));
  ASSERT_EQ(8, e.product(2, 4));
}

TEST(Play, CompileTimeArray2)
{
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitC<ndim1,ndim2> Traits;
  typedef typename Traits::ARRAY2D Array2D;
  Array2D a(ndim1, ndim2);
  for(int i=0; i < ndim1; ++i) {
    for(int j=0; j < ndim2; ++j) {
      a(i,j) = 20;
    }
  }
  ASSERT_DOUBLE_EQ(20, a(ndim1-1,ndim2-1));
  ASSERT_NO_THROW(a.resize(ndim1, ndim2));
  ASSERT_ANY_THROW(a.resize(ndim1+1, ndim2));
  ASSERT_ANY_THROW(Array2D b(ndim1+1,ndim2));
}

TEST(Play, RunTimeArray2)
{
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitR Traits;
  typedef typename Traits::ARRAY2D Array2D;
  Array2D a(ndim1,ndim2);
  for(int i=0; i < ndim1; ++i) {
    for(int j=0; j < ndim2; ++j) {
      a(i,j) = 20;
    }
  }
  ASSERT_DOUBLE_EQ(20, a(ndim1-1,ndim2-1));
  ASSERT_NO_THROW(a.resize(ndim1, ndim2));
  ASSERT_NO_THROW(a.resize(ndim1+1, ndim2));
}
