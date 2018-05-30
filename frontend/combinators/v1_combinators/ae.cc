#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"

#define super NullParser

using namespace cs160::frontend;

/*
	ae -> num_parser  ||  ae add_sub_op ae  || open_paren ae add_sub_op ae close_paren
                    ||  ae mul_div_op ae  || open_paren ae mul_div_op ae close_paren
*/

ParseStatus ArithExprParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
	AddSubExprParser ae;
	SemiColonParser semiColonP;
	ParseStatus aeParseResult = ae.parse(inputProgram);
	if(aeParseResult.status) {
		ParseStatus semiColonResult = semiColonP.parse(aeParseResult.remainingCharacters);
		aeParseResult.status = semiColonResult.status;		
		if(semiColonResult.status) {
			aeParseResult.remainingCharacters = semiColonResult.remainingCharacters;
			aeParseResult.parsedCharacters += semiColonResult.parsedCharacters;
		}
		else {
			aeParseResult.errorType = semiColonResult.errorType;
		}
	}
	 return aeParseResult;
}
