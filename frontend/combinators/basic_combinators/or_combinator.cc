#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>
#define super NullParser

using namespace cs160::frontend;

ParseStatus OrCombinator::parse(std::string inputProgram,
				int startCharacter) {
  ParseStatus firstStatus = firstParser->parse(inputProgram, startCharacter);
  if (firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus = secondParser->parse(inputProgram, startCharacter);
  if(!secondStatus.status) {
  	secondStatus.errorType += " or " + firstStatus.errorType;
  }
  return secondStatus;
}
