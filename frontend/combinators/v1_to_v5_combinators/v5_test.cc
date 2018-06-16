#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v5.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/tuple_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/main/dereference_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/assignment_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/tuple_creation_parser.h"
#include "gtest/gtest.h"

#include <iostream>

using namespace cs160::frontend;

TEST(TupleHelpers, SucessLeftBracket) {
  LeftBracketOp parser;
  ParseStatus result = parser.do_parse("[", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "[");
}

TEST(TupleHelpers, FailLeftBracket) {
  LeftBracketOp parser;
  ParseStatus result = parser.do_parse("]", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expecting character: [");
}

TEST(FunctionHelpers, SucessRightBracket) {
  RightBracketOp parser;
  ParseStatus result = parser.do_parse("]", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "]");
}

TEST(FunctionHelpers, FailRightBracket) {
  RightBracketOp parser;
  ParseStatus result = parser.do_parse("[", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expecting character: ]");
}

TEST(AssignmentParser, assignToArray) {
  AssignmentParser parser;
  ParseStatus result = parser.do_parse("z[5] = x;", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 9);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "z[5]=x;");
}

TEST(AssignmentParser, assignToArray2) {
  AssignmentParser parser;
  ParseStatus result = parser.do_parse("a[ z + 6 + ( 4 + 5 ) ] = x;", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 27);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "a[z+6+(4+5)]=x;");
}

TEST(TupleCreationParser, createTuple) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5];", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 15);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5];");
}

TEST(TupleCreationParser, createTuple2) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5 + 7 + z];", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5+7+z];");
}

TEST(TupleCreationParser, createTuple3) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5 * 2 + z];", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5*2+z];");
}
