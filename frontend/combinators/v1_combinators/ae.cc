#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/semicolon_parser.h"

#define super NullParser

using namespace cs160::frontend;

/*
	ae -> num_parser  ||  ae add_sub_op ae  || open_paren ae add_sub_op ae
   close_paren
		    ||  ae mul_div_op ae  || open_paren ae mul_div_op ae
   close_paren
*/

ParseStatus ArithExprParser::parse(std::string inputProgram, int startCharacter,
				   std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter);
  }
  AddSubExprParser ae;
  SemiColonParser semiColonP;
  ParseStatus aeParseResult = ae.parse(inputProgram, endCharacter);
  aeParseResult.startCharacter = startCharacter;
  if (aeParseResult.status) {
    ParseStatus semiColonResult =
	semiColonP.parse(aeParseResult.remainingCharacters, aeParseResult.endCharacter);
    aeParseResult.status = semiColonResult.status;
    if (semiColonResult.status) {
      aeParseResult.remainingCharacters = semiColonResult.remainingCharacters;
      aeParseResult.parsedCharacters += semiColonResult.parsedCharacters;
      aeParseResult.endCharacter = semiColonResult.endCharacter;
    } else {
      aeParseResult.errorType = semiColonResult.errorType;
    }
  }
  return aeParseResult;
}
