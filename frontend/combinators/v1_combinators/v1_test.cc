#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v1.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
// #include "frontend/combinators/basic_combinators/parsestatus.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include "frontend/combinators/v1_combinators/mul_div_op.h"
// #include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
// #include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
// #include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/close_paren.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/open_paren.h"

#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"
#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/v1_combinators/negative_sign.h"


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
TEST(Combinators, FailAddOp) {
  AddSubOpParser test;
  ParseStatus result = test.parse("*");

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "*");
  EXPECT_EQ(result.parsedCharacters, "");
}

// sub_op test
TEST(Combinators, SubOp) {
  AddSubOpParser test;
  ParseStatus result = test.parse("-");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "-");
}

// // sub_op fail test
TEST(Combinators, FailSubOp) {
  AddSubOpParser test;
  ParseStatus result = test.parse("/");

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "/");
  EXPECT_EQ(result.parsedCharacters, "");
}

// // mul_op test
TEST(Combinators, MulOp) {
  MulDivOpParser test;
  ParseStatus result = test.parse("*");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "*");
}

// // mul_op fail test
TEST(Combinators, FailMulOp) {
  MulDivOpParser test;
  ParseStatus result = test.parse("0");

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "0");
  EXPECT_EQ(result.parsedCharacters, "");
}

// // div_op test
TEST(Combinators, DivOp) {
  MulDivOpParser test;
  ParseStatus result = test.parse("/");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "/");
}

// // mul_op fail test
TEST(Combinators, FailDivOp) {
  MulDivOpParser test;
  ParseStatus result = test.parse("0");

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.remainingCharacters, "0");
  EXPECT_EQ(result.parsedCharacters, "");
}

// open_paren test
TEST(Combinators, OpenParen) {
  OpenParenParser test;
  ParseStatus result = test.parse("(abc");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "abc");
}

// close_paren test
TEST(Combinators, CloseParen) {
  CloseParenParser test;
  ParseStatus result = test.parse(")abc");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "abc");
}

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

TEST(Combinators, SimpleNumTerm) {
  TermExprParser test;
  ParseStatus result = test.parse("101281510abasdf");

  // Traversing the AST created from the number
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "abasdf");
  EXPECT_EQ(output, "101281510");
}


TEST(Combinators, SimplyAddition) {
  AddSubExprParser test;
  ParseStatus result = test.parse("5+77");

  // Traversing the AST created from the number
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(+ 5 77)");
}

TEST(Combinators, SimpleMul) {
  MulDivExprParser test;
  ParseStatus result = test.parse("225*335");

  // Traversing the AST created from the number
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(* 225 335)");
}


// TODO: FIX THESE
TEST(Combinators, SimpleParen) {
  MulDivExprParser test;
  ParseStatus result = test.parse("(225*335)");

  // Traversing the AST created from the number
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(* 225 335)");
}

// TEST(Combinators, NegativeNumber) {
//   TermExprParser test;
//   ParseStatus result = test.parse("-101281510abasdf");

//   // Traversing the AST created from the number
//   PrintVisitor *a = new PrintVisitor();
//   result.ast->Visit(a);
//   std::string output = a->GetOutput();

//   EXPECT_EQ(result.status, true);
//   EXPECT_EQ(result.remainingCharacters, "abasdf");
//   EXPECT_EQ(output, "0-101281510");
// }

//   EXPECT_EQ(test.parse("23"), result);
// }
// /*
// TEST(Combinators, AE) {
//   ParseStatus result;
//   result.status = true;
//   result.remainingCharacters = "";
//   ArithExprParser test;

//   EXPECT_EQ(test.parse("2+3"), result);
// }
// */
