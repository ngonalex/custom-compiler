#include "frontend/combinators/v2_combinators/word_parser.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

ParseStatus WordParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	SingleCharParser charParser;
	SingleDigitParser digitParser;

	OrCombinator orCombinator;
	orCombinator.firstParser = reinterpret_cast<NullParser *>(&charParser);
	orCombinator.secondParser = reinterpret_cast<NullParser *>(&digitParser);
	ZeroOrMoreCombinator zeroOrMore;

	zeroOrMore.parser = reinterpret_cast<NullParser *>(&orCombinator);

	// Parse the first character
	ParseStatus result = charParser.parse(inputProgram);

	if (result.success){
		ParseStatus result2 = zeroOrMore.parse(result.remainingCharacters);
		result.parsedCharacters += result2.parsedCharacters;
		result.remainingCharacters = result2.remainingCharacters;
	}

	return result;
}
