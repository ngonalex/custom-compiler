#include "frontend/combinators/single_char.h"

#define super NullParser

ParseStatus AndCombinator::parse(std::string inputProgram){
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	ParseStatus firstStatus = firstParser.parse(inputProgram);
	if (!firstStatus.status){
		return firstStatus;
	}

	ParseStatus secondStatus = secondParser.parse(firstStatus.remainingCharacters);
	return secondStatus;
}