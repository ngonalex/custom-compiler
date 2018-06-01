#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>
#define super NullParser

using namespace cs160::frontend;

ParseStatus OrCombinator::parse(std::string inputProgram,
				int startCharacter,
				std::string errorType) {
  ParseStatus firstStatus = firstParser->parse(inputProgram, startCharacter);
  if (firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus = secondParser->parse(inputProgram, startCharacter);
  if(!secondStatus.status) {
  	secondStatus.errorType = firstStatus.errorType + " or " + secondStatus.errorType;
  }
  return secondStatus;
}
