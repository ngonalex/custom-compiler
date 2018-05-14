#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/single_op.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/close_paren.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/open_paren.h"




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

// Simple AND combinator test
TEST(Combinators, DigitAndCharCombinator) {
  SingleDigitParser digitParser;
  SingleCharParser charParser;

  AndCombinator andC;
  andC.firstParser = reinterpret_cast<NullParser *>(&digitParser);
  andC.secondParser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "a";

  EXPECT_EQ(andC.parse("a"), result);
}

// Simple OR combinator test
TEST(Combinators, DigitOrCharCombinator) {
  SingleDigitParser digitParser;
  SingleCharParser charParser;

  OrCombinator orC;
  orC.firstParser = reinterpret_cast<NullParser *>(&digitParser);
  orC.secondParser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";

  EXPECT_EQ(orC.parse("a"), result);
}

// single_op test
TEST(Combinators, SingleOp) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  SingleOperatorParser test;

  EXPECT_EQ(test.parse("*"), result);
}

// single_op fail test
TEST(Combinators, FailSingleOp) {

  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "a";
  SingleDigitParser test;

  EXPECT_EQ(test.parse("a"), result);
}

// Zero or more combinator test
TEST(Combinators, ZeroOrMoreCombinator) {
  SingleDigitParser digitParser;
  ZeroOrMoreCombinator zeroOrMore;

  zeroOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "ab";

  EXPECT_EQ(zeroOrMore.parse("123ab"), result);
}

// Passing in nothing for Zero or more combinator test
TEST(Combinators, NoMatchZeroOrMoreCombinator) {
  SingleDigitParser digitParser;
  ZeroOrMoreCombinator zeroOrMore;

  zeroOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "ab";

  EXPECT_EQ(zeroOrMore.parse("ab"), result);
}


// One or more combinator test
TEST(Combinators, OneOrMoreCombinator) {
  SingleCharParser charParser;
  OneOrMoreCombinator oneOrMore;

  oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "1cd";

  EXPECT_EQ(oneOrMore.parse("ab1cd"), result);
}

// Fail Case for One or more combinator test
TEST(Combinators, FailOneOrMoreCombinator) {
  SingleDigitParser digitParser;
  OneOrMoreCombinator oneOrMore;

  oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "ab1cd";

  EXPECT_EQ(oneOrMore.parse("ab1cd"), result);
}


// open_paren test
TEST(Combinators, OpenParen) {
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "abc";
  OpenParenParser test;

  EXPECT_EQ(test.parse("(abc"), result);
}