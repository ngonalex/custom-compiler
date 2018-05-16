#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus ZeroOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus pStatus;
	pStatus.status = true;
	pStatus.remainingCharacters = inputProgram;

	while (pStatus.status){
		ParseStatus status2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = status2.status;
		if (status2.status){
			pStatus.remainingCharacters = status2.remainingCharacters;
		}
	}

	pStatus.status = true;

	return pStatus;
}
