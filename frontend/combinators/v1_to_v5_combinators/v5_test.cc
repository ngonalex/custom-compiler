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

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 9);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "z[5]=x;");
  EXPECT_EQ(output, "z[5] = x");
}

TEST(AssignmentParser, assignToArray2) {
  AssignmentParser parser;
  ParseStatus result = parser.do_parse("a[ z + 6 + ( 4 + 5 ) ] = x;", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 27);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "a[z+6+(4+5)]=x;");
  EXPECT_EQ(output, "a[((z + 6) + (4 + 5))] = x");
}

TEST(TupleCreationParser, createTuple) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5];", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 15);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5];");
  EXPECT_EQ(output, "z[5]");
}

TEST(TupleCreationParser, createTuple2) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5 + 7 + z];", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5+7+z];");
  EXPECT_EQ(output, "z[((5 + 7) + z)]");
}

TEST(TupleCreationParser, createTuple3) {
  TupleAssignmentParser parser;
  ParseStatus result = parser.do_parse("var z : int[5 * 2 + z];", 0);

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:int[5*2+z];");
  EXPECT_EQ(output, "z[((5 * 2) + z)]");
}

TEST(DereferenceParser, SucessDereference1) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("x[3]", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "x[3]");

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "x[3]");
}

TEST(DereferenceParser, SucessDereference2) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("var[2515]", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 9);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var[2515]");

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "var[2515]");
}

TEST(DereferenceParser, SucessDereference3) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("var[((3 + 5) + x)]", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 18);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var[((3+5)+x)]");

  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "var[((3 + 5) + x)]");
}

TEST(DereferenceParser, FailedDereference1) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("[2515]", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.remainingCharacters, "[2515]");
  EXPECT_EQ(result.parsedCharacters, "");
  EXPECT_EQ(result.errorType, "Declare variable names with 'var variable_name : type = expression'");
}

TEST(DereferenceParser, FailedDereference2) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("var2515]", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.parsedCharacters, "");
  EXPECT_EQ(result.errorType, "Expecting character: [");
}

TEST(DereferenceParser, FailedDereference3) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("var[2515", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.parsedCharacters, "");
  EXPECT_EQ(result.errorType, "Expected ']'");
}

TEST(DereferenceParser, FailedDereference4) {
  DereferenceParser parser;
  ParseStatus result = parser.do_parse("var[]", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.parsedCharacters, "");
  EXPECT_EQ(result.errorType, "Missing arithmetic expression");
}