#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/open_paren.h"
#include "frontend/combinators/v1_combinators/close_paren.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/add_sub_op.h"
#include "frontend/combinators/v1_combinators/mul_div_op.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#define super NullParser

/*
	ae -> num_parser  ||  ae add_sub_op ae  || open_paren ae add_sub_op ae close_paren
                    ||  ae mul_div_op ae  || open_paren ae mul_div_op ae close_paren
*/

ParseStatus ArithExprParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
	AddSubExprParser ae;
	ParseResult aeParseResult = ae.parse(inputProgram);
	if (aeParseResult.status) {
		return std::move(aeParseResult.ast);
	} else {
		printf("Error Parsing");
	}
}
