#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
// #include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
// #include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "gtest/gtest.h"

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
  RelationOperator test;
  ParseStatus result = test.parse(">=", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ">=");
}

// TEST(RelationExpression, greaterThan) {
//   RelationParser parser;
//   ParseStatus result = parser.parse("x == 3");

//   EXPECT_EQ(result.status, true);
//   EXPECT_EQ(result.remainingCharacters, "");
//   EXPECT_EQ(result.parsedCharacters, "x == 3");
// }