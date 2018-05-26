#include "frontend/combinators/v1_combinators/ae.h"


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
	ParseStatus aeParseResult = ae.parse(inputProgram);
    return aeParseResult;
}
