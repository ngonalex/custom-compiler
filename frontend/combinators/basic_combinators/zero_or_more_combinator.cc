#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus ZeroOrMoreCombinator::parse(std::string inputProgram, std::string errorType){

	ParseStatus pStatus;
	pStatus.status = true;
	pStatus.remainingCharacters = inputProgram;


	while (pStatus.status) {
        ParseStatus status2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = status2.status;
		if (status2.status) {
			pStatus.parsedCharacters += status2.parsedCharacters;
			pStatus.remainingCharacters = status2.remainingCharacters;
			if(status2.ast != NULL) {
				pStatus.astNodes.push_back(std::move(status2.ast)); 
			}
		}
		else {
			pStatus.status = false;
			pStatus.errorType = status2.errorType;
		}
	}

	pStatus.status = true;

	return pStatus;
}

