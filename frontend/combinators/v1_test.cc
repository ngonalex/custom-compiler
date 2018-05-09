#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/single_char.h"
#include "frontend/combinators/single_char.h"

#include "gtest/gtest.h"

TEST(Combinators, SingleCharTest) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = '';
  SingleCharParser test();

  EXPECT_EQ(test.parse(), result);
}
