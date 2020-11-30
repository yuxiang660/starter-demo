#include "foo.h"
#include "gtest/gtest.h"

TEST(Foo, Foo_hello_ok)
{
  Foo f = Foo();
  EXPECT_EQ(1, f.hello(1));
}
