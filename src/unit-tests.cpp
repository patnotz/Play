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
