#include "frontend/combinators/basic_combinators/or_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OrCombinator::parse(std::string inputProgram, std::string errorType){
	ParseStatus firstStatus = firstParser->parse(inputProgram);
	if (firstStatus.status){
		return firstStatus;
	}

	ParseStatus secondStatus = secondParser->parse(inputProgram);
	return secondStatus;
}
