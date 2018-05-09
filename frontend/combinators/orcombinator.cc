#include "frontend/combinators/orcombinator.h"

#define super NullParser

ParseStatus OrCombinator::parse(std::string inputProgram){
	if (inputProgram.size() == 0){
		return super::parse(inputProgram);
	}

	ParseStatus firstStatus = firstParser.parse(inputProgram);
	if (firstStatus.status){
		return firstStatus;
	}

	ParseStatus secondStatus = secondParser.parse(inputProgram);
	return secondStatus;
}