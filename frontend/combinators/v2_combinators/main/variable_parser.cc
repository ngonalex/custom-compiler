#include "frontend/combinators/v2_combinators/helpers/word_parser.h"
#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

ParseStatus VariableParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
	WordParser wordParser;
	ZeroOrMoreCombinator zeroOrMore;

	// Parse the first character
	ParseStatus result = wordParser.parse(inputProgram);

	if (result.status){
		ParseStatus result2 = zeroOrMore.parse(result.remainingCharacters);
		result.parsedCharacters += result2.parsedCharacters;
		result.remainingCharacters = result2.remainingCharacters;
	}
	else{
		// Error type returned to user
		result.errorType =  "variableParser error";
	}

	return result;
}
