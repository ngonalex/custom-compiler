#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

ParseStatus NumParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  SingleDigitParser digitParser;
  OneOrMoreCombinator oneOrMore;
	
	oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);
	
	// Parse the integer
  ParseStatus result = oneOrMore.parse(inputProgram);
	// Make an Integer expression
	result.ast = std::move(make_unique<IntegerExpr>(stoi(result.parsedCharacters)));
	return result;
}
