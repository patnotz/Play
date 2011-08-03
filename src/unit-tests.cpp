#include <gtest/gtest.h>
#include <Play.hpp>

const play::IndexT SIZE = 100000;

// This test is mostly meant as a concise example of how to parse and access
// Json data and to make sure the build is working. It's not meant to an
// exhaustive test of the JsonCPP package.
TEST(Play, Pass)
{
  ASSERT_TRUE(SIZE > 0);
}

TEST(Play, TraitC)
{
  typedef play::TraitC<2,3> TraitC;
  play::Expr<TraitC> e;
  ASSERT_NO_THROW(TraitC::set_dims(2, 3));
  ASSERT_EQ(6, e.product());
  ASSERT_NE(8, e.product());
  ASSERT_ANY_THROW(TraitC::set_dims(2, 2));
}

TEST(Play, TraitR)
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
