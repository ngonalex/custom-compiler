#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/single_char.h"
#include "frontend/combinators/single_char.h"

#include "gtest/gtest.h"

//Basic single_char.cc test
TEST(Combinators, SingleCharTest) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  SingleCharParser test;

  EXPECT_EQ(test.parse("a"), result);
}

// Basic single_digit.cc test
TEST(Combinators, SingleDigitTest) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  SingleCharParser test;

  EXPECT_EQ(test.parse("a"), result);
}
