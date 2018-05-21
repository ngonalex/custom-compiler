#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus pStatus = parser->parse(inputProgram);

	if (!pStatus.status) {
		return pStatus;
	}

	while (pStatus.status) {
		ParseStatus pStatus2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = pStatus2.status;
		if (pStatus2.status){
      pStatus.parsedCharacters += (pStatus2.parsedCharacters);
			pStatus.remainingCharacters = pStatus2.remainingCharacters; 
		}
	}
	pStatus.status = true;
	return pStatus;
}
