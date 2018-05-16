#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus AndCombinator::parse(std::string inputProgram){
	ParseStatus firstStatus = firstParser->parse(inputProgram);
	if (!firstStatus.status){
		return firstStatus;
	}

	ParseStatus secondStatus = secondParser->parse(firstStatus.remainingCharacters);
	return secondStatus;
}

