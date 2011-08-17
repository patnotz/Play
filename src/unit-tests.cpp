#include <gtest/gtest.h>
#include <Play.hpp>

TEST(Play, CompileTimeTraits)
{
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitC<ndim1,ndim2> TraitC;
  play::Expr<TraitC> e;
  ASSERT_NO_THROW(e.eval(ndim1,ndim2));
  ASSERT_EQ(14, e.values(0));
  ASSERT_ANY_THROW(e.set_dims(ndim1+1, ndim2));
}

TEST(Play, RunTimeTraits)
{
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef typename play::TraitR TraitR;
  play::Expr<TraitR> e;

  ASSERT_NO_THROW(e.eval(ndim1,ndim2));
  ASSERT_EQ(14, e.values(0));
  ASSERT_NO_THROW(e.eval(ndim1, ndim2+1));
  ASSERT_EQ(30, e.values(0));
}
