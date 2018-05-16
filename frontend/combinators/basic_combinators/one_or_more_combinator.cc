#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus pStatus = parser->parse(inputProgram);

	while (pStatus.status) {
		ParseStatus pStatus2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = pStatus2.status;
		if (pStatus2.status) {
			pStatus.remainingCharacters = pStatus2.remainingCharacters;
			pStatus.parsedCharacters =
										 pStatus.parsedCharacters.append(pStatus2.parsedCharacters); 
		}
	}

	return pStatus;
}
