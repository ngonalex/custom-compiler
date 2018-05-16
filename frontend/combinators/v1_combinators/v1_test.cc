#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
// #include "frontend/combinators/basic_combinators/parsestatus.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
// #include "frontend/combinators/v1_combinators/mul_div_op.h"
// #include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
// #include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
// #include "frontend/combinators/v1_combinators/ae.h"
// #include "frontend/combinators/v1_combinators/close_paren.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
// #include "frontend/combinators/v1_combinators/open_paren.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

// Success case for Single Character test :: single_char.cc
TEST(Combinators, SingleCharTest) {
  SingleCharParser test;
  ParseStatus result = test.parse("a123");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "123");
  EXPECT_EQ(result.parsedCharacters, "a");
}

// Fail case for Single Character test :: single_char.cc
TEST(Combinators, FailedSingleCharTest) {
  SingleCharParser test;
  ParseStatus result = test.parse("1");
  result.status = false;
  result.remainingCharacters = "1";

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "1");
  EXPECT_EQ(result.parsedCharacters, "");
}

// Success case for Single Digit test :: single_digit.cc
TEST(Combinators, SuccessSingleDigitTest) {
  SingleDigitParser test;
  ParseStatus result = test.parse("1"); 

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "1");
}

// Failed case for Single Digit test :: single_digit.cc
TEST(Combinators, FailedSingleDigitTest) {
  SingleDigitParser test;
  ParseStatus result = test.parse("a"); 

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "a");
  EXPECT_EQ(result.parsedCharacters, "");
}

// Simple AND combinator test
TEST(Combinators, DigitAndCharCombinator) {
  SingleDigitParser digitParser;
  SingleCharParser charParser;

  AndCombinator aC;
  aC.firstParser = reinterpret_cast<NullParser *>(&digitParser);
  aC.secondParser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result = aC.parse("1o1");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "1");
  EXPECT_EQ(result.parsedCharacters, "1o");
}

// Simple OR combinator test
TEST(Combinators, DigitOrCharCombinator) {
  SingleDigitParser digitParser;
  SingleCharParser charParser;

  OrCombinator orC;
  orC.firstParser = reinterpret_cast<NullParser *>(&digitParser);
  orC.secondParser = reinterpret_cast<NullParser *>(&charParser);

  ParseStatus result = orC.parse("a0");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "0");
  EXPECT_EQ(result.parsedCharacters, "a");
}

// add_op test
TEST(Combinators, AddOp) {
  AddSubOpParser test;

  ParseStatus result = test.parse("+");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "+");
}

// // add_op fail test
// TEST(Combinators, FailAddOp) {

//   ParseStatus result;
//   result.status = false;
//   result.remainingCharacters = "*";
//   AddSubOpParser test;

//   EXPECT_EQ(test.parse("*"), result);
// }

// // sub_op test
// TEST(Combinators, SubOp) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "";
//   AddSubOpParser test;

//   EXPECT_EQ(test.parse("-"), result);
// }

// // sub_op fail test
// TEST(Combinators, FailSubOp) {

//   ParseStatus result;
//   result.status = false;
//   result.remainingCharacters = "/";
//   AddSubOpParser test;

//   EXPECT_EQ(test.parse("/"), result);
// }

// // mul_op test
// TEST(Combinators, MulOp) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "";
//   MulDivOpParser test;

//   EXPECT_EQ(test.parse("*"), result);
// }

// // mul_op fail test
// TEST(Combinators, FailMulOp) {

//   ParseStatus result;
//   result.status = false;
//   result.remainingCharacters = "-";
//   MulDivOpParser test;

//   EXPECT_EQ(test.parse("-"), result);
// }

// // div_op test
// TEST(Combinators, DivOp) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "";
//   MulDivOpParser test;

//   EXPECT_EQ(test.parse("/"), result);
// }

// // mul_op fail test
// TEST(Combinators, FailDivOp) {

//   ParseStatus result;
//   result.status = false;
//   result.remainingCharacters = "+";
//   MulDivOpParser test;

//   EXPECT_EQ(test.parse("+"), result);
// }

// // Zero or more combinator test
// TEST(Combinators, ZeroOrMoreCombinator) {
//   SingleDigitParser digitParser;
//   ZeroOrMoreCombinator zeroOrMore;

//   zeroOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "ab";

//   EXPECT_EQ(zeroOrMore.parse("123ab"), result);
// }

// // Passing in nothing for Zero or more combinator test
// TEST(Combinators, NoMatchZeroOrMoreCombinator) {
//   SingleDigitParser digitParser;
//   ZeroOrMoreCombinator zeroOrMore;

//   zeroOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "ab";

//   EXPECT_EQ(zeroOrMore.parse("ab"), result);
// }


// // One or more combinator test
// TEST(Combinators, OneOrMoreCombinator) {
//   SingleCharParser charParser;
//   OneOrMoreCombinator oneOrMore;

//   oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);

//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "1cd";

//   EXPECT_EQ(oneOrMore.parse("ab1cd"), result);
// }

// // Fail Case for One or more combinator test
// TEST(Combinators, FailOneOrMoreCombinator) {
//   SingleDigitParser digitParser;
//   OneOrMoreCombinator oneOrMore;

//   oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);

//   ParseStatus result;
//   result.status = false;
//   result.remainingCharacters = "ab1cd";

//   EXPECT_EQ(oneOrMore.parse("ab1cd"), result);
// }


// // open_paren test
// TEST(Combinators, OpenParen) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "abc";
//   OpenParenParser test;

//   EXPECT_EQ(test.parse("(abc"), result);
// }


// // close_paren test
// TEST(Combinators, CloseParen) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "abc";
//   CloseParenParser test;

//   EXPECT_EQ(test.parse(")abc"), result);
// }

TEST(Combinators, FailedDigit) {
  NumParser test;
  ParseStatus result = test.parse("fa10abasdf");

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "fa10abasdf");
}

TEST(Combinators, MultiDigitNumber) {
  NumParser test;
  ParseStatus result = test.parse("101281510abasdf");

  // Traversing the AST created from the number
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "abasdf");
  EXPECT_EQ(output, "101281510");
}
// /*
// // TEST(Combinators, AE) {
// //   ParseStatus result;
// //   result.status = true;
// //   result.remainingCharacters = "";
// //   AEParser test;

// //   EXPECT_EQ(test.parse("23"), result);
// // }
// /*
// TEST(Combinators, AE) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "";
//   ArithExprParser test;

//   EXPECT_EQ(test.parse("2+3"), result);
// }
// */
