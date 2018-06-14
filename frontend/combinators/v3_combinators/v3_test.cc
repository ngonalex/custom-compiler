#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v3.h"
// #include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace cs160::frontend;

// Success Case >=
TEST(RelationlHelper, gtoet) {
  GreaterThanOrEqualToOpParser test;
  ParseStatus result = test.do_parse(">=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">=");
}

// // Success Case <=
TEST(RelationlHelper, ltoet) {
  LessThanOrEqualToOpParser test;
  ParseStatus result = test.do_parse("<=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "<=");
}

// // Success Case ==
TEST(RelationlHelper, eqto) {
  EqualToOpParser test;
  ParseStatus result = test.do_parse("==", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "==");
}

// // Success Case >
TEST(RelationlHelper, gt) {
  GreaterThanOpParser test;
  ParseStatus result = test.do_parse(">", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">");
}

// // Success Case <
TEST(RelationlHelper, lt) {
  LessThanOpParser test;
  ParseStatus result = test.do_parse("<", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "<");
}

// // Success Case &&
TEST(RelationlHelper, andP) {
  AndOpParser test;
  ParseStatus result = test.do_parse("&&", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "&&");
}

// // Success Case ||
TEST(RelationlHelper, orP) {
  OrOpParser test;
  ParseStatus result = test.do_parse("||", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "||");
}

// // Success Case !
TEST(RelationlHelper, notP) {
  NotOpParser test;
  ParseStatus result = test.do_parse("!", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "!");
}

TEST(RelationHelper, allParser1) {
  RelationOperatorParser test;
  ParseStatus result = test.do_parse(">=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">=");
}

TEST(RelationHelper, OpenBrackets) {
  OpenCurlyBrackets test;
  ParseStatus result = test.do_parse("{", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "{");
}

TEST(RelationHelper, FailOpenBrackets) {
  OpenCurlyBrackets test;
  ParseStatus result = test.do_parse("j", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
}

TEST(RelationHelper, CloseBrackets) {
  CloseCurlyBrackets test;
  ParseStatus result = test.do_parse("}", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "}");
}

TEST(RelationHelper, FailCloseBrackets) {
  CloseCurlyBrackets test;
  ParseStatus result = test.do_parse("j", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
}

TEST(RelationHelper, RepeatKeyword) {
  RepeatKeyword test;
  ParseStatus result = test.do_parse("repeat", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 6);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "repeat");
}

TEST(RelationHelper, FailRepeatKeyword) {
  RepeatKeyword test;
  ParseStatus result = test.do_parse("not repeat", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
}

TEST(RelationHelper, WhileKeyword) {
  WhileKeyword test;
  ParseStatus result = test.do_parse("while", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 5);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "while");
}


TEST(RelationHelper, FailWhileKeyword) {
  WhileKeyword test;
  ParseStatus result = test.do_parse("not while", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
}

TEST(FailedRelationHelper, allParser1) {
  LogicOperatorParser test;
  ParseStatus result = test.do_parse("==", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.remainingCharacters, "==");
  EXPECT_EQ(result.parsedCharacters, "");
}

TEST(RelationHelper, logicParser1) {
  LogicOperatorParser test;
  ParseStatus result = test.do_parse("&&", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "&&");
}

TEST(RelationHelper, logicParser2) {
  LogicOperatorParser test;
  ParseStatus result = test.do_parse("||", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "||");
}

TEST(RelationBody, simpleRBody1) {
  RelationBodyParser parser;
  ParseStatus result = parser.do_parse("x == 3", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 6);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(x == 3)");
}

TEST(RelationBody, simpleRBody2) {
  RelationBodyParser parser;
  ParseStatus result = parser.do_parse("-9+3 >= 222", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 11);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "((0 - (9 + 3)) >= 222)");
}

TEST(RelationBody, simpleRBody3) {
  RelationBodyParser parser;
  ParseStatus result = parser.do_parse("-912981*128158+1919+(99-22) > x", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 31);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "((0 - (((912981 * 128158) + 1919) + (99 - 22))) > x)");
}

TEST(RelationParser, simpleRelationParser1) {
  RelationParser parser;
  ParseStatus result = parser.do_parse("x > y", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 5);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(x > y)");
}

TEST(RelationParser, simpleRelationParser2) {
  RelationParser parser;
  ParseStatus result = parser.do_parse("!x > y", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 6);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "! ((x > y))");
}

TEST(LogicParser, simpleLogicParser1) {
  RelationParser parser;
  ParseStatus result = parser.do_parse("!x > y && x >= z", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 15);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "! (((x > y) && (x >= z)))");
}

TEST(LogicParser, simpleLogicParser2) {
  RelationParser parser;
  ParseStatus result = parser.do_parse("x > y || x == 2 && 2 >= 0", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "((x > y) || ((x == 2) && (2 >= 0)))");
}

TEST(KeywordParser, IfKeyword) {
  IfKeyword parser;
  ParseStatus result = parser.do_parse("if (this == that)", 0);
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, " (this == that)");
  EXPECT_EQ(result.parsedCharacters, "if");
}

TEST(KeywordParser, ElseKeyword) {
  ElseKeyword parser;
  ParseStatus result = parser.do_parse("else (this == that)", 0);
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, " (this == that)");
  EXPECT_EQ(result.parsedCharacters, "else");
}