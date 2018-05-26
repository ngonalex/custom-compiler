#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus WordParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	trim(inputProgram);

	SingleVarCharParser charParser;
	SingleDigitParser digitParser;

	OrCombinator orCombinator;
	orCombinator.firstParser = reinterpret_cast<NullParser *>(&charParser);
	orCombinator.secondParser = reinterpret_cast<NullParser *>(&digitParser);
	ZeroOrMoreCombinator zeroOrMore;

	zeroOrMore.parser = reinterpret_cast<NullParser *>(&orCombinator);

	// Parse the first character
	ParseStatus result = charParser.parse(inputProgram);

	if (result.status){
		ParseStatus result2 = zeroOrMore.parse(result.remainingCharacters);
		result.parsedCharacters += result2.parsedCharacters;
		result.remainingCharacters = result2.remainingCharacters;
		result.ast = std::move(make_unique<const VariableExpr>(result.parsedCharacters));
	}
	else{
		// Error type returned to user
		result.errorType =  "variable name needs to start with char";
	}

	return result;
}
