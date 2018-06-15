#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v3.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v2_combinators/main/program_parser.h"
#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "gtest/gtest.h"

using namespace cs160::frontend;

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser1) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("11fab", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser2) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("()qam", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser4) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("&&qam", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser5) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("%%qam", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser6) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("123victor", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser1) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("var", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 3);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var");
  EXPECT_EQ(output, "var");
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser2) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("var;35345345", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 3);
  EXPECT_EQ(result.remainingCharacters, ";35345345");
  EXPECT_EQ(result.parsedCharacters, "var");
  EXPECT_EQ(output, "var");
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser3) {
  WordParser wordParser;
  ParseStatus result = wordParser.parse("_victor", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 7);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "_victor");
  EXPECT_EQ(output, "_victor");
}

// Success Case trim in utility
TEST(WordParserCombinator, trimTest) {
  std::string result = "    var ";
  cs160::trim(result);
  EXPECT_EQ(result, "var ");
}

// Success Case var_keyword_parser
TEST(WordParserCombinator, successVarKeywordParser1) {
  VarKeywordParser test;
  ParseStatus result = test.parse(" var", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var");
}

// Fail Case var_keyword_parser
TEST(WordParserCombinator, failVarKeywordParser1) {
  VarKeywordParser test;
  ParseStatus result = test.parse("victor", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Start variable declaration with var");
}

// Success Case ColonParser
TEST(WordParserCombinator, sucessColonParser1) {
  ColonParser test;
  ParseStatus result = test.parse("   :  joj", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 3);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "  joj");
  EXPECT_EQ(result.parsedCharacters, ":");
}

// Fail Case ColonParser
TEST(WordParserCombinator, failColonParser1) {
  ColonParser test;
  ParseStatus result = test.parse("first char not colon:", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Missing colon");
}

// Success Case TypeParser
TEST(WordParserCombinator, successTypeParser1) {
  TypeParser test;
  ParseStatus result = test.parse("     Integer", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 5);
  EXPECT_EQ(result.endCharacter, 12);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "Integer");
}

// Fail Case TypeParser
TEST(WordParserCombinator, failTypeParser1) {
  TypeParser test;
  ParseStatus result = test.parse("  _abc  ", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 2);
  EXPECT_EQ(result.endCharacter, 2);
  EXPECT_EQ(result.errorType, "Incorrect type in variable declaration");
}

// Success Case EqualSignParser
TEST(WordParserCombinator, sucessEqualSignParser1) {
  EqualSignParser test;
  ParseStatus result = test.parse("   =  3 + 4", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 3);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "  3 + 4");
  EXPECT_EQ(result.parsedCharacters, "=");
}

