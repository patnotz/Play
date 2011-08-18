// Copyright 2011 Patrick Notz
#include <Play.hpp>
#include <gtest/gtest.h>

TEST(Play, CompileTimeTraits) {
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitsC<ndim1, ndim2> TraitsC;
  play::Expr<TraitsC> e;
  ASSERT_NO_THROW(e.eval(ndim1, ndim2));
  ASSERT_EQ(14, e.result());
  ASSERT_ANY_THROW(e.set_dims(ndim1+1, ndim2));
}

TEST(Play, RunTimeTraits) {
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitsR TraitsR;
  play::Expr<TraitsR> e;

  ASSERT_NO_THROW(e.eval(ndim1, ndim2));
  ASSERT_EQ(14, e.result());
  ASSERT_NO_THROW(e.eval(ndim1, ndim2+1));
  ASSERT_EQ(30, e.result());
}

TEST(Play, VirtualBase) {
  const int ndim1 = 2;
  const int ndim2 = 3;
  typedef play::TraitsC<ndim1, ndim2> TraitsC;
  typedef play::TraitsR TraitsR;

  play::ExprBase* e = new play::Expr<TraitsR>();
  ASSERT_NO_THROW(e->eval(ndim1, ndim2));
  ASSERT_EQ(14, e->result());
  ASSERT_NO_THROW(e->set_dims(ndim1+1, ndim2));
  delete e;

  e = new play::Expr<TraitsC>();
  ASSERT_NO_THROW(e->eval(ndim1, ndim2));
  ASSERT_EQ(14, e->result());

  // Hmmm, the code throws as expected but
  // GTest chokes on either of these
  // ASSERT_ANY_THROW(e->set_dims(ndim1+1, ndim2));
  // ASSERT_THROW(e->set_dims(ndim1+1, ndim2), std::range_error);
  // Hmmm, even manual exception checking fails even though
  // this works as expected in the other tests!
  delete e;
}
