#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram, std::string errorType){

  if (!pStatus.status) {
    return pStatus;
  }

	if (!pStatus.status) {
		return pStatus;
	}

	while (pStatus.status) {
		ParseStatus pStatus2 = parser->parse(pStatus.remainingCharacters);
		pStatus.status = pStatus2.status;
 		pStatus.parsedCharacters += (pStatus2.parsedCharacters);
		pStatus.remainingCharacters = pStatus2.remainingCharacters; 
		if (pStatus2.status && pStatus2.ast != NULL) {
			pStatus.astNodes.push_back(std::move(pStatus2.ast)); 
		}
	}

	if(pStatus.parsedCharacters != "")
		pStatus.status = true;

	return pStatus;
}