// Fail Case EqualSignParser
TEST(WordParserCombinator, failEqualSignParser1) {
  EqualSignParser test;
  ParseStatus result = test.parse("_=", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Missing equal sign");
}

// Success Case VariableParser
TEST(VariableParserCombinator, successVariableParser1) {
  VariableParser test;
  ParseStatus result = test.parse(" var _victor : Integer;", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 23);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var_victor:Integer;");
  EXPECT_EQ(output, "_victor");
}

// Success Case VariableParser
TEST(VariableParserCombinator, successVariableParser2) {
  VariableParser test;
  ParseStatus result = test.parse(" var    _victor :   Integer;", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 28);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var_victor:Integer;");
  EXPECT_EQ(output, "_victor");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser1) {
  VariableParser test;
  ParseStatus result = test.parse("_victor:Integer;", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Start variable declaration with var");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser2) {
  VariableParser test;
  ParseStatus result = test.parse("var _victor : ;", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType, "Incorrect type in variable declaration");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser3) {
  VariableParser test;
  ParseStatus result = test.parse(" Var _victor Integer", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 11);
  EXPECT_EQ(result.endCharacter, 11);
  EXPECT_EQ(result.errorType, "Start variable declaration with var");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser4) {
  VariableParser test;
  ParseStatus result = test.parse("var 1victor : Integer", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(
      result.errorType,
      "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case VariableParser : colon missing
TEST(VariableParserCombinator, failVariableParser5) {
  VariableParser test;
  ParseStatus result = test.parse(" var _victor Integer", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 11);
  EXPECT_EQ(result.endCharacter, 11);
  EXPECT_EQ(result.errorType, "Missing colon");
}

// Failing using keyword
TEST(WordParserCombinator, failVariableParser6) {
  VariableParser test;
  ParseStatus result = test.parse(" var heap : Integer", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 11);
  EXPECT_EQ(result.endCharacter, 11);
  EXPECT_EQ(result.errorType, "");
}

// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser1) {
  AssignmentParser test;
  ParseStatus result = test.parse("var _victor : Integer = 490;", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 28);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "var_victor:Integer=490;");
  EXPECT_EQ(output, "_victor = 490");
}

// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser2) {
  AssignmentParser test;
  ParseStatus result = test.parse(" victor = 490;", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 14);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor=490;");
  EXPECT_EQ(output, "victor = 490");
}

// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser3) {
  AssignmentParser test;
  ParseStatus result = test.parse(" victor = (123 * 1 + 3901 - 2);", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 31);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor=(123*1+3901-2);");
  EXPECT_EQ(output, "victor = (((123 * 1) + 3901) - 2)");
}

// Fail Case VariableParser
TEST(AssignmentParserCombinator, failAssignmentParser1) {
  AssignmentParser test;
  ParseStatus result = test.parse("_victor = ;", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType,
            "Missing arithmetic expression");  // Start of arithmetic expression
}

// Fail Case VariableParser
TEST(AssignmentParserCombinator, failAssignmentParser2) {
  AssignmentParser test;
  ParseStatus result = test.parse("1victor = (123 * 1 + 3901 - 2);", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType,
            "Declare variable names with 'var variable_name : type = "
            "expression' or Start variable declaration with var");
}

// Fail Case VariableParser : missing equal sign
TEST(AssignmentParserCombinator, failAssignmentParser3) {
  AssignmentParser test;
  ParseStatus result = test.parse("victor", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType, "Missing equal sign");
}

// Fail Case VariableParser : missing equal sign
TEST(AssignmentParserCombinator, failAssignmentParser4) {
  AssignmentParser test;
  ParseStatus result = test.parse("138 + 3; victor", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType,
            "Declare variable names with 'var variable_name : type = "
            "expression' or Start variable declaration with var");
}

TEST(BinaryOperatorExpr, successBinaryOperatorExpr1) {
  ArithExprParser parser;

  ParseStatus result = parser.parse(" (victor + 490) - _foo + 3", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 26);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "(victor+490)-_foo+3");
  EXPECT_EQ(output, "(((victor + 490) - _foo) + 3)");
}

// Success Case ProgramParser
TEST(AssignmentParserCombinator, successProgramParser1) {
  ProgramParser test;
  ParseStatus result =
      test.parse(" victor = (123 * 1 + 3901 - 2); j = 4; j+victor;", 0);

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 1);
  EXPECT_EQ(result.endCharacter, 48);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor=(123*1+3901-2);j=4;j+victor;");
  EXPECT_EQ(output, "victor = (((123 * 1) + 3901) - 2); j = 4; (j + victor)");
}

// Fail Case ProgramParser
TEST(AssignmentParserCombinator, failProgramParser1) {
  ProgramParser test;
  ParseStatus result = test.parse("x = ; x + 2;", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType, "Expecting character: ;");
}

// Fail Case ProgramParser
TEST(AssignmentParserCombinator, failProgramParser2) {
  ProgramParser test;
  ParseStatus result = test.parse("x = 3; x + 2", 10);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 10);
  EXPECT_EQ(result.endCharacter, 10);
  EXPECT_EQ(result.errorType, "Expecting ;");
}
