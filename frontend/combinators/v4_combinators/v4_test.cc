#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v4.h"
#include "frontend/combinators/v4_combinators/helpers/function_helpers.h"
#include "frontend/combinators/v4_combinators/main/function_call_parser.h"
#include "frontend/combinators/v4_combinators/main/function_decl_parser.h"

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
  EXPECT_EQ(result.errorType, "Expected ','");
}

/*

Continue with funciton_helpers.cc ...

*/


TEST(FunctionDeclParser, successFunction) {
  FunctionDeclParser parser;
  ParseStatus result = parser.do_parse("", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 45);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "varz:Integer=0;if(z==0){z=100;}z;");
  EXPECT_EQ(result.errorType, "");
/*
  PrintVisitor *a = new PrintVisitor();
  result.ast->Visit(a);
  std::string output = a->GetOutput();
  EXPECT_EQ(output, "z = 0; if (z == 0) {z = 100} else {}; z");*/
}


