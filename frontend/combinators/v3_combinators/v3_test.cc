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
  ParseStatus result = test.parse(">=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">=");
}

// // Success Case <=
TEST(RelationlHelper, ltoet) {
  LessThanOrEqualToOpParser test;
  ParseStatus result = test.parse("<=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "<=");
}

// // Success Case ==
TEST(RelationlHelper, eqto) {
  EqualToOpParser test;
  ParseStatus result = test.parse("==", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "==");
}

// // Success Case >
TEST(RelationlHelper, gt) {
  GreaterThanOpParser test;
  ParseStatus result = test.parse(">", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">");
}

// // Success Case <
TEST(RelationlHelper, lt) {
  LessThanOpParser test;
  ParseStatus result = test.parse("<", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "<");
}

// // Success Case &&
TEST(RelationlHelper, andP) {
  AndOpParser test;
  ParseStatus result = test.parse("&&", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "&&");
}

// // Success Case ||
TEST(RelationlHelper, orP) {
  OrOpParser test;
  ParseStatus result = test.parse("||", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "||");
}

// // Success Case !
TEST(RelationlHelper, notP) {
  NotOpParser test;
  ParseStatus result = test.parse("!", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "!");
}

TEST(RelationHelper, allParser) {
  RelationOperatorParser test;
  ParseStatus result = test.parse(">=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">=");
}

TEST(RelationBody, simpleRBody1) {
  RelationBodyParser parser;
  ParseStatus result = parser.parse("x == 3", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 5);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(x == 3)");
}

TEST(RelationBody, simpleRBody2) {
  RelationBodyParser parser;
  ParseStatus result = parser.parse("-9+3 >= 222", 0);

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
  ParseStatus result = parser.parse("-912981*128158+1919+(99-22) > x", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 30);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "((0 - (((912981 * 128158) + 1919) + (99 - 22))) > x)");
}

TEST(RelationParser, simpleRelationParser1) {
  RelationParser parser;
  ParseStatus result = parser.parse("x > y", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "(x > y)");
}

TEST(RelationParser, simpleRelationParser2) {
  RelationParser parser;
  ParseStatus result = parser.parse("!x > y", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 5);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "! ((x > y))");
}

TEST(LogicParser, simpleLogicParser1) {
  RelationParser parser;
  ParseStatus result = parser.parse("!x > y && x >= z", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 12);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "! (((x > y) && (x >= z)))");
}

TEST(LogicParser, simpleLogicParser2) {
  RelationParser parser;
  ParseStatus result = parser.parse("x > y || x == 2 && 2 >= 0", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  
  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.endCharacter, 20);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(output, "((x > y) || ((x == 2) && (2 >= 0)))");
}