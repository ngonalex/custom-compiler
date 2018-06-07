#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"

#include <iostream>

#define super NullParser

using namespace cs160::frontend;

/*
  ae -> num_parser  ||  ae add_sub_op ae  || open_paren ae add_sub_op ae
   close_paren
        ||  ae mul_div_op ae  || open_paren ae mul_div_op ae
   close_paren
*/

ParseStatus ArithExprParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }
  AddSubExprParser ae;
  SemiColonParser semiColonP;
  ParseStatus aeParseResult = ae.do_parse(inputProgram, endCharacter);
  aeParseResult.startCharacter = startCharacter;
  if (aeParseResult.status) {
    ParseStatus semiColonResult = semiColonP.do_parse(aeParseResult.remainingCharacters, aeParseResult.endCharacter);
    aeParseResult.status = semiColonResult.status;
    if (semiColonResult.status) {
      aeParseResult.remainingCharacters = semiColonResult.remainingCharacters;
      aeParseResult.parsedCharacters += semiColonResult.parsedCharacters;
      aeParseResult.startCharacter = endCharacter;
      aeParseResult.endCharacter = semiColonResult.endCharacter;
    } else {
      aeParseResult.errorType = semiColonResult.errorType;
    }
  }
  aeParseResult.errorType = "Missing arithmetic expression";

  return aeParseResult;
}