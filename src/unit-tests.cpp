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
