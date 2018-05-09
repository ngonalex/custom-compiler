#include "frontend/combinators/one_or_more_combinator.h"

#define super NullParser

ParseStatus ZeroOrMoreCombinator::parse(std::string inputProgram){

	ParseStatus status;
	status.success = true;
	status.remainingCharacters = inputProgram;

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