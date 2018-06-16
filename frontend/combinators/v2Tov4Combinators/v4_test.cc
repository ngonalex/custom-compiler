#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v4.h"
#include "frontend/combinators/v2Tov4Combinators/helpers/function_helpers.h"
#include "frontend/combinators/v2Tov4Combinators/main/function_call_parser.h"
#include "frontend/combinators/v2Tov4Combinators/main/function_decl_parser.h"

#include "gtest/gtest.h"

#include <iostream>

using namespace cs160::frontend;


TEST(FunctionHelpers, successComma) {
  CommaOp parser;
  ParseStatus result = parser.do_parse(",", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, ",");
}


TEST(FunctionHelpers, failComma) {
  CommaOp parser;
  ParseStatus result = parser.do_parse("}", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expected character: ,");
}

TEST(FunctionHelpers, SuccessfuncKeyword) {
  FuncKeyword parser;
  ParseStatus result = parser.do_parse("func", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 4);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "func");
}


TEST(FunctionHelpers, successFuncKeyword) {
  FuncKeyword parser;
  ParseStatus result = parser.do_parse("not func", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  //EXPECT_EQ(result.errorType, "Expected func operator");
}

TEST(FunctionHelpers, successReturnType) {
  ReturnTypeOp parser;
  ParseStatus result = parser.do_parse("->", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "->");
}


TEST(FunctionHelpers, failReturnType) {
  CommaOp parser;
  ParseStatus result = parser.do_parse("not ->", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expected '->'");
}

TEST(FunctionHelpers, successFunctionVariable) {
  FunctionVariableParsers parser;
  ParseStatus result = parser.do_parse("victor : Integer, prabal : String", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 20);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor:Integer,prabal:String");
}


TEST(FunctionHelpers, failFunctionVariable) {
  FunctionVariableParsers parser;
  ParseStatus result = parser.do_parse("victor : Integer, prabal ", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expected function variable declaration");
}

TEST(FunctionHelpers, successFunctionArguments) {
  FunctionArgumentParsers parser;
  ParseStatus result = parser.do_parse("33 + 2, victor", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 20);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "33+2,victor");
}


TEST(FunctionHelpers, failFunctionArguments) {
  FunctionArgumentParsers parser;
  ParseStatus result = parser.do_parse("joj, victor,", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "Expected function variable declaration");
}




TEST(FunctionDeclParser, successFunctionDec1) {
  FunctionDeclParser parser;
  ParseStatus result = parser.do_parse("func victor(prabal :Integer, Jason : String) -> String {if(x == 2){e = 2;} else {f = 3;}}", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 45);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "funcvictor(prabal:Integer,Jason:String)->String{if(x==2){e=2;}else{f=3;}}");
  EXPECT_EQ(result.errorType, "");
/*
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "z = 0; if (z == 0) {z = 100} else {}; z");*/
}

TEST(FunctionDeclParser, successFunctionDec2) {
  FunctionDeclParser parser;
  ParseStatus result = parser.do_parse("func victor() -> String {if(x == 2){e = 2;} else {f = 3;}}", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 45);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "funcvictor()->String{if(x==2){e=2;}else{f=3;}}");
  EXPECT_EQ(result.errorType, "");
/*
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "z = 0; if (z == 0) {z = 100} else {}; z");*/
}


TEST(FunctionDeclParser, failFunctionDec1) {
  FunctionDeclParser parser;
  ParseStatus result = parser.do_parse("func victor(prabal : Integer) {x + 2}", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "");
}

TEST(FunctionDeclParser, failFunctionDec2) {
  FunctionDeclParser parser;
  ParseStatus result = parser.do_parse("func victor(pra) {x + 2}", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "");
}



TEST(FunctionDeclParser, successFunctionCall1) {
  FunctionCallParser parser;
  ParseStatus result = parser.do_parse("victor(2+3*(2)+victor, jason)", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 45);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor(2+3*(2)+victor,jason)");
  EXPECT_EQ(result.errorType, "");
/*
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "z = 0; if (z == 0) {z = 100} else {}; z");*/
}

TEST(FunctionDeclParser, successFunctionCall2) {
  FunctionCallParser parser;
  ParseStatus result = parser.do_parse("victor()", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 45);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "victor");
  EXPECT_EQ(result.errorType, "");
/*
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "z = 0; if (z == 0) {z = 100} else {}; z");*/
}


TEST(FunctionDeclParser, failFunctionCall1) {
  FunctionCallParser parser;
  ParseStatus result = parser.do_parse("jason(if victor)", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "");
}

TEST(FunctionDeclParser, failFunctionCall2) {
  FunctionCallParser parser;
  ParseStatus result = parser.do_parse("word(yo, 3", 0);

  EXPECT_EQ(result.status, false);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 0);
  EXPECT_EQ(result.errorType, "");
}



