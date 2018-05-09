#include "frontend/combinators/one_or_more_combinator.h"

#define super NullParser

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus status = parser->parse(inputProgram);

	if (!status.success){
		return status;
	}

	while (status.success){
		ParseStatus status2 = parser->parse(status.remainingCharacters);
		status.success = status2.success;
		if (status2.success){
			status.remainingCharacters = status2.remainingCharacters;
		}
	}

	status.success = true;

	return status;
}