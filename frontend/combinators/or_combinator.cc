#include "frontend/combinators/or_combinator.h"

#define super NullParser

ParseStatus OrCombinator::parse(std::string inputProgram){
	ParseStatus firstStatus = firstParser.parse(inputProgram);
	if (firstStatus.status){
		return firstStatus;
	}

	ParseStatus secondStatus = secondParser.parse(inputProgram);
	return secondStatus;
}