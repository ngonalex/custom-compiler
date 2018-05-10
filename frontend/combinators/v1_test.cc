#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/single_char.h"
#include "frontend/combinators/single_digit.h"
#include "frontend/combinators/parsestatus.h"
#include "frontend/combinators/and_combinator.h"

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
TEST(Combinators, SuccessSingleDigitTest) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  SingleDigitParser test;

  EXPECT_EQ(test.parse("1"), result);
}

// Failed case for Single Digit test :: single_digit.cc
TEST(Combinators, FailedSingleDigitTest) {
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "a";
  SingleDigitParser test;

  EXPECT_EQ(test.parse("a"), result);
}

// TODO below

// NOTE this test doesn't work
// one or more combinators test
TEST(Combinators, DigitOrCharCombinator) {
  SingleDigitParser digitParser;
  SingleCharParser charParser;

  AndCombinator andC;
  andC.firstParser = reinterpret_cast<NullParser *>(&digitParser);
  andC.secondParser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";

  EXPECT_EQ(andC.parse("a"), result);
}
/*
// Failed case for Single Digit test :: single_digit.cc
TEST(Combinators, DigitAndLetterCombinator) {
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "a";
  SingleDigitParser test;

  EXPECT_EQ(test.parse("a"), result);
}*/
