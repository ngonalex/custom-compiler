#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"

#include <iostream>

#define super NullParser

using namespace cs160::frontend;

/*
        ae -> num_parser  ||  ae add_sub_op ae  || open_paren ae add_sub_op ae
   close_paren
                    ||  ae mul_div_op ae  || open_paren ae mul_div_op ae
   close_paren
*/

ParseStatus ArithExprParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  AddSubExprParser ae;
  ParseStatus aeParseResult = ae.do_parse(inputProgram, endCharacter);

  if (!aeParseResult.status) {
    aeParseResult.errorType = "Missing arithmetic expression";
  }

  return aeParseResult;
}