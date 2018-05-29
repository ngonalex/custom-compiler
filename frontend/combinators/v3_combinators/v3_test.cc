#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "gtest/gtest.h"

using namespace cs160::frontend;

// Success Case >=
TEST(RelationlHelper, gtoet) {
  GreaterThanOrEqualToOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = ">=";

  ParseStatus testResult = opParser.parse(">=");

  EXPECT_EQ(testResult, result);
}

// Success Case <=
TEST(RelationlHelper, ltoet) {
  LessThanOrEqualToOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "<=";

  ParseStatus testResult = opParser.parse("<=");

  EXPECT_EQ(testResult, result);
}

// Success Case ==
TEST(RelationlHelper, eqto) {
  EqualToOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "==";

  ParseStatus testResult = opParser.parse("==");

  EXPECT_EQ(testResult, result);
}

// Success Case >
TEST(RelationlHelper, gt) {
  GreaterThanOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = ">";

  ParseStatus testResult = opParser.parse(">");

  EXPECT_EQ(testResult, result);
}

// Success Case <
TEST(RelationlHelper, lt) {
  LessThanOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "<";

  ParseStatus testResult = opParser.parse("<");

  EXPECT_EQ(testResult, result);
}

// Success Case &&
TEST(RelationlHelper, andP) {
  AndOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "&&";

  ParseStatus testResult = opParser.parse("&&");

  EXPECT_EQ(testResult, result);
}

// Success Case ||
TEST(RelationlHelper, orP) {
  OrOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "||";

  ParseStatus testResult = opParser.parse("||");

  EXPECT_EQ(testResult, result);
}

// Success Case !
TEST(RelationlHelper, notP) {
  NotOpParser opParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "!";

  ParseStatus testResult = opParser.parse("!");

  EXPECT_EQ(testResult, result);
}

TEST(RelationHelper, allParser) {
  RelationOperator parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = ">=";

  ParseStatus testResult = parser.parse(">=");

  EXPECT_EQ(testResult, result);
}