#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v2_combinators/main/program_parser.h"
#include "gtest/gtest.h"

using namespace cs160::frontend;

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser1) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "11fab";

  ParseStatus testResult = wordParser.parse("11fab");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser2) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "()qam";

  ParseStatus testResult = wordParser.parse("()qam");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser4) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "&&qam";

  ParseStatus testResult = wordParser.parse("&&qam");

  EXPECT_EQ(testResult, result);
}
// Fail Case WordParser
TEST(WordParserCombinator, failWordParser5) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "%%qam";

  ParseStatus testResult = wordParser.parse("%%qam");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser6) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "123victor";

  ParseStatus testResult = wordParser.parse("123victor");

  EXPECT_EQ(testResult, result);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser1) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var";

  ParseStatus testResult = wordParser.parse("var");

  EXPECT_EQ(testResult.remainingCharacters, result.remainingCharacters);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser2) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var;35345345";

  ParseStatus testResult = wordParser.parse("var;35345345");

  EXPECT_EQ(testResult.status, result.status);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser3) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "_victor";

  ParseStatus testResult = wordParser.parse("_victor");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "_victor");
}

// Success Case trim in utility
TEST(WordParserCombinator, trimTest) {
  std::string result = "    var    ";
  cs160::trim(result);
  EXPECT_EQ(result, "var");
}

// Success Case var_keyword_parser
TEST(WordParserCombinator, successVarKeywordParser1) {
  VarKeywordParser varParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var";

  ParseStatus testResult = varParser.parse("   var");

  EXPECT_EQ(testResult, result);
}

// Fail Case var_keyword_parser
TEST(WordParserCombinator, failVarKeywordParser1) {
  VarKeywordParser varParser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = varParser.parse("victor");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Start variable declaration with var");
}

// Success Case ColonParser
TEST(WordParserCombinator, sucessColonParser1) {
  ColonParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "  joj";
  result.parsedCharacters = ":";

  ParseStatus testResult = parser.parse("   :  joj");

  EXPECT_EQ(testResult, result);
}

// Fail Case ColonParser
TEST(WordParserCombinator, failColonParser1) {
  ColonParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("first char not colon:");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Missing colon");
}

// Success Case TypeParser
TEST(WordParserCombinator, successTypeParser1) {
  TypeParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "Integer";

  ParseStatus testResult = parser.parse("   Integer");

  EXPECT_EQ(testResult, result);
}

// Fail Case TypeParser
TEST(WordParserCombinator, failTypeParser1) {
  TypeParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("  _abc  "); // type can only contain a through z

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Incorrect type in variable declaration");
}

// Success Case EqualSignParser
TEST(WordParserCombinator, sucessEqualSignParser1) {
  EqualSignParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "  3 + 4";
  result.parsedCharacters = "=";

  ParseStatus testResult = parser.parse("   =  3 + 4");

  EXPECT_EQ(testResult, result);
}

// Fail Case EqualSignParser
TEST(WordParserCombinator, failEqualSignParser1) {
  EqualSignParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse(" _=");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Missing equal sign");
}

// Success Case VariableParser
TEST(VariableParserCombinator, successVariableParser1) {
  VariableParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var _victor : Integer;";

  ParseStatus testResult = parser.parse(" var _victor : Integer;");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "_victor");
}

// Success Case VariableParser
TEST(VariableParserCombinator, successVariableParser2) {
  VariableParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var _victor : Integer;";

  ParseStatus testResult = parser.parse(" var _victor : Integer;");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "_victor");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser1) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("_victor:Integer;");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Start variable declaration with var");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser2) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("var _victor : ;");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Incorrect type in variable declaration");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser3) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("Var _victor Integer");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Start variable declaration with var");
}

// Fail Case VariableParser
TEST(VariableParserCombinator, failVariableParser4) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("var 1victor : Integer");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case VariableParser : colon missing
TEST(VariableParserCombinator, failVariableParser5) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse(" var _victor Integer");

  EXPECT_EQ(testResult, result);
  //TODO FIX
  //EXPECT_EQ(testResult.errorType, )
}

// Fail Case Missing semicolon
TEST(WordParserCombinator, failVariableParser6) {
  VariableParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse(" var _victor : Integer");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Missing semicolon");
}


// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser1) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var _victor : Integer = 490;";

  ParseStatus testResult = parser.parse("var _victor : Integer = 490;");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "_victor = 490");
}

// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser2) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "victor = 490;";

  ParseStatus testResult = parser.parse(" victor = 490;");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "victor = 490");
}

// Success Case VariableParser
TEST(AssignmentParserCombinator, successAssignmentParser3) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "victor = (123*1+3901-2);";

  ParseStatus testResult = parser.parse(" victor = (123 * 1 + 3901 - 2);");

    // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "victor = (((123 * 1) + 3901) - 2)");
}

// Fail Case VariableParser
TEST(AssignmentParserCombinator, failAssignmentParser1) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("_victor = ;");

  EXPECT_EQ(testResult, result);
}

// Fail Case VariableParser
TEST(AssignmentParserCombinator, failAssignmentParser2) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("1victor = (123 * 1 + 3901 - 2);");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Declare variable names with 'var variable_name : type = expression'");
}

// Fail Case VariableParser : missing equal sign
TEST(AssignmentParserCombinator, failAssignmentParser3) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("victor");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Missing equal sign");
}

// Fail Case VariableParser : missing equal sign
TEST(AssignmentParserCombinator, failAssignmentParser4) {
  AssignmentParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("138 + 3; victor");

  EXPECT_EQ(testResult, result);
}

TEST(BinaryOperatorExpr, successBinaryOperatorExpr1) {
  ArithExprParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "victor+490;";

  ParseStatus testResult = parser.parse(" victor + 490;");

  // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(output, "(victor + 490)");
}

/*
// Success Case VariableParser
TEST(AssignmentParserCombinator, successProgramParser1) {
  ProgramParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "victor = (123*1+3901-2);j=4;j+victor";

  ParseStatus testResult = parser.parse(" victor = (123 * 1 + 3901 - 2); j = 4; 3 + 4;");

    // Traversing the AST created from the variable name
  PrintVisitor *a = new PrintVisitor();
  testResult.ast->Visit(a);
  std::string output = a->GetOutput();

  //EXPECT_EQ(testResult, result);
    EXPECT_EQ(testResult.status, result.status);
  EXPECT_EQ(testResult.remainingCharacters, result.remainingCharacters);
  EXPECT_EQ(testResult.parsedCharacters, result.parsedCharacters);
  EXPECT_EQ(output, "victor = (((123 * 1) + 3901) - 2);j=4;j+victor");
}*/
