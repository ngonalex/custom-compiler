#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/single_char.h"
#include "frontend/combinators/single_char.h"

#include "gtest/gtest.h"

// Success case for Single Character test :: single_char.cc
TEST(Combinators, SingleCharTest) {
  SingleCharParser test;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";

  EXPECT_EQ(test.parse("a"), result);
}

// Fail case for Single Character test :: single_char.cc
TEST(Combinators, FailedSingleCharTest) {
  SingleCharParser test;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "1";

  EXPECT_EQ(test.parse("1"), result);
}

// Success case for Single Digit test :: single_digit.cc
TEST(Combinators, SingleDigitTest) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  SingleDigitTest test;

  EXPECT_EQ(test.parse("1"), result);
}
