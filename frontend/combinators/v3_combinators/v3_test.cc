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
  result.status = false;
  result.remainingCharacters = ">=";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case <=
TEST(RelationlHelper, gtolt) {
  LessThanOrEqualToOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "<=";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case ==
TEST(RelationlHelper, eqto) {
  EqualToOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "==";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case >
TEST(RelationlHelper, gt) {
  GreaterThanOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = ">";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case <
TEST(RelationlHelper, lt) {
  LessThanOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "<";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}