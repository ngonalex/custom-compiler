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

// Success Case &&
TEST(RelationlHelper, andP) {
  AndOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "&&";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case ||
TEST(RelationlHelper, orP) {
  OrOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "||";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

// Success Case !
TEST(RelationlHelper, notP) {
  NotOpParser opParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "!";

  ParseStatus testResult = opParser.parse("");

  EXPECT_EQ(testResult, result);
}

TEST(RelationHelper, allParser) {



    EqualToOpParser equalTo;
    GreaterThanOpParser greaterThan;
    LessThanOpParser lessThan;
    GreaterThanOrEqualToOpParser greaterOrEqual;
    LessThanOrEqualToOpParser lessOrEqual;
      // > or <
    OrCombinator greaterOrLess;
    greaterOrLess.firstParser = reinterpret_cast<NullParser *>(&greaterThan);
    greaterOrLess.secondParser = reinterpret_cast<NullParser *>(&lessThan);
    // >= or <=
    OrCombinator orEqualTo;
    orEqualTo.firstParser = reinterpret_cast<NullParser *>(&greaterOrEqual);
    orEqualTo.secondParser = reinterpret_cast<NullParser *>(&lessOrEqual);
    // > or < or >= or <=
    OrCombinator both;
    both.firstParser = reinterpret_cast<NullParser *>(&orEqualTo);
    both.secondParser = reinterpret_cast<NullParser *>(&greaterOrLess);
    // == or > or < or >= or <=
    OrCombinator all;
    all.firstParser = reinterpret_cast<NullParser *>(&equalTo);
    all.secondParser = reinterpret_cast<NullParser *>(&both);

    ParseStatus result;
    result.status = false;
    result.remainingCharacters = "!";
}