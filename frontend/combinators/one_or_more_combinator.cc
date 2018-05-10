#include "frontend/combinators/one_or_more_combinator.h"

#define super NullParser

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus pStatus = parser->parse(inputProgram);

	if (!pStatus.status){
		return pStatus;
	}

	while (pStatus.status){
		ParseStatus pStatus2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = pStatus2.status;
		if (pStatus2.status){
			pStatus.remainingCharacters = pStatus2.remainingCharacters;
		}
	}

	pStatus.status = true;

	return pStatus;
}
