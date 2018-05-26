#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include <string>

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus NumParser::parse(std::string inputProgram, std::string errorType) {
	std::string errorMessage = "Number should only have digits (0-9)";

	trim(inputProgram);

	if (inputProgram.size() == 0) { return super::parse(inputProgram, errorMessage); }

	SingleDigitParser digitParser;
  	OneOrMoreCombinator oneOrMore;

	oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);	// Feels bad
	// Parse the integer
  	ParseStatus result = oneOrMore.parse(inputProgram);
	if (result.status) {
		// Make an Integer expression
		result.ast = std::move(make_unique<IntegerExpr>(stoi(result.parsedCharacters)));
	}
	else { 
		return super::parse(inputProgram, errorMessage);
	}
	return result;
}
